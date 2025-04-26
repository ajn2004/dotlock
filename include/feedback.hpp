#include "simulation.hpp"
#include <deque>

struct PID{
  double kp = 10;
  double ki = 0.01;
  double kd = 0.5;
  Vec2 integral = {0.0, 0.0};
  double previous_error = 0.0;
  std::deque<Vec2> error_history;

  Vec2 compute(const Vec2& target, const Vec2& actual);
};

Vec2 basic_feedback(Vec2& target, Vec2& actual);
