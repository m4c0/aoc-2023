#pragma leco tool
import bruh;
import hai;
import jute;
import silog;

int res{};

void run(jute::view line) {
  auto [gl, gr] = line.split(':');
  auto id = atoi(gl.split(' ').after);

  bool possible = true;
  auto games = gr.subview(1).after;
  while (games != "") {
    auto [game, rest] = games.split(';');

    do {
      auto [gl, gr] = game.split(',');
      auto [num_s, color] = gl.split(' ');
      auto num = atoi(num_s);
      if (color == "red" && num > 12)
        possible = false;
      if (color == "green" && num > 13)
        possible = false;
      if (color == "blue" && num > 14)
        possible = false;

      game = gr.subview(1).after;
    } while (game != "");

    games = rest.subview(1).after;
  }

  if (possible)
    res += id;
}

int main() {
  loop(run);
  info("res", res);
}
