#include <iostream>
#include <random>
#include <cmath>
#include <array>

using Vec2 = std::array<double, 2>;

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

struct MicroscopeSim {
  Vec2 dot_pos{0.0, 0.0};
  Vec2 stage_pos{0.0, 0.0};
  
  
  double dot_std = 0.2;
  double stage_drift_std = 0.5;
  double motor_speed = 0.1;
  
  // Simulate one step
  void step(const Vec2& action) {
    // Update dot with Brownian motion
    dot_pos = add(dot_pos, random_step(dot_std));
    
    // Update stage with control and its own drift
    Vec2 drift = random_step(stage_drift_std);
    Vec2 move = scale(action, motor_speed);
    stage_pos = add(stage_pos, add(drift, move));
  }
  
  // Compute reward (negative distance)
  double reward() const {
    return -norm(subtract(dot_pos, stage_pos));
  }
  
  void print_state() const {
    std::cout << "Dot: (" << dot_pos[0] << ", " << dot_pos[1] << ")\n";
    std::cout << "Stage: (" << stage_pos[0] << ", " << stage_pos[1] << ")\n";
    std::cout << "Reward: " << reward() << "\n\n";
  }
};

int main() {
  MicroscopeSim sim;
  Vec2 action{0.0, 0.0};
  
  for (int i = 0; i < 10; ++i) {
    sim.step(action);
    sim.print_state();
    
    // Example: introduce a control action every few steps
    if (i == 5) {
      action = {-1.0, 0.5};
    }
  }
  
  return 0;
}
