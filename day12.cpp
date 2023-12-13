#pragma leco tool
import bruh;
import hai;
import jute;
import silog;

struct dp {
  long data[200][200]{};
};
dp gdp{};

void reset() {
  for (auto &row : gdp.data)
    for (auto &c : row)
      c = -1;
}
long check(auto pat_s, auto pat_e, auto chk_s, auto chk_e);

long mcheck(auto pat_s, auto pat_e, auto chk_s, auto chk_e) {
  auto &memo = gdp.data[pat_e - pat_s][chk_e - chk_s];
  if (memo >= 0) {
    return memo;
  }
  return memo = check(pat_s, pat_e, chk_s, chk_e);
}

long check(auto pat_s, auto pat_e, auto chk_s, auto chk_e) {
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

  long r = 0;
  // 11607695322318

  if (*pat_s == '?') {
    // test as '.'
    r = mcheck(pat_s + 1, pat_e, chk_s, chk_e);
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

  return r + mcheck(pat_s, pat_e, chk_s, chk_e);
}

int main(int argc, char **argv) {
  auto dt = data::of(argc);

  long res{};
  for (auto row : dt) {
    reset();

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
    silog::log(silog::debug, "%20ld -- %.*s", r, pat.size(), pat.begin());
    res += r;
  }
  info("res", res);
}
