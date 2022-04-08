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

class Board {
 public:
  Board();

  bool OnEvent(ftxui::Event event);
  void Step();
  void Draw(ftxui::Canvas& c) const;

 private:
  b2World world_;
  ContactListener contact_listener_;

  std::vector<Ball> balls_;
  std::vector<Brick> bricks_;
};

#endif  // BOARD_HPP
