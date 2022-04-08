#include "board.hpp"
#include <ftxui/dom/elements.hpp>

b2Vec2 Gravity() {
  return b2Vec2(0.F, 140.F);  // NOLINT
}
Board::Board() : world_(Gravity()) {
  world_.SetContactListener(&contact_listener_);

  for (int y = 50; y < 150; y += 16) {    // NOLINT
    for (int x = 10; x < 150; x += 16) {  // NOLINT
      bricks_.push_back(
          std::make_unique<BrickBase>(world_, x, y, 5, 5));  // NOLINT
    }
  }
}

// NOLINTNEXTLINE
bool Board::OnEvent(ftxui::Event event) {
  if (event.is_mouse() && event.mouse().motion == ftxui::Mouse::Released) {
    const int mouse_x = (event.mouse().x - 1) * 2;
    const int mouse_y = (event.mouse().y - 1) * 4;
    balls_.push_back(std::make_unique<BallBase>(world_, 75, 0, mouse_x, mouse_y,
                                                3.F));  // NOLINT
    return true;
  };
  return false;
}

void Board::Step() {
  // Prepare for simulation. Typically we use a time step of 1/60 of a
  // second (60Hz) and 10 iterations. This provides a high quality
  // simulation in most game scenarios.
  float timeStep = 1.0f / 60.0f;  // NOLINT
  int32 velocityIterations = 6;   // NOLINT
  int32 positionIterations = 2;   // NOLINT
  // Instruct the world_ to perform a single step of simulation.
  // It is generally best to keep the time step and iterations fixed.
  world_.Step(timeStep, velocityIterations, positionIterations);

  for (auto& brick : bricks_) {
    brick->Step();
  }

  auto counter_null = [](const std::unique_ptr<BrickBase>& brick) {
    return brick->counter() == 0;
  };
  bricks_.erase(std::remove_if(bricks_.begin(), bricks_.end(), counter_null),
                bricks_.end());
}

void Board::Draw(ftxui::Canvas& c) const {
  for (const Ball& ball : balls_) {
    ball->Draw(c);
  }
  for (const Brick& brick : bricks_) {
    brick->Draw(c);
  }
}
