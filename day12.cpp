#pragma leco tool
import bruh;
import hai;
import jute;
import silog;

constexpr bool matches(jute::view buf, atoi_sit chk) {
  while (buf != "" && buf[0] == '.')
    buf = buf.subview(1).after;

  if (buf == "")
    return chk == atoi_sit{};

  if (chk == atoi_sit{}) {
    return buf.index_of('#') == -1;
  }
  if (buf.size() < *chk)
    return false;

  for (auto i = 0; i < *chk; i++) {
    if (buf[i] != '#')
      return false;
  }

  if (buf.size() >= *chk && buf[*chk] == '#')
    return false;

  return matches(buf.subview(*chk).after, ++chk);
}
static_assert(matches("", atoi_sit{}));
static_assert(!matches("", atoi_sit{"1", ','}));
static_assert(matches("...", atoi_sit{}));
static_assert(!matches("...", atoi_sit{"1", ','}));

static_assert(!matches(".#..", atoi_sit{}));
static_assert(!matches(".#..", atoi_sit{"1,2", ','}));

static_assert(matches(".#..", atoi_sit{"1", ','}));
static_assert(!matches(".#..", atoi_sit{"1,1", ','}));
static_assert(!matches(".##.", atoi_sit{"1,1", ','}));
static_assert(matches(".#.#.", atoi_sit{"1,1", ','}));
static_assert(matches("#.##...###", atoi_sit{"1,2,3", ','}));

static_assert(matches("#...###", atoi_sit{" 1,3", ','}));
static_assert(!matches("#...###", atoi_sit{" 1,1,3", ','}));
static_assert(matches("#.#.###", atoi_sit{"1,1,3", ','}));

int check(jute::view cbuf, char *buf, jute::view row) {
  auto [cc, rest] = row.subview(1);
  if (cc == " ") {
    auto r = matches(cbuf, {row, ','});
    return r;
  }
  if (cc == "?") {
    *buf = '#';
    auto res = check(cbuf, buf + 1, rest);
    *buf = '.';
    return res + check(cbuf, buf + 1, rest);
  }
  *buf = cc[0];
  return check(cbuf, buf + 1, rest);
}

int main(int argc, char **argv) {
  auto dt = data::of(argc);

  int res{};
  for (auto row : dt) {
    char buf[128]{};
    jute::view bv{buf, (unsigned long)row.index_of(' ')};
    res += check(bv, buf, row);
  }
  info("res", res);
}
