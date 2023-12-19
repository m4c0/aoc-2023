export module scanf;
import jute;

export namespace scan {
template <typename T> struct parse {
  constexpr bool operator()(jute::view &in, jute::view &fmt, T &v);
};
} // namespace scan

namespace {
template <typename T>
constexpr bool parse_int(jute::view &str, jute::view &fmt, T &v) {
  v = {};

  auto st = str[0] == '-' ? 1 : 0;
  auto [l, r] = str.subview(st);
  auto i = 0;
  for (; i < r.size(); i++) {
    auto c = r[i];
    if (c < '0' || c > '9') {
      break;
    }
    v = v * 10 + (c - '0');
  }
  v *= (st * -2 + 1);
  str = r.subview(i).after;
  return i != 0;
}
static_assert(![] {
  jute::view str = "";
  jute::view fmt = "";
  int v;
  return parse_int(str, fmt, v);
}());
static_assert(![] {
  jute::view str = "asd";
  jute::view fmt = "";
  int v;
  return parse_int(str, fmt, v);
}());
static_assert(34 == [] {
  jute::view str = "34";
  jute::view fmt = "a";
  int v;
  if (!parse_int(str, fmt, v))
    return 9999;
  if (str != "")
    return 9998;
  if (fmt != "a")
    return 9997;
  return v;
}());
static_assert(-123 == [] {
  jute::view str = "-123aaa";
  jute::view fmt = "xxx";
  int v;
  if (!parse_int(str, fmt, v))
    return 9999;
  if (str != "aaa")
    return 9998;
  if (fmt != "xxx")
    return 9997;
  return v;
}());
} // namespace

export namespace scan {
template <> struct parse<int> {
  constexpr bool operator()(jute::view &inp, jute::view &fmt, int &v) {
    return parse_int<int>(inp, fmt, v);
  }
};
template <> struct parse<long> {
  constexpr bool operator()(jute::view &inp, jute::view &fmt, long &v) {
    return parse_int<long>(inp, fmt, v);
  }
};
template <> struct parse<char> {
  constexpr bool operator()(jute::view &inp, jute::view &fmt, char &v) {
    if (inp.size() == 0)
      return false;
    v = inp[0];
    inp = inp.subview(1).after;
    return true;
  }
};
template <> struct parse<jute::view> {
  constexpr bool operator()(jute::view &inp, jute::view &fmt, jute::view &v) {
    auto i = 0;
    if (fmt == "") {
      v = inp;
      return true;
    }
    for (; i < inp.size(); i++) {
      if (inp[i] == fmt[0]) {
        auto [l, r] = inp.subview(i);
        v = l;
        inp = r;
        return true;
      }
    }
    return false;
  }
};
} // namespace scan

namespace {
static constexpr const auto ctl_chr = '\v';

template <typename... Args> struct helper {
  constexpr bool scanf(jute::view inp, jute::view fmt, Args &...a);
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
        inp = inp.subview(i).after;
        fmt = fmt.subview(i + 1).after;
        if (!scan::parse<T>{}(inp, fmt, v)) {
          return false;
        }

        return helper<Args...>{}.scanf(inp, fmt, a...);
      }
      if (inp[i] != fmt[i]) {
        return false;
      }
    }
    throw "more arguments than formats defined";
  }
};
} // namespace

export namespace scan {
template <typename... Args>
constexpr bool f(jute::view inp, jute::view fmt, Args &...a) {
  return helper<Args...>{}.scanf(inp, fmt, a...);
}
} // namespace scan

static_assert(scan::f("fine", ""));
static_assert(scan::f("this is fine", "this is"));
static_assert(!scan::f("this is fine", "this isnt"));

static_assert('o' == [] {
  char n;
  return scan::f("got ok", "got \v", n) ? n : 'f';
}());
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
static_assert("fine" == [] {
  jute::view v;
  return scan::f("this is fine", "this is \v", v) ? v : "error";
}());
static_assert(![] {
  jute::view v;
  return scan::f("this is fine", "this \vnt fine", v);
}());
static_assert("is" == [] {
  jute::view v;
  return scan::f("this is fine", "this \v fine", v) ? v : "error";
}());
