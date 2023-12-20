#pragma leco tool
import bruh;
import hai;
import jute;
import scanf;
import silog;

struct part {
  // x m a s
  long min[4]{1, 1, 1, 1};
  long max[4]{4000, 4000, 4000, 4000};
};

constexpr const auto A = 1000000;
constexpr const auto R = 2000000;

constexpr int tok(jute::view key) {
  if (key == "A")
    return A;
  if (key == "R")
    return R;

  int res{};
  for (auto c : key) {
    res *= 28;
    res += c - 'a' + 1;
  }
  return res;
}
static_assert(tok("") == 0);
static_assert(tok("A") == A);
static_assert(tok("R") == R);
static_assert(tok("aaa") == 813);
static_assert(tok("aab") == 814);

struct qpart : part {
  int wf{tok("in")};
};
void info(const char *lbl, int col, const qpart &p) {
  silog::log(silog::info,
             "\e[0;%dm%8s: x=[%4ld;%4ld] m=[%4ld;%4ld] a=[%4ld;%4ld] "
             "s=[%4ld;%4ld]\e[0m wf:%d",
             col, lbl, p.min[0], p.max[0], p.min[1], p.max[1], p.min[2],
             p.max[2], p.min[3], p.max[3], p.wf);
}

using fn_t = void (*)(long &, long &, long);
constexpr void lt(long &min, long &max, long thr) {
  mn(min, thr - 1);
  mn(max, thr - 1);
}
static_assert([] {
  long min = 1;
  long max = 4000;
  lt(min, max, 1665);
  return min == 1 && max == 1664;
}());

constexpr void gt(long &min, long &max, long thr) {
  mx(min, thr + 1);
  mx(max, thr + 1);
}
static_assert([] {
  long min = 1;
  long max = 4000;
  gt(min, max, 1665);
  return min == 1666 && max == 4000;
}());

namespace scan {
template <> struct parse<fn_t> {
  constexpr bool operator()(jute::view &in, jute::view &fmt, fn_t &v) {
    if (in[0] == '<') {
      v = lt;
    } else if (in[0] == '>') {
      v = gt;
    } else {
      return false;
    }
    in = in.subview(1).after;
    return true;
  }
};
} // namespace scan

struct inst {
  int lhs{-1};
  fn_t op{};
  long rhs{};
  int outcome{};
};
struct wflow {
  int key{-1};
  inst insts[10]{};
  int icount{};
  int fallback{};
  jute::view line{};
};
constexpr int toi(char c) {
  switch (c) {
  case 'x':
    return 0;
  case 'm':
    return 1;
  case 'a':
    return 2;
  case 's':
    return 3;
  default:
    throw 0;
  }
}
constexpr wflow tow(jute::view line) {
  jute::view key;
  jute::view ops;
  if (!scan::f(line, "\v{\v}", key, ops))
    throw 0;

  wflow res{.key = tok(key), .line = line};

  for (auto op : split_it<','>{ops}) {
    inst &i = res.insts[res.icount];
    char lhs;
    jute::view out;
    if (scan::f(op, "\v\v\v:\v", lhs, i.op, i.rhs, out)) {
      i.lhs = toi(lhs);
      i.outcome = tok(out);
      res.icount++;
    } else if (op.size() <= 3) {
      res.fallback = tok(op);
      break;
    } else {
      throw 0;
    }
  }

  return res;
}
static constexpr const auto test = tow("px{a<2006:qkq,m>2090:A,rfg}");
static_assert(test.key == tok("px"));
static_assert(test.icount == 2);
static_assert(test.fallback == tok("rfg"));
static_assert(test.insts[0].lhs == 2);
static_assert(test.insts[0].op == lt);
static_assert(test.insts[0].rhs == 2006);
static_assert(test.insts[0].outcome == tok("qkq"));
static_assert(test.insts[1].lhs == 1);
static_assert(test.insts[1].op == gt);
static_assert(test.insts[1].rhs == 2090);
static_assert(test.insts[1].outcome == A);

qpart apply(const inst &op, qpart &q) {
  auto lhs = op.lhs;
  if (q.min[lhs] > q.max[lhs]) {
    return q;
  }

  qpart qc = q;
  op.op(qc.min[lhs], qc.max[lhs], op.rhs);
  if (qc.min[lhs] > q.min[lhs]) {
    q.max[lhs] = qc.min[lhs] - 1;
  }
  if (qc.max[lhs] < q.max[lhs]) {
    q.min[lhs] = qc.max[lhs] + 1;
  }
  return qc;
}
bool valid(const qpart &q) {
  for (auto i = 0; i < 4; i++) {
    if (q.min[i] > q.max[i])
      return false;
  }
  return true;
}

int main(int argc, char **argv) {
  auto dt = data::of(argc);
  hai::array<wflow> wfs{30 * 30 * 30};

  for (auto line : dt) {
    if (line == "") {
      break;
    }
    auto i = tow(line);
    wfs[i.key] = i;
  }

  hai::varray<qpart> queue{102400};
  queue.push_back(qpart{});

  int rcount{};
  int acount{};
  long res{};
  const auto oc = [&](auto outcome, auto &q) {
    if (outcome == R) {
      rcount++;
      info("reject", 31, q);
    } else if (outcome == A) {
      acount++;
      info("accept", 33, q);

      if (valid(q)) {
        long rate{1};
        for (auto i = 0; i < 4; i++) {
          rate *= q.max[i] - q.min[i] + 1;
        }
        res += rate;
      }
    } else {
      q.wf = outcome;
      info("enqueue", 35, q);
      queue.push_back(q);
    }
  };

  while (queue.size() > 0) {
    auto q = queue.pop_back();
    const auto *inst = &wfs[q.wf];
    silog::log(silog::info, "-=-=-=-=-=-=-=-=-=-=-=-=-=-");
    info("dequeue", 37, q);
    info("   wflow", inst->line);

    for (auto i = 0; i < inst->icount; i++) {
      info("check", 32, q);
      auto &op = inst->insts[i];

      auto lhs = op.lhs;
      qpart qc = apply(op, q);

      if (qc.min[lhs] <= qc.max[lhs]) {
        oc(op.outcome, qc);
      }
    }

    oc(inst->fallback, q);
  }

  info("r", rcount);
  info("a", acount);

  info("exp", 167409079868000L);
  info("res", res);
}
