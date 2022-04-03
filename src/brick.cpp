#include "brick.hpp"

namespace {
void DrawRectangle(ftxui::Canvas& c,
                   int x_min,
                   int x_max,
                   int y_min,
                   int y_max,
                   ftxui::Color color) {

  auto style = [color](ftxui::Pixel& pixel) {
    pixel.background_color = color;
  };
  for (int y = y_min; y <= y_max; y += 4) {
    for (int x = x_min; x <= x_max; x += 2) {
      c.DrawPoint(x, y, false, style);
    }
  }
}

};  // namespace

BrickBase::BrickBase(b2World& world,
                     float x,
                     float y,
                     int half_width,
                     int half_height)
  :
    half_width_(half_width),
    half_height_(half_height)
{
  bodyDef.type = b2_staticBody;
  bodyDef.position.Set(x, y);          // NOLINT
  body = world.CreateBody(&bodyDef);
  body->GetUserData().pointer = reinterpret_cast<uintptr_t>(this);  // NOLINT
  dynamicBox.SetAsBox(static_cast<float>(half_width_),
                      static_cast<float>(half_height_));
  fixtureDef.shape = &dynamicBox;
  fixtureDef.density = 1.0f;   // NOLINT
  fixtureDef.friction = 3.0f;  // NOLINT
  body->CreateFixture(&fixtureDef);

  static uint8_t hue = 0;
  hue += 50;                                 // NOLINT
  color_hue_ = hue;
}

void BrickBase::Step() {
  if (color_value_ < 200)                            // NOLINT
    color_value_ += 10;                               // NOLINT
};

void BrickBase::OnContact() {
  if (counter_ > 0) {
    counter_--;
    color_value_ = 100;  // NOLINT
  }
}

void BrickBase::Draw(ftxui::Canvas& c) const {
  int X = static_cast<int>(body->GetPosition().x);  // NOLINT
  int Y = static_cast<int>(body->GetPosition().y);  // NOLINT
  DrawRectangle(c, X - half_width_, X + half_width_, Y - half_height_,
                Y + half_height_,
                ftxui::Color::HSV(color_hue_, 255, color_value_));  // NOLINT
  std::string label = std::to_string(counter_);
  c.DrawText(X - static_cast<int>(label.size()) / 2, Y, label,
             [](ftxui::Pixel& pixel) {
               pixel.foreground_color = ftxui::Color::Black;
             });
}
