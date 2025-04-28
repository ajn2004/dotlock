#include "../include/simulation.hpp"
#include <iostream>
#include <random>
#include <cmath>
#include <fstream>

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

// logging
void MicroscopeSim::log_step(const Vec2 &action) {
  trajectory_log.push_back({dot_pos, tar_pos, action, reward(), time});
}
void MicroscopeSim::write_log(const std::string& filename) {
  std::ofstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Failed to open log file!" << std::endl;
        return;
    }

    // Write header
    file << "time,dot_x,dot_y,stage_x,stage_y,action_x,action_y,reward\n";

    // Write each logged point
    for (const auto& point : trajectory_log) {
        file << point.time << ","
             << point.dot_pos[0] << "," << point.dot_pos[1] << ","
             << point.tar_pos[0] << "," << point.tar_pos[1] << ","
             << point.action[0] << "," << point.action[1] << ","
             << point.reward << "\n";
    }

    file.close();
    std::cout << "Wrote log with " << trajectory_log.size() << " points." << std::endl;

    // Optional: Clear log for next episode
    trajectory_log.clear();
}
// Compute reward (negative distance)
double MicroscopeSim::reward() const {
  return -norm(subtract(dot_pos, tar_pos));
}

void MicroscopeSim::reset(){
  *this = MicroscopeSim();
}
