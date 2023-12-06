#pragma leco tool
import bruh;
import hai;
import jute;
import silog;

int main() {
  auto d = data::fake();
  auto it = d.begin();
  auto time_it = atoi_it((*it).split(':').after).begin();
  auto dist_it = atoi_it((*++it).split(':').after).begin();

  int num{1};
  while (time_it != atoi_it{}) {
    auto t = *time_it;
    auto d = *dist_it;

    int nr{};
    for (auto i = 0; i < t; i++) {
      if (i * (t - i) > d)
        nr++;
    }
    num *= nr;
    info("nr", nr);

    ++time_it;
    ++dist_it;
  }
  info("num", num);
}
