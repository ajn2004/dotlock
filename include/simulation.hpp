#pragma once

#include <array>

using Vec2 = std::array<double, 2>;
double norm(const Vec2 &v);

Vec2 add(const Vec2 &a, const Vec2 &b);
Vec2 subtract(const Vec2& a, const Vec2& b);
Vec2 scale(const Vec2 &v, double scalar);
Vec2 random_step(double stddev);

struct MicroscopeSim {
  Vec2 dot_pos{0.0, 0.0};
  Vec2 stage_pos{0.0, 0.0};

  double dot_std = 0.2;
  double stage_drift_std = 0.5;
  double motor_speed = 0.1;
  
  void step(const Vec2& action);
  double reward() const;
  void print_state() const;
};
