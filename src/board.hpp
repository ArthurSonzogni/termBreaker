#ifndef BOARD_HPP
#define BOARD_HPP

#include <box2d/box2d.h>
#include <algorithm>
#include <cmath>
#include <ftxui/component/component.hpp>
#include <ftxui/component/event.hpp>
#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/dom/canvas.hpp>
#include <ftxui/dom/elements.hpp>
#include <ftxui/screen/color.hpp>
#include <ftxui/screen/screen.hpp>
#include <vector>

#include "ball.hpp"
#include "brick.hpp"
#include "contact_listener.hpp"

struct BoardConfig {
  int balls = 0;
};

class Board {
 public:
  explicit Board(BoardConfig config);

  bool OnEvent(ftxui::Event event);
  void Step();
  void Draw(ftxui::Canvas& c) const;

 private:
  void InitializeBricks();
  void DrawShootingLine(ftxui::Canvas& c) const;
  static b2Vec2 ShootPosition();
  b2Vec2 ShootSpeed() const;
  void MoveUp();
  void MoveBricks();

  const BoardConfig config_;

  b2World world_;
  ContactListener contact_listener_;

  std::vector<Ball> balls_;
  std::vector<Brick> bricks_;

  int mouse_x_ = 0;
  int mouse_y_ = 0;
  int step_ = 0;

  bool is_shooting_ = false;
  int remaining_balls_to_shoot_ = 0;
  b2Vec2 shooting_direction_;
};

#endif  // BOARD_HPP
