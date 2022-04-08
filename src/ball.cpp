#include "ball.hpp"

BallBase::BallBase(b2World& world, float x, float y, float radius) {
  bodyDef.type = b2_dynamicBody;
  bodyDef.position.Set(x, y);          // NOLINT
  bodyDef.linearVelocity.Set(50, 50);  // NOLINT
  body = world.CreateBody(&bodyDef);
  dynamicBox.m_radius = radius;
  fixtureDef.shape = &dynamicBox;
  fixtureDef.density = 1.0f;       // NOLINT
  fixtureDef.friction = 1.0f;      // NOLINT
  fixtureDef.restitution = 0.97F;  // NOLINT
  body->CreateFixture(&fixtureDef);

  static uint8_t hue = 0;
  hue += 50;                                 // NOLINT
  color = ftxui::Color::HSV(hue, 200, 200);  // NOLINT;
}

float BallBase::x() const {
  return body->GetPosition().x;
}

float BallBase::y() const {
  return body->GetPosition().y;
}

float BallBase::radius() const {
  return dynamicBox.m_radius;
}

void BallBase::Draw(ftxui::Canvas& c) const {
  float angle = body->GetAngle();
  c.DrawPointCircle(static_cast<int>(x()), static_cast<int>(y()),
                    static_cast<int>(radius()), color);
  c.DrawPointLine(static_cast<int>(x() + radius() * cos(angle)),
                  static_cast<int>(y() + radius() * sin(angle)),
                  static_cast<int>(x()), static_cast<int>(y()), color);
}
