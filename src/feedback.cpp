#include "../include/feedback.hpp"
#include <cstdio>

Vec2 PID::compute(const Vec2& target, const Vec2& actual){
  {
    Vec2 error = subtract(target, actual);

    // Proportional component
    Vec2 P = scale(error, kp);
    
    // Integral component
    integral = add(integral, error);
    Vec2 I = scale(integral, ki);
    
    // Derivative component
    Vec2 D = {0.0, 0.0};
    if (!error_history.empty()){
      Vec2 previous = error_history.back();
      Vec2 delta_error = subtract(error, previous);
      D = scale(delta_error, kd);
    };

    error_history.push_back(error);
    if (error_history.size()>30){ error_history.pop_front();};
    return scale(add(P, add(I, D)), 1);
  }
}

void PID::reset(){
  PID new_pid;
  kp = new_pid.kp;
  ki = new_pid.ki;
  kd = new_pid.kd;
  integral = new_pid.integral;
  error_history = new_pid.error_history;
}

Vec2 basic_feedback(Vec2 &target, Vec2 &actual) {
  Vec2 error = subtract(target, actual);
  Vec2 correction = scale(error, 1);
  return correction;
}
