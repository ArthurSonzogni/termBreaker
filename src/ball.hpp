#ifndef LINE_RIDER_BALL_HPP
#define LINE_RIDER_BALL_HPP

#include <box2d/box2d.h>
#include <ftxui/dom/canvas.hpp>
#include <ftxui/screen/color.hpp>
#include <memory>

class BallBase {
 public:
  BallBase(b2World& world, float x, float y, float radius);

  float x() const;
  float y() const;
  float radius() const;

  void Draw(ftxui::Canvas& c) const;

 private:
  b2BodyDef bodyDef;
  b2Body* body = nullptr;
  b2CircleShape dynamicBox;
  b2FixtureDef fixtureDef;
  ftxui::Color color;
};

using Ball = std::unique_ptr<BallBase>;

#endif  // LINE_RIDER_BALL_HPP
