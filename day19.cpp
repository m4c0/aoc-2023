#pragma leco tool
import bruh;
import hai;
import jute;
import scanf;
import silog;

struct part {
  long n[4]; // x m a s
};
constexpr part top(jute::view line) {
  part res{};
  scan::f(line, "{x=\v,m=\v,a=\v,s=\v}", res.n[0], res.n[1], res.n[2],
          res.n[3]);
  return res;
}

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

constexpr bool lt(long a, long b) { return a < b; }
constexpr bool gt(long a, long b) { return a > b; }

namespace scan {
template <> struct parse<bool (*)(long, long)> {
  constexpr bool operator()(jute::view &in, jute::view &fmt,
                            bool (*&v)(long, long)) {
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
  bool (*op)(long, long){};
  long rhs{};
  int outcome{};
};
struct wflow {
  int key{-1};
  inst insts[10]{};
  int icount{};
  int fallback{};
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

  wflow res{.key = tok(key)};

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

hai::array<wflow> wfs{10240};

long run(jute::view line) {
  auto xmas = top(line);
  const auto *inst = &wfs[tok("in")];

  while (true) {
    if (inst->icount == 0)
      throw 0;

    int outcome = inst->fallback;
    for (auto i = 0; i < inst->icount; i++) {
      auto &op = inst->insts[i];
      auto lhs = xmas.n[op.lhs];
      if (!op.op(lhs, op.rhs)) {
        continue;
      }
      outcome = op.outcome;
      break;
    }

    if (outcome == R) {
      info("reject", 0);
      return 0;
    }
    if (outcome == A) {
      info("accept", xmas.n[0] + xmas.n[1] + xmas.n[2] + xmas.n[3]);
      return xmas.n[0] + xmas.n[1] + xmas.n[2] + xmas.n[3];
    }

    info("loop", outcome);
    inst = &wfs[outcome];
  }
}

int main(int argc, char **argv) {
  auto dt = data::of(argc);

  long res{};
  bool running{};
  for (auto line : dt) {
    if (line == "") {
      info("run", 0);
      running = true;
    } else if (!running) {
      auto i = tow(line);
      wfs[i.key] = i;
    } else {
      res += run(line);
    }
  }

  info("res", res);
}
