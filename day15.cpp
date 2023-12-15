#pragma leco tool
import bruh;
import hai;
import jute;
import silog;

constexpr auto hash(jute::view v) {
  int res{};
  for (auto c : v) {
    res += c;
    res *= 17;
    res %= 256;
  }
  return res;
}
static_assert(hash("HASH") == 52);

struct bucket {
  jute::view key;
  int v;
  bucket *next;
  bucket *prev;
};
struct blist {
  bucket *head;
  bucket *tail;
};

constexpr auto split(jute::view v) {
  struct {
    jute::view a;
    char op;
    int num{};
  } res;

  auto [lbl, r] = v.split('=');
  if (r != "") {
    res.a = lbl;
    res.op = '=';
    res.num = atoi(r);
  } else {
    auto [lbl, r] = v.split('-');
    res.a = lbl;
    res.op = '-';
  }
  return res;
}

int main(int argc, char **argv) {
  auto dt = data::of(argc);
  auto line = *(dt.begin());

  blist map[256]{};

  while (line != "") {
    auto [l, r] = line.split(',');
    auto [lbl, op, v] = split(l);

    auto h = hash(lbl);
    auto &bl = map[h];
    auto p = bl.head;
    while (p) {
      if (p->key == lbl) {
        break;
      }
      p = p->next;
    }

    if (op == '-') {
      if (p) {
        if (p == bl.head)
          bl.head = bl.head->next;
        if (p == bl.tail)
          bl.tail = bl.tail->prev;
        if (p->prev)
          p->prev->next = p->next;
        if (p->next)
          p->next->prev = p->prev;
      }
    } else if (p) {
      p->v = v;
    } else {
      auto n = new bucket{lbl, v, nullptr, bl.tail};
      if (bl.tail) {
        bl.tail->next = n;
      } else {
        bl.head = n;
      }
      bl.tail = n;
    }

    line = r;
  }

  int sum{};
  for (auto b = 0; b < 256; b++) {
    auto i = 1;
    auto p = map[b].head;
    while (p) {
      silog::log(silog::debug, "%d %.2s %d", b, p->key.data(), p->v);
      sum += (b + 1) * i * p->v;
      i++;
      p = p->next;
    }
  }
  info("sum", sum);
}
