#include <catch2/catch.hpp>
#include "game.hpp"

using namespace ftxui;

TEST_CASE("Intro return after pressing enter", "[coverage]") {
  bool enable_audio = false;
  bool quit = false;
  auto on_quit = [&] { quit = true; };
  auto component = term_breaker::Intro(&enable_audio, on_quit);

  REQUIRE(quit == false);

  // The component should be able to render without crashing.
  (void)component->Render();

  SECTION("Using arrow keys to select the other button if any") {
    component->OnEvent(Event::ArrowRight);
  }

  // The component should be able to render without crashing.
  (void)component->Render();

  // No matter what, pressing enter should quit the intro.
  REQUIRE(quit == false);
  component->OnEvent(Event::Return);
  REQUIRE(quit == true);
}
