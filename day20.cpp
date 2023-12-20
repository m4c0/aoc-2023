#pragma leco tool
import bruh;
import hai;
import jute;
import scanf;
import silog;

struct id {
  int v;
  jute::view s;
};
constexpr const auto max_id = 30 * 30 * 30;
constexpr bool parse(jute::view &in, id &v) {
  v = {};
  int i;
  for (i = 0; i < in.size(); i++) {
    auto c = in[i];
    if (c < 'a' || c > 'z')
      break;
    v.v *= 28;
    v.v += c - 'a' + 1;
  }
  auto [l, r] = in.subview(i);
  v.s = l;
  in = r;
  return i > 0;
}
constexpr id parse(jute::view in) {
  id res{};
  if (!parse(in, res))
    throw 0;
  return res;
}

namespace scan {
template <> struct parse<id> {
  constexpr bool operator()(jute::view &in, jute::view &fmt, id &v) {
    return ::parse(in, v);
  }
};
} // namespace scan

struct sgn {
  id from;
  id target;
  bool value;
};
hai::varray<sgn> signals{1024000};

class node {
  hai::varray<id> m_targets{100};
  id m_id;
  hai::varray<id> m_inputs{100};

public:
  explicit constexpr node(id i) : m_id{i} {}

  const auto &ident() const noexcept { return m_id; }
  const auto &inputs() const noexcept { return m_inputs; }
  const auto &targets() const noexcept { return m_targets; }

  virtual void take(id from, bool signal) = 0;
  virtual void dump() {}

  void add_input(id from) { m_inputs.push_back(from); }
  void add_target(id i) { m_targets.push_back(i); }

protected:
  void send(bool signal) {
    for (auto t : targets()) {
      signals.push_back(sgn{m_id, t, signal});
    }
  }
};
class ff : public node {
  bool status{};

public:
  using node::node;

  void take(id from, bool signal) override {
    if (!signal) {
      status = !status;
      send(status);
    }
  }

  void dump() override {
    silog::log(silog::debug, "%%%.*s = %s", (int)ident().s.size(),
               ident().s.data(), status ? "on" : "off");
  }
};
class comb : public node {
  bool mem[max_id]{};

public:
  using node::node;

  void take(id from, bool signal) override {
    mem[from.v] = signal;
    for (auto id : inputs()) {
      if (!mem[id.v]) {
        send(true);
        return;
      }
    }
    send(false);
  }
};
class bcast : public node {
public:
  using node::node;

  void take(id from, bool signal) override { send(signal); }
};
class rx : public node {
public:
  using node::node;

  void take(id from, bool signal) override {}
};

int main(int argc, char **argv) {
  auto dt = data::of(argc);

  node *bcaster{};
  node *rxn{};
  hai::array<node *> nodes{max_id};

  for (auto line : dt) {
    node *n;
    char tp{};
    id name{};
    jute::view out;
    if (scan::f(line, "broadcaster -> \v", out)) {
      bcaster = n = new bcast{name};
      name.s = "broadcaster";
    } else if (scan::f(line, "\v\v -> \v", tp, name, out)) {
      if (tp == '%') {
        n = new ff{name};
      } else if (tp == '&') {
        n = new comb{name};
      } else {
        throw 0;
      }
    } else {
      throw 0;
    }
    nodes[name.v] = n;

    id tgt{};
    while (parse(out, tgt)) {
      n->add_target(tgt);
      out = out.subview(1).after.trim();
    }
  }
  for (auto n : nodes) {
    if (!n)
      continue;
    for (auto t : n->targets()) {
      if (t.s == "rx") {
        rxn = nodes[t.v] = new rx{t};
      }
      if (nodes[t.v])
        nodes[t.v]->add_input(n->ident());
    }
  }

  long hp{};
  long lp{};

  for (auto rep = 0; rep < 1000; rep++) {
    signals.truncate(0);
    bcaster->take({}, false);
    lp++;
    for (auto i = 0; i < signals.size(); i++) {
      auto [from, to, v] = signals[i];

      /*
      silog::log(silog::debug, "%.*s -%s-> %.*s", (int)from.s.size(),
                 from.s.data(), v ? "high" : "low", (int)to.s.size(),
                 to.s.data());
                 */

      if (v) {
        hp++;
      } else {
        lp++;
      }
      if (nodes[to.v])
        nodes[to.v]->take(from, v);
    }
    // for (auto s : nodes) s->dump();
  }

  info("rxn", rxn ? "Y" : "N");
  hai::varray<int> vis{100};
  const auto back = [&](auto back, node *n, int i) -> void {
    bool ok{true};
    for (auto v : vis)
      if (v == n->ident().v)
        ok = false;

    silog::log(silog::debug, "%*s%.*s%s", i, "", (int)n->ident().s.size(),
               n->ident().s.data(), ok ? "" : " loop!");
    if (!ok)
      return;
    vis.push_back(n->ident().v);
    for (auto nn : n->inputs()) {
      back(back, nodes[nn.v], i + 2);
    }
    vis.pop_back();
  };
  back(back, rxn, 0);

  long res{hp * lp};
  info("hp", hp);
  info("lp", lp);
  info("res", res);
}
