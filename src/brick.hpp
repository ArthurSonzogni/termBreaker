#ifndef LINE_RIDER_BRICK_HPP
#define LINE_RIDER_BRICK_HPP

#include <box2d/box2d.h>
#include <ftxui/dom/canvas.hpp>
#include <ftxui/screen/color.hpp>
#include <memory>

class BrickBase {
 public:
  BrickBase(b2World& world, float x, float y, int half_width, int half_height);
  ~BrickBase();

  void OnContact();
  void Step();
  void Draw(ftxui::Canvas& c) const;
  int counter() const { return counter_; }

 private:
  b2World& world_;
  int counter_ = 10;
  int half_width_;
  int half_height_;
  uint8_t color_hue_ = 0;
  uint8_t color_value_ = 0;
  b2BodyDef bodyDef;
  b2Body* body = nullptr;
  b2PolygonShape dynamicBox;
  b2FixtureDef fixtureDef;
};

using Brick = std::unique_ptr<BrickBase>;

#endif  // LINE_RIDER_BRICK_HPP
