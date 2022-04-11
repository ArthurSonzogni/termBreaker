#include <catch2/catch.hpp>
#include "game.hpp"

using namespace ftxui;

// NOLINTNEXTLINE(readability-function-cognitive-complexity)
TEST_CASE("MainMenu", "[coverage]") {
  bool quit = false;
  int play = -1;
  auto on_quit = [&] { quit = true; };
  auto on_play = [&](int i) { play = i; };
  term_breaker::BoardConfig config;
  const int initial_coins = 2000;
  const int initial_balls = 5;
  config.balls = initial_balls;
  config.coins = initial_coins;
  auto component = term_breaker::MainMenu(config, on_play, on_quit);

  // The component should be able to render without crashing.
  (void)component->Render();

  SECTION("Player selects play") {
    REQUIRE(component->OnEvent(Event::ArrowDown) == true);
    REQUIRE(quit == false);
  }

  SECTION("Player select shop") {
    component->OnEvent(Event::ArrowRight);
    REQUIRE(component->OnEvent(Event::ArrowDown) == true);
    REQUIRE(quit == false);

    // Buy something successfully:
    REQUIRE(component->OnEvent(Event::Return) == true);
    REQUIRE(quit == false);
    REQUIRE(config.balls == 10);
    REQUIRE(config.coins == 0);

    // Buy something unsuccessfully:
    REQUIRE(component->OnEvent(Event::Return) == true);
    REQUIRE(quit == false);
    REQUIRE(config.balls == 10);
    REQUIRE(config.coins == 0);
  }

  SECTION("Player select quit") {
    component->OnEvent(Event::ArrowRight);
    component->OnEvent(Event::ArrowRight);
    component->OnEvent(Event::ArrowDown);

    SECTION("Player confirms quit") {
      REQUIRE(quit == false);
      component->OnEvent(Event::Return);
      REQUIRE(quit == true);
    }
  }
  (void)component->Render();
}
