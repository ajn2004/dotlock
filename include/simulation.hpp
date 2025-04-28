#pragma once

#include <array>
#include <vector>
#include <string>

using Vec2 = std::array<double, 2>;
double norm(const Vec2 &v);

// bool use_feedback = false;
// bool use_pid = true;

Vec2 add(const Vec2 &a, const Vec2 &b);
Vec2 subtract(const Vec2& a, const Vec2& b);
Vec2 scale(const Vec2 &v, double scalar);
Vec2 random_step(double stddev);
Vec2 drift_wind(const double A, const double w);

struct TrajectoryPoint {
  Vec2 dot_pos;
  Vec2 tar_pos;
  Vec2 action;
  double reward;
  double time;
};

struct MicroscopeSim {
  Vec2 dot_pos{0.0, 0.0};
  Vec2 tar_pos{0.0, 0.0};
  double dot_std = 0.2;
  double motor_speed = 0.1;
  double time = 0.0;
  double radius = 0;
  double frequency = 1;
  double dt = 0.016;
  std::vector<TrajectoryPoint> trajectory_log;
  
  void step(const Vec2& action);
  void log_step(const Vec2& action);
  void write_log(const std::string& filename = "trajectory_log.txt");
  double reward() const;
  void print_state() const;
  void reset();
};
