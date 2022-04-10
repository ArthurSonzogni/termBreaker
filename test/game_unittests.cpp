#include <catch2/catch.hpp>
#include "game.hpp"

using namespace ftxui;

namespace {
Event MakeMouseEvent(int x, int y, bool pressed) {
  auto mouse = Mouse{
      /*mouse.button=*/Mouse::Left,
      /*mouse.Motion=*/pressed ? Mouse::Pressed : Mouse::Released,
      /*mouse.shift=*/false,
      /*mouse.meta=*/false,
      /*mouse.control=*/false,
      /*mouse.x=*/x,
      /*mouse.y=*/y,
  };
  return Event::Mouse("", mouse);
}
}  // namespace

// NOLINTNEXTLINE(readability-function-cognitive-complexity)
TEST_CASE("GameScreen", "[component]") {
  term_breaker::BoardConfig config;
  config.balls = 100;  // NOLINT
  term_breaker::Board board(config);

  bool quit = false;
  bool win = false;
  auto on_quit = [&] { quit = true; };
  auto on_win = [&] { win = true; };

  auto component = term_breaker::GameScreen(board, on_win, on_quit);

  // Wait for bricks to appear:
  const int iterations = 600;
  for (int i = 0; i < iterations; ++i) {
    REQUIRE(component->OnEvent(Event::Custom) == true);
    component->Render();
  }

  // Click to throw a balls:
  const int mouse_x = 40;
  const int mouse_y = 40;
  REQUIRE(component->OnEvent(MakeMouseEvent(mouse_x, mouse_y, false)) == true);
  REQUIRE(component->OnEvent(MakeMouseEvent(mouse_x, mouse_y, true)) == false);

  // It shouldn't be possible to throw balls twice.
  REQUIRE(component->OnEvent(MakeMouseEvent(mouse_x, mouse_y, false)) == false);
  REQUIRE(component->OnEvent(MakeMouseEvent(mouse_x, mouse_y, true)) == false);

  // Wait for all the balls to fall and the user to be able to throw balls
  // again.
  while(!component->OnEvent(MakeMouseEvent(mouse_x, mouse_y, false))) {
    component->OnEvent(Event::Custom);
    component->Render();
  }
}
