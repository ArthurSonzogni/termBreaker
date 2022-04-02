#include <box2d/box2d.h>
#include <memory>
#include <cmath>  // for sin, cos
#include <ftxui/component/component.hpp>  // for Renderer, CatchEvent, Horizontal, Menu, Tab
#include <ftxui/component/component_base.hpp>      // for ComponentBase
#include <ftxui/component/event.hpp>               // for Event
#include <ftxui/component/mouse.hpp>               // for Mouse
#include <ftxui/component/screen_interactive.hpp>  // for ScreenInteractive
#include <ftxui/dom/canvas.hpp>                    // for Canvas
#include <ftxui/dom/elements.hpp>  // for canvas, Element, separator, hbox, operator|, border
#include <ftxui/screen/color.hpp>  // for Color, Color::Red, Color::Blue, Color::Green, ftxui
#include <ftxui/screen/screen.hpp>  // for Pixel
#include <iostream>
#include <memory>   // for allocator, shared_ptr, __shared_ptr_access
#include <string>   // for string, basic_string
#include <utility>  // for move
#include <vector>   // for vector, __alloc_traits<>::value_type

int main() {
  using namespace ftxui;

  // Define the gravity vector.
  b2Vec2 gravity(0.0f, 70.0f);  // NOLINT
  b2World world(gravity);

  // Call the body factory which allocates memory for the ground body
  // from a pool and creates the ground box shape (also from a pool).
  // The body is also added to the world.
  b2BodyDef groundBodyDef;
  groundBodyDef.position.Set(75.f, 148.0f);  // NOLINT
  b2Body* groundBody = world.CreateBody(&groundBodyDef);
  b2PolygonShape groundBox;
  groundBox.SetAsBox(75.0f, 1.0f);              // NOLINT
  groundBody->CreateFixture(&groundBox, 0.0f);  // NOLINT

  class CircleBase {
   public:
    CircleBase(b2World& world, float x, float y, float radius) {
      bodyDef.type = b2_dynamicBody;
      bodyDef.position.Set(x, y);  // NOLINT
      body = world.CreateBody(&bodyDef);
      dynamicBox.m_radius = radius;
      fixtureDef.shape = &dynamicBox;
      fixtureDef.density = 1.0f;   // NOLINT
      fixtureDef.friction = 3.0f;  // NOLINT
      body->CreateFixture(&fixtureDef);

      static uint8_t hue = 0;
      hue += 50; // NOLINT
      color = Color::HSV(hue, 200, 200); // NOLINT;
    }

    [[nodiscard]] float x() const {
      return body->GetPosition().x;
    }  // NOLINT
    [[nodiscard]] float y() const {
      return body->GetPosition().y;
    }  // NOLINT
    [[nodiscard]] float radius() const { return dynamicBox.m_radius; }

    void Draw(Canvas& c) const {
      float angle = body->GetAngle();
      c.DrawPointCircle(static_cast<int>(x()), static_cast<int>(y()),
                        static_cast<int>(radius()), color);
      c.DrawPointLine(static_cast<int>(x() + radius() * cos(angle)),
                      static_cast<int>(y() + radius() * sin(angle)),
                      static_cast<int>(x()), static_cast<int>(y()), color);
    }

   private:
    b2BodyDef bodyDef;
    b2Body* body;
    b2CircleShape dynamicBox;
    b2FixtureDef fixtureDef;
    Color color;
  };
  using Circle = std::unique_ptr<CircleBase>;

  std::vector<Circle> circles;
  circles.push_back(
      std::make_unique<CircleBase>(world, 10.F, 30.F, 20.F));  // NOLINT
  circles.push_back(
      std::make_unique<CircleBase>(world, 30.F, 0.F, 30.F));  // NOLINT
  circles.push_back(
      std::make_unique<CircleBase>(world, 40.F, 30.F, 15.F));  // NOLINT
  // A triangle following the mouse, using braille characters.
  auto renderer = Renderer([&] {
    auto c = Canvas(150, 150);  // NOLINT

    for(auto& circle : circles) {
      circle->Draw(c);
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
      return true;
    }

    if (event.is_mouse() && event.mouse().motion == Mouse::Released) {
      const int mouse_x = (event.mouse().x - 1) * 2;
      const int mouse_y = (event.mouse().y - 1) * 4;
      circles.push_back(std::make_unique<CircleBase>(world, mouse_x, mouse_y,
                                                     15.F));  // NOLINT

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
