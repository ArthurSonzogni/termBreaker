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
#include <functional>
#include <vector>

#include "ball.hpp"
#include "brick.hpp"
#include "contact_listener.hpp"

namespace term_breaker {

struct BoardConfig {
  int balls = 0;

  bool buy_item_1 = false;
  bool buy_item_2 = false;
  bool buy_item_3 = false;
  bool buy_item_4 = false;
  bool buy_item_5 = false;
  bool buy_item_6 = false;
  bool buy_item_7 = false;
  bool buy_item_8 = false;
};

class Board {
 public:
  explicit Board(BoardConfig config,
                 std::function<void()> win,
                 std::function<void()> lose,
                 std::function<void()> quit);

  bool OnEvent(ftxui::Event event);
  void Step();
  ftxui::Element Draw() const;

 private:
  void InitializeBricks();
  void DrawShootingLine(ftxui::Canvas& c) const;
  static b2Vec2 ShootPosition();
  b2Vec2 ShootSpeed() const;
  void MoveUp();
  void MoveBricks();

  const BoardConfig config_;

  std::function<void()> win_;
  std::function<void()> lose_;
  std::function<void()> quit_;

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

} // namespace term_breaker

#endif  // BOARD_HPP

// Copyright 2022 Arthur Sonzogni. All rights reserved.
// Use of this source code is governed by the MIT license that can be found in
// the LICENSE file.
