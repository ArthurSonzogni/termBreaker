#include <catch2/catch.hpp>
#include "game.hpp"

using namespace ftxui;

// NOLINTNEXTLINE(readability-function-cognitive-complexity)
TEST_CASE("MainMenu", "[coverage]") {
  bool quit = false;
  bool play = false;
  auto on_quit = [&] { quit = true; };
  auto on_play = [&] { play = true; };
  auto component = term_breaker::MainMenu(on_play, on_quit);

  // The component should be able to render without crashing.
  (void)component->Render();

  SECTION("Player selects play") {
    REQUIRE(component->OnEvent(Event::ArrowDown) == false);
    REQUIRE(quit == false);
  }

  SECTION("Player select shop") {
    component->OnEvent(Event::ArrowRight);
    REQUIRE(component->OnEvent(Event::ArrowDown) == false);
    REQUIRE(quit == false);
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
