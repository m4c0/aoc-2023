#pragma leco tool
import bruh;
import hai;
import jute;
import silog;

int rem(auto s, auto e) {
  int r{};
  while (s != e) {
    r += *s++ + 1;
  }
  return r - 1;
}
int check(auto pat_s, auto pat_e, auto chk_s, auto chk_e) {
  if (rem(chk_s, chk_e) > pat_e - pat_s)
    return 0;

  while (pat_s != pat_e && *pat_s == '.')
    pat_s++;

  if (pat_s == pat_e)
    return chk_s == chk_e ? 1 : 0;
  if (chk_s == chk_e) {
    while (pat_s != pat_e) {
      if (*pat_s++ == '#')
        return 0;
    }
    return 1;
  }

  auto r = 0;

  if (*pat_s == '?') {
    // test as '.'
    r = check(pat_s + 1, pat_e, chk_s, chk_e);
  }

  auto c = *chk_s++;
  for (auto i = 0; i < c; i++) {
    if (pat_s == pat_e)
      return r;
    if (*pat_s++ == '.')
      return r;
  }

  if (pat_s != pat_e && *pat_s++ == '#')
    return r;

  return r + check(pat_s, pat_e, chk_s, chk_e);
}

int main(int argc, char **argv) {
  auto dt = data::of(argc);

  int res{};
  for (auto row : dt) {
    auto [p, ch] = row.split(' ');
    hai::varray<char> pat{1024};
    hai::varray<int> chk{1024};

    atoi_sit chi{ch, ','};
    for (auto i = 0; i < 5; i++) {
      if (i != 0)
        pat.push_back('?');
      for (auto c : p)
        pat.push_back(c);
      for (auto n : chi)
        chk.push_back(n);
    }
    auto r = check(pat.begin(), pat.end(), chk.begin(), chk.end());
    silog::log(silog::debug, "%2d -- %.*s", r, pat.size(), pat.begin());
    res += r;
  }
  info("res", res);
}
