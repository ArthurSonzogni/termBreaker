#include <box2d/box2d.h>
#include <cmath>
#include <ftxui/component/component.hpp>
#include <ftxui/component/component_base.hpp>
#include <ftxui/component/event.hpp>
#include <ftxui/component/mouse.hpp>
#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/dom/canvas.hpp>
#include <ftxui/dom/elements.hpp>
#include <ftxui/screen/color.hpp>
#include <ftxui/screen/screen.hpp>
#include <iostream>
#include <memory>
#include <string>
#include <utility>
#include <vector>

#include "ball.hpp"
#include "brick.hpp"
#include "contact_listener.hpp"

int main() {
  using namespace ftxui;

  // Define the gravity vector.
  b2Vec2 gravity(0.0f, 70.0f);  // NOLINT
  b2World world(gravity);

  ContactListener contact_listener;
  world.SetContactListener(&contact_listener);

  // Call the body factory which allocates memory for the ground body
  // from a pool and creates the ground box shape (also from a pool).
  // The body is also added to the world.
  b2BodyDef groundBodyDef;
  groundBodyDef.position.Set(75.f, 148.0f);  // NOLINT
  b2Body* groundBody = world.CreateBody(&groundBodyDef);
  b2PolygonShape groundBox;
  groundBox.SetAsBox(75.0f, 1.0f);              // NOLINT
  groundBody->CreateFixture(&groundBox, 0.0f);  // NOLINT

  using Circle = std::unique_ptr<CircleBase>;

  std::vector<Circle> circles;

  std::vector<std::unique_ptr<BrickBase>> bricks;
  bricks.push_back(std::make_unique<BrickBase>(world, 100, 50, 20, 20));  // NOLINT
  bricks.push_back(std::make_unique<BrickBase>(world, 100, 80, 10, 10));  // NOLINT
  bricks.push_back(
      std::make_unique<BrickBase>(world, 75, 146, 100, 6));  // NOLINT

  // A triangle following the mouse, using braille characters.
  auto renderer = Renderer([&] {
    auto c = Canvas(150, 150);  // NOLINT

    for(auto& circle : circles) {
      circle->Draw(c);
    }
    for (auto& brick : bricks) {
      brick->Draw(c);
    }
    return canvas(std::move(c)) | border | bold;
  });

  renderer |= CatchEvent([&](Event event) {  // NOLINT
    if (event == Event::Custom) {
      // Prepare for simulation. Typically we use a time step of 1/60 of a
      // second (60Hz) and 10 iterations. This provides a high quality
      // simulation in most game scenarios.
      float timeStep = 1.0f / 60.0f;  // NOLINT
      int32 velocityIterations = 6;   // NOLINT
      int32 positionIterations = 2;   // NOLINT
      // Instruct the world to perform a single step of simulation.
      // It is generally best to keep the time step and iterations fixed.
      world.Step(timeStep, velocityIterations, positionIterations);

      for (auto& brick : bricks) {
        brick->Step();
      }
      return true;
    }

    if (event.is_mouse() && event.mouse().motion == Mouse::Released) {
      const int mouse_x = (event.mouse().x - 1) * 2;
      const int mouse_y = (event.mouse().y - 1) * 4;
      circles.push_back(std::make_unique<CircleBase>(world, mouse_x, mouse_y,
                                                     3.F));  // NOLINT

      return true;
    };

    return false;
  });

  auto screen = ScreenInteractive::FitComponent();
  // This thread exists to make sure that the event queue has an event to
  // process at approximately a rate of 30 FPS
  std::atomic<bool> refresh_ui_continue = true;
  std::thread refresh_ui([&] {
    while (refresh_ui_continue) {
      using namespace std::chrono_literals;
      std::this_thread::sleep_for(1.0s / 60.0);  // NOLINT magic numbers
      screen.PostEvent(ftxui::Event::Custom);
    }
  });
  screen.Loop(renderer);

  refresh_ui_continue = false;
  refresh_ui.join();


  return 0;
}
