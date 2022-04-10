#ifndef LINE_RIDER_BALL_HPP
#define LINE_RIDER_BALL_HPP

#include <box2d/box2d.h>
#include <ftxui/dom/canvas.hpp>
#include <ftxui/screen/color.hpp>
#include <memory>

namespace term_breaker {

class BallBase {
 public:
  BallBase(b2World& world, b2Vec2 position, b2Vec2 speed, float radius);
  ~BallBase();

  float x() const;
  float y() const;
  float radius() const;

  void Draw(ftxui::Canvas& c) const;

 private:
  b2World& world_;
  b2BodyDef body_def_;
  b2Body* body_ = nullptr;
  b2CircleShape dynamic_box_;
  b2FixtureDef fixture_def_;
  ftxui::Color color_;
};

using Ball = std::unique_ptr<BallBase>;

}  // namespace term_breaker

#endif  // LINE_RIDER_BALL_HPP

// Copyright 2022 Arthur Sonzogni. All rights reserved.
// Use of this source code is governed by the MIT license that can be found in
// the LICENSE file.
