#include "brick.hpp"
#include <box2d/box2d.h>
#include "resources.hpp"

namespace {
void DrawRectangle(ftxui::Canvas& c,
                   int x_min,
                   int x_max,
                   int y_min,
                   int y_max,
                   ftxui::Color color) {
  auto style = [color](ftxui::Pixel& pixel) { pixel.background_color = color; };
  for (int y = y_min; y < y_max; y += 4) {
    for (int x = x_min; x < x_max; x += 2) {
      c.DrawPoint(x, y, false, style);
    }
  }
}

};  // namespace

BrickBase::BrickBase(b2World& world,
                     int x,
                     int y,
                     int half_width,
                     int half_height,
                     int counter)
    : world_(world),
      x_(x),
      y_(y),
      half_width_(half_width),
      half_height_(half_height),
      counter_(counter) {
  bodyDef_.type = b2_staticBody;
  bodyDef_.position.Set(static_cast<float>(x_), static_cast<float>(y_));
  body_ = world_.CreateBody(&bodyDef_);
  body_->GetUserData().pointer = reinterpret_cast<uintptr_t>(this);  // NOLINT
  dynamicBox_.SetAsBox(static_cast<float>(half_width_),
                       static_cast<float>(half_height_));
  fixtureDef_.filter.categoryBits = 2;
  fixtureDef_.filter.maskBits = 1;
  fixtureDef_.shape = &dynamicBox_;
  fixtureDef_.density = 1.0f;   // NOLINT
  fixtureDef_.friction = 3.0f;  // NOLINT
  body_->CreateFixture(&fixtureDef_);

  static uint8_t hue = 0;
  hue += 50;  // NOLINT
  color_hue_ = hue;
}

BrickBase::~BrickBase() {
  world_.DestroyBody(body_);
}

void BrickBase::Step() {
  if (color_value_ < 200)  // NOLINT
    color_value_ += 10;    // NOLINT
};

void BrickBase::OnContact() {
  if (counter_ > 0) {
    counter_--;
    color_value_ = 100;  // NOLINT
    term_breaker::PlaySound(term_breaker::sb_boing);
  }
}

void BrickBase::Draw(ftxui::Canvas& c) const {
  const auto hue = static_cast<uint8_t>(counter_ * 2);
  DrawRectangle(c,                                           //
                x_ - half_width_,                            //
                x_ + half_width_,                            //
                y_ - half_height_,                           //
                y_ + half_height_,                           //
                ftxui::Color::HSV(hue, 255, color_value_));  // NOLINT
  std::string label = std::to_string(counter_);

  const auto color_foreground_black = [](ftxui::Pixel& pixel) {
    pixel.foreground_color = ftxui::Color::Black;
  };

  c.DrawText(x_ - static_cast<int>(label.size()) / 2, y_, label,
             color_foreground_black);
}

void BrickBase::MoveUp() {
  y_ -= 4;
  body_->SetTransform(b2Vec2(static_cast<float>(x_), static_cast<float>(y_)),
                      0.F);
}

// Copyright 2022 Arthur Sonzogni. All rights reserved.
// Use of this source code is governed by the MIT license that can be found in
// the LICENSE file.
