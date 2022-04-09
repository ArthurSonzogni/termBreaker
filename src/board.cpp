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
  if (event.is_mouse()) {
    mouse_x_ = (event.mouse().x - 1) * 2;
    mouse_y_ = (event.mouse().y - 1) * 4;
    if (event.mouse().motion == ftxui::Mouse::Released) {
      const float radius = 3.F;
      balls_.push_back(std::make_unique<BallBase>(world_, ShootPosition(),
                                                  ShootSpeed(), radius));
      return true;
    }
  }
  return false;
}

void Board::Step() {
  step_ ++;
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

  //bool draw = true;
  b2Vec2 position = ShootPosition();
  b2Vec2 speed = ShootSpeed();
  float timeStep = 1.0f / 60.0f;  // NOLINT

  const float ball_friction = 0.45F;
  const float friction = std::pow(ball_friction, timeStep);

  const int iterations = 50;
  const int iterations_inner = 2;
  for (int i = 0; i < iterations; i++) {
    b2Vec2 start = position;
    for (int j = 0; j < iterations_inner; ++j) {
      b2Vec2 position_increment = speed;
      b2Vec2 speed_increment = Gravity();

      position_increment *= timeStep;
      speed_increment *= timeStep * friction;

      position += position_increment;
      speed += speed_increment;

      speed *= friction;
    }

    const int color_value = -10 * step_ + 15 * i;  // NOLINT
    //draw = !draw;
    //if (draw) {
    c.DrawPointLine(static_cast<int>(start.x),     //
                    static_cast<int>(start.y),     //
                    static_cast<int>(position.x),  //
                    static_cast<int>(position.y),  //
                    ftxui::Color::HSV(0, 0, static_cast<uint8_t>(color_value)));
    //}
  }
}

// static
b2Vec2 Board::ShootPosition() {
  const b2Vec2 position(75.F, 0.F);
  return position;
}

b2Vec2 Board::ShootSpeed() const {
  const b2Vec2 position = ShootPosition();
  const auto mouse_x = static_cast<float>(mouse_x_);
  const auto mouse_y = static_cast<float>(mouse_y_);
  const b2Vec2 target(mouse_x, mouse_y);
  b2Vec2 speed = target - position;
  speed.Normalize();
  const float speed_norm = 100.F;
  speed *= speed_norm;
  return speed;
}
