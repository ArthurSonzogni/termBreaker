#include <catch2/catch.hpp>
#include "game.hpp"

using namespace ftxui;

TEST_CASE("WinScreen return after pressing enter", "[coverage]") {
  const int coins = 10;
  bool quit = false;
  auto on_quit = [&] { quit = true; };
  auto component = term_breaker::WinScreen(coins, on_quit);

  // The component should be able to render without crashing.
  (void)component->Render();

  REQUIRE(quit == false);
  component->OnEvent(Event::Return);
  REQUIRE(quit == true);
}
