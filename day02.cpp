#pragma leco tool
import bruh;
import hai;
import jute;
import silog;

int res{};

void run(jute::view line) {
  auto [gl, gr] = line.split(':');
  auto id = atoi(gl.split(' ').after);

  auto max_r{0};
  auto max_g{0};
  auto max_b{0};
  auto games = gr.subview(1).after;
  while (games != "") {
    auto [game, rest] = games.split(';');

    do {
      auto [gl, gr] = game.split(',');
      auto [num_s, color] = gl.split(' ');
      auto num = atoi(num_s);

      if (color == "red") {
        mx(max_r, num);
      } else if (color == "green") {
        mx(max_g, num);
      } else if (color == "blue") {
        mx(max_b, num);
      } else {
        throw 0;
      }

      game = gr.subview(1).after;
    } while (game != "");

    games = rest.subview(1).after;
  }
  silog::log(silog::debug, "%d %d %d", max_r, max_g, max_b);
  res += max_r * max_g * max_b;
}

int main() {
  loop(run);
  info("res", res);
}
