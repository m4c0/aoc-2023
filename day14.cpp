#pragma leco tool
import bruh;
import hai;
import jute;
import silog;

class map_t {
  unsigned m_cycle{};
  unsigned m_cols;
  unsigned m_rows;
  hai::array<char> m_data;

public:
  map_t(unsigned rows, unsigned cols)
      : m_cols{cols}, m_rows{rows}, m_data{rows * cols} {
    if (rows != cols)
      throw 1;
  }

  auto begin() { return m_data.begin(); }

  auto copy() {
    hai::cstr res{m_cols * m_rows};
    auto p = m_data.begin();
    for (auto &c : res) {
      c = *p++;
    }
    return res;
  }

  char &get(int r, int c) {
    switch (m_cycle) {
    case 0:
      return m_data[r * m_cols + c];
    case 1:
      return m_data[c * m_cols + r];
    case 2:
      r = m_rows - r - 1;
      return m_data[r * m_cols + c];
    case 3:
      r = m_rows - r - 1;
      return m_data[c * m_cols + r];
    default:
      throw 0;
    }
  }

  void cycle(unsigned c) {
    m_cycle = c;
    for (auto c = 0; c < m_cols; c++) {
      int rr = 0;
      for (auto r = 0; r < m_rows; r++) {
        switch (get(r, c)) {
        case '.':
          break;
        case 'O':
          get(r, c) = '.';
          get(rr, c) = 'O';
          rr++;
          break;
        case '#':
          rr = r + 1;
          break;
        }
      }
    }
  }

  void full_cycle() {
    for (auto c = 0; c < 4; c++)
      cycle(c);
  }
};

bool operator==(const hai::cstr &a, const char *b) {
  for (auto c : a) {
    if (c != *b++)
      return false;
  }
  return true;
}

int main(int argc, char **argv) {
  auto dt = data::of(argc);

  unsigned rows{};
  unsigned cols{};
  for (auto row : dt) {
    cols = row.size();
    rows++;
  }
  map_t map{rows, cols};
  auto p = map.begin();
  for (auto row : dt) {
    for (auto c : row)
      *p++ = c;
  }

  struct bleh_t {
    hai::cstr map;
    long res;
    long i;
  };
  hai::varray<bleh_t> bleh{102400};
  constexpr const long limit = 1000000000;
  for (long i = 0; i < limit; i++) {
    map.full_cycle();

    hai::array<long> pts{cols};
    for (auto &p : pts) {
      p = rows;
    }

    int r{};
    long res{};
    for (auto i = 0; i < rows; i++) {
      auto row = map.begin() + r * cols;
      for (int c{}; c < cols; c++) {
        if (row[c] == 'O') {
          res += rows - r;
        }
      }
      r++;
    }

    // for (auto i = 0; i < rows; i++)
    //   silog::log(silog::debug, "%.*s", cols, map.begin() + i * cols);

    silog::log(silog::debug, "%10ld %ld", i, res);
    for (const auto &b : bleh) {
      if (res != b.res)
        continue;
      if (b.map != map.begin())
        continue;

      info("i", i);
      info("b.i", b.i);
      long x = (limit - b.i) % (i - b.i);
      i = limit - x;
    }
    bleh.push_back(bleh_t{map.copy(), res, i});
  }
}
