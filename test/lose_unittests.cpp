#include <catch2/catch_test_macros.hpp>
#include "game.hpp"

using namespace ftxui;

TEST_CASE("Lose return after pressing enter", "[component]") {
  const int coins = 10;
  bool quit = false;
  auto on_quit = [&] { quit = true; };
  auto component = term_breaker::WinScreen(coins, on_quit);

  // The component should be able to render without crashing.
  component->Render();

  // The component should be able to quit, by pressing enter.
  REQUIRE(quit == false);
  component->OnEvent(Event::Return);
  REQUIRE(quit == true);
}
