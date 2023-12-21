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
jute::view id_map[max_id]{};
bool parse(jute::view &in, id &v) {
  int i;
  for (i = 0; i < in.size(); i++) {
    if (in[i] < 'a' || in[i] > 'z')
      break;
  }
  auto [l, r] = in.subview(i);
  v = {.v = 0, .s = l};
  in = r;

  if (v.s == "")
    return false;

  for (v.v = 1; v.v < max_id; v.v++) {
    auto &ii = id_map[v.v];
    if (ii == "" || ii == v.s) {
      ii = v.s;
      break;
    }
  }

  return l.size() > 0;
}
id parse(jute::view in) {
  id res{};
  if (!parse(in, res))
    throw 0;
  return res;
}

namespace scan {
template <> struct parse<id> {
  bool operator()(jute::view &in, jute::view &fmt, id &v) {
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

  virtual void add_input(id from) { m_inputs.push_back(from); }
  void add_target(id i) { m_targets.push_back(i); }

protected:
  void send(bool signal) {
    for (auto t : targets()) {
      signals.push_back(sgn{m_id, t, signal});
    }
  }
};
static unsigned long gff = 0;
class ff : public node {
public:
  using node::node;

  void take(id from, bool signal) override {
    if (!signal) {
      unsigned long mask = 1UL << ident().v;
      gff ^= mask;
      send((gff & mask) != 0);
    }
  }
};
class comb : public node {
  unsigned long mem{};

public:
  using node::node;

  void add_input(id i) override {
    node::add_input(i);
    mem |= 1 << i.v;
  }

  void take(id from, bool signal) override {
    unsigned long mask = 1UL << from.v;
    if (signal) {
      mem &= ~mask;
    } else {
      mem |= mask;
    }
    send(mem != 0);
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

  for (auto rep = 0; rep < 10000000; rep++) {
    if (rep % 1000000 == 0)
      info("running", rep);
    if (rep == 1000) {
      long res{hp * lp};
      silog::log(silog::debug, "hp: %ld/39083", hp);
      silog::log(silog::debug, "lp: %ld/17061", lp);
      silog::log(silog::debug, "res: %ld/66795063", res);
    }
    signals.truncate(0);
    bcaster->take({}, false);
    lp++;
    for (auto i = 0; i < signals.size(); i++) {
      auto [from, to, v] = signals[i];

      if (rxn && to.v == rxn->ident().v && !v) {
        info("res 2", rep);
        return 0;
      }
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
  info("meh", 0);
  return 1;
}
