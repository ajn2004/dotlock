#include "../include/simulation.hpp"
#include <iostream>
#include <random>
#include <cmath>

std::default_random_engine generator;
std::normal_distribution<double> gaussian(0.0, 1.0);

double norm(const Vec2& v) {
  return std::sqrt(v[0]*v[0] + v[1]*v[1]);
}

Vec2 add(const Vec2& a, const Vec2& b) {
  return {a[0] + b[0], a[1] + b[1]};
}

Vec2 subtract(const Vec2& a, const Vec2& b) {
  return {a[0] - b[0], a[1] - b[1]};
}

Vec2 scale(const Vec2& v, double scalar) {
  return {v[0] * scalar, v[1] * scalar};
}

Vec2 random_step(double stddev) {
  return {gaussian(generator) * stddev, gaussian(generator) * stddev};
}

void MicroscopeSim::print_state() const {
  std::cout << "Dot: (" << dot_pos[0] << ", " << dot_pos[1] << ")\n";
  std::cout << "Stage: (" << stage_pos[0] << ", " << stage_pos[1] << ")\n";
  std::cout << "Reward: " << reward() << "\n\n";
}
  // Simulate one step
void MicroscopeSim::step(const Vec2& action) {
  time += dt;
  // Update dot with Brownian motion
  dot_pos = add(dot_pos, random_step(dot_std));
  // add deterministic drift
  Vec2 det_drift = {cos(2*M_PI/frequency*time), sin(2*M_PI/frequency*time)};
  dot_pos = add(dot_pos, scale(det_drift, radius));

  // correct dot position
  dot_pos = add(dot_pos, scale(action, motor_speed));
}
  
// Compute reward (negative distance)
double MicroscopeSim::reward() const {
  return -norm(subtract(dot_pos, stage_pos));
}

void MicroscopeSim::reset(){
  *this = MicroscopeSim();
}
