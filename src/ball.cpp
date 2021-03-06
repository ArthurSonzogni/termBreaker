#include "ball.hpp"

namespace term_breaker {

namespace {
ftxui::Color NextHue() {
  static uint8_t hue = 0;
  const uint8_t saturation = 200;
  const uint8_t value = 200;
  hue += 50;
  return ftxui::Color::HSV(hue, saturation, value);
}
}  // namespace

BallBase::BallBase(b2World& world, b2Vec2 position, b2Vec2 speed, float radius)
    : world_(world) {
  body_def_.type = b2_dynamicBody;
  body_def_.position = position;
  body_def_.linearVelocity = speed;
  body_ = world_.CreateBody(&body_def_);
  dynamic_box_.m_radius = radius;
  fixture_def_.filter.categoryBits = 1;
  fixture_def_.filter.maskBits = 2;
  fixture_def_.shape = &dynamic_box_;
  fixture_def_.density = 1.0f;      // NOLINT
  fixture_def_.friction = 0.5f;     // NOLINT
  fixture_def_.restitution = 0.9F;  // NOLINT
  body_->CreateFixture(&fixture_def_);

  color_ = NextHue();
}

BallBase::~BallBase() {
  world_.DestroyBody(body_);
}

float BallBase::x() const {
  return body_->GetPosition().x;
}

float BallBase::y() const {
  return body_->GetPosition().y;
}

float BallBase::radius() const {
  return dynamic_box_.m_radius;
}

void BallBase::Draw(ftxui::Canvas& c) const {
  float angle = body_->GetAngle();
  c.DrawPointCircle(static_cast<int>(x()), static_cast<int>(y()),
                    static_cast<int>(radius()), color_);
  c.DrawPointLine(static_cast<int>(x() + radius() * cos(angle)),
                  static_cast<int>(y() + radius() * sin(angle)),
                  static_cast<int>(x()), static_cast<int>(y()), color_);
}

}  // namespace term_breaker

// Copyright 2022 Arthur Sonzogni. All rights reserved.
// Use of this source code is governed by the MIT license that can be found in
// the LICENSE file.
