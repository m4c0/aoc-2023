export module scanf;
import jute;

export namespace scan {
template <typename T> struct result {
  T v{};
  jute::view rest;
};

template <typename T> struct parse {
  constexpr result<T> operator()(jute::view val);
};
} // namespace scan

namespace {
template <typename T> constexpr auto parse_int(jute::view str) {
  typename scan::result<T> res{};

  auto st = str[0] == '-' ? 1 : 0;
  auto [l, r] = str.subview(st);
  for (auto i = 0; i < r.size(); i++) {
    auto c = r[i];
    if (c < '0' || c > '9') {
      res.rest = r.subview(i).after;
      break;
    }
    res.v = res.v * 10 + (c - '0');
  }
  res.v *= (st * -2 + 1);
  return res;
}
static_assert(parse_int<int>("10").v == 10);
static_assert(parse_int<int>("123").v == 123);
static_assert(parse_int<int>("-123").v == -123);
static_assert(parse_int<int>("123").rest == "");
static_assert(parse_int<int>("123 four").rest == " four");
} // namespace

export namespace scan {
template <> struct parse<int> {
  constexpr result<int> operator()(jute::view val) {
    return parse_int<int>(val);
  }
};
template <> struct parse<long> {
  constexpr result<long> operator()(jute::view val) {
    return parse_int<long>(val);
  }
};
} // namespace scan

static constexpr const auto ctl_chr = '\v';

template <typename... Args> struct helper {
  constexpr bool scanf(jute::view inp, jute::view fmt, Args &...a) {
    throw "unsupported argument type";
    return false;
  }
};
template <> struct helper<> {
  constexpr bool scanf(jute::view inp, jute::view fmt) {
    return inp.starts_with(fmt);
  }
};
template <typename T, typename... Args> struct helper<T, Args...> {
  constexpr bool scanf(jute::view inp, jute::view fmt, T &v, Args &...a) {
    if (inp.size() < fmt.size()) {
      return false;
    }
    for (auto i = 0; i < fmt.size(); i++) {
      if (fmt[i] == ctl_chr) {
        auto [vv, rest] = scan::parse<T>()(inp.subview(i).after);
        v = vv;
        return helper<Args...>{}.scanf(rest, fmt.subview(i + 1).after, a...);
      }
      if (inp[i] != fmt[i]) {
        return false;
      }
    }
    return false;
  }
};

export namespace scan {
template <typename... Args>
constexpr bool f(jute::view inp, jute::view fmt, Args &...a) {
  return helper<Args...>{}.scanf(inp, fmt, a...);
}
} // namespace scan

static_assert(scan::f("fine", ""));
static_assert(scan::f("this is fine", "this is"));
static_assert(!scan::f("this is fine", "this isnt"));

static_assert(-42 == [] {
  int n;
  return scan::f("got -42", "got \v", n) ? n : 9999;
}());
static_assert(-42 == [] {
  int n;
  return scan::f("got -42 here", "got \v here", n) ? n : 9999;
}());
static_assert(12 + 23l == [] {
  int a;
  long b;
  return scan::f("{a=12,b=23}", "{a=\v,b=\v}", a, b) ? a + b : 9999;
}());
