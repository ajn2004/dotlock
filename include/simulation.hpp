#pragma once

#include <array>

using Vec2 = std::array<double, 2>;
double norm(const Vec2 &v);

// bool use_feedback = false;
// bool use_pid = true;

Vec2 add(const Vec2 &a, const Vec2 &b);
Vec2 subtract(const Vec2& a, const Vec2& b);
Vec2 scale(const Vec2 &v, double scalar);
Vec2 random_step(double stddev);
Vec2 drift_wind(const double A, const double w);

struct MicroscopeSim {
  Vec2 dot_pos{0.0, 0.0};
  Vec2 stage_pos{0.0, 0.0};

  double dot_std = 0.2;
  double stage_drift_std = 0.5;
  double motor_speed = 0.1;
  double time = 0.0;
  double radius = 0;
  double frequency = 1;
  double dt = 0.016;
  
  void step(const Vec2& action);
  double reward() const;
  void print_state() const;
  void reset();
};
