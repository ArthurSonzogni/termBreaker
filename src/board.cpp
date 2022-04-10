#include "board.hpp"
#include <ftxui/dom/elements.hpp>
#include <random>
#include "resources.hpp"

namespace {

constexpr int g_board_width = 150;
constexpr int g_board_height = 150;

// Beind the screen, there is a dead area where there are no brick allowed to
// stay. This avoid collisions with balls out of the screen.
bool BrickInDeadArea(const Brick& brick) {
  const int threshold_up = -4;
  const int threshold_down = 80;
  return brick->Up() >= g_board_height + threshold_up &&
         brick->Up() <= g_board_height + threshold_down;
}

struct CollisionCallback : public b2QueryCallback {
  bool ReportFixture(b2Fixture* /*fixture*/) override {
    collided = true;
    return false;  // Terminate the query.
  }
  bool collided = false;
};

b2Vec2 Gravity() {
  const float gravity = 140.f;
  return b2Vec2(0.F, gravity);
}

}  // namespace

Board::Board(BoardConfig config) : config_(config), world_(Gravity()) {
  world_.SetContactListener(&contact_listener_);

  InitializeBricks();
  term_breaker::PlayBackgroundMusic();
}

void Board::InitializeBricks() {
  std::random_device rd;
  std::mt19937 mt(rd());

  using uniform = std::uniform_int_distribution<int>;
  using exponential = std::exponential_distribution<float>;
  auto x_dist = uniform(0, g_board_width / 2);  // NOLINT
  auto y_dist =
      uniform(g_board_height * 2 / 4, 3 * g_board_height / 4);  // NOLINT
  auto half_width_dist = uniform(2, 10);                        // NOLINT
  auto half_height_dist = uniform(1, 4);                        // NOLINT
  auto counter_distribution = exponential(1.F / 5.F);           // NOLINT

  const int max_iterations = 100000;
  for (int i = 0; i < max_iterations; ++i) {
    int x = x_dist(mt) * 2;                      // NOLINT
    int y = y_dist(mt) * 4;                      // NOLINT
    int half_width = half_width_dist(mt) * 2;    // NOLINT
    int half_height = half_height_dist(mt) * 4;  // NOLINT
    int counter = 1 + static_cast<int>(counter_distribution(mt));

    b2AABB aabb;
    aabb.lowerBound.x = static_cast<float>(x - half_width - 1);
    aabb.lowerBound.y = static_cast<float>(y - half_height - 1);
    aabb.upperBound.x = static_cast<float>(x + half_width + 1);
    aabb.upperBound.y = static_cast<float>(y + half_height + 1);

    if (aabb.lowerBound.x < 0)
      continue;
    if (aabb.upperBound.x > g_board_width)
      continue;

    CollisionCallback callback;
    world_.QueryAABB(&callback, aabb);
    if (callback.collided) {
      continue;
    }

    bricks_.push_back(std::make_unique<BrickBase>(world_, x, y, half_width,
                                                  half_height, counter));
    const size_t bricks = 100;
    if (bricks_.size() >= bricks) {
      break;
    }
  }

  bricks_.erase(std::remove_if(bricks_.begin(), bricks_.end(), BrickInDeadArea),
                bricks_.end());
}

bool Board::OnEvent(ftxui::Event event) {
  if (!event.is_mouse()) {
    return false;
  }

  mouse_x_ = (event.mouse().x - 1) * 2;
  mouse_y_ = (event.mouse().y - 1) * 4;

  // Ignore the first few events, to avoid miss click.
  if (step_ < 30)
    return false;

  if (is_shooting_) {
    return false;
  }

  if (event.mouse().motion != ftxui::Mouse::Released) {
    return false;
  }

  is_shooting_ = true;
  remaining_balls_to_shoot_ = config_.balls;
  shooting_direction_ = ShootSpeed();
  return true;
}

void Board::Step() {
  // Evolve the worlds using Box2D.
  step_++;
  float timeStep = 1.0f / 60.0f;  // NOLINT
  int32 velocityIterations = 6;   // NOLINT
  int32 positionIterations = 2;   // NOLINT
  world_.Step(timeStep, velocityIterations, positionIterations);

  for (auto& brick : bricks_) {
    brick->Step();
  }

  // Move bricks that are close to the bottom of the screen higher.
  for (auto& brick : bricks_) {
    while (BrickInDeadArea(brick))
      brick->MoveUp();
  }

  // Erase dead bricks.
  auto counter_null = [](const Brick& brick) { return brick->counter() == 0; };
  bricks_.erase(std::remove_if(bricks_.begin(), bricks_.end(), counter_null),
                bricks_.end());

  // Shoot a sequence of balls.
  const int shoot_steps = 10;
  if (is_shooting_ && step_ % shoot_steps == 0 &&
      remaining_balls_to_shoot_ >= 1) {
    remaining_balls_to_shoot_--;
    const float radius = 4.F;
    balls_.push_back(std::make_unique<BallBase>(world_, ShootPosition(),
                                                shooting_direction_, radius));
  }

  // Erase out of screen balls.
  auto ball_out_of_screen = [](const Ball& ball) {
    return ball->x() < -10.F ||                 // NOLINT
           ball->x() > 160.F ||                 // NOLINT
           ball->y() >= g_board_height + 10.f;  // NOLINT
  };
  balls_.erase(std::remove_if(balls_.begin(), balls_.end(), ball_out_of_screen),
               balls_.end());

  // Allow user to shoot again after the previous shoot completed.
  if (is_shooting_ && remaining_balls_to_shoot_ == 0 && balls_.empty()) {
    is_shooting_ = false;
    MoveUp();
  }

  MoveBricks();
}

void Board::MoveBricks() {
  if (is_shooting_)
    return;

  // Require some bricks to appear on the screen:
  int min_y = g_board_height * 2;
  for (const auto& brick : bricks_) {
    min_y = std::min(min_y, brick->Up());
  }
  if (min_y > g_board_height * 1 / 2) {  // NOLINT
    MoveUp();
    return;
  }
}

void Board::MoveUp() {
  for (auto& brick : bricks_) {
    brick->MoveUp();
  }
}

ftxui::Element Board::Draw() const {
  using namespace ftxui;
  auto c = Canvas(g_board_width, g_board_height);

  for (const Ball& ball : balls_) {
    ball->Draw(c);
  }
  for (const Brick& brick : bricks_) {
    brick->Draw(c);
  }

  DrawShootingLine(c);

  std::string balls_remaining = std::to_string(remaining_balls_to_shoot_) +
                                "/" + std::to_string(config_.balls);

  auto frame = hbox({
      canvas(std::move(c)),
      separator(),
      vbox({
          window(text("bricks:"), text(std::to_string(bricks_.size()))),
          window(text("balls:"), text(balls_remaining)),
      }),
  });

  frame |= border;
  frame = vbox(frame, filler());
  frame = hbox(frame, filler());
  return frame;
}

void Board::DrawShootingLine(ftxui::Canvas& c) const {
  if (is_shooting_) {
    return;
  }

  b2Vec2 position = ShootPosition();
  b2Vec2 speed = ShootSpeed();
  float timeStep = 1.0f / 60.0f;  // NOLINT

  const float ball_friction = 0.5F;
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
    c.DrawPointLine(
        static_cast<int>(start.x),     //
        static_cast<int>(start.y),     //
        static_cast<int>(position.x),  //
        static_cast<int>(position.y),  //
        // NOLINTNEXTLINE
        ftxui::Color::HSV(static_cast<uint8_t>(color_value), 255, 128));
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

// Copyright 2022 Arthur Sonzogni. All rights reserved.
// Use of this source code is governed by the MIT license that can be found in
// the LICENSE file.
