#pragma once
#include <algorithm>

class Joint {
public:
  int motor_id;
  
  float zero_offset;
  float min_angle;
  float max_angle;

  float target_angle;
  float compliant_angle;
  float current_angle;
  int current_load;

  Joint(int id, float offset, float min_lim, float max_lim) {
    motor_id = id;
    zero_offset = offset;
    min_angle = min_lim;
    max_angle = max_lim;
    target_angle = 0.0;
    compliant_angle = 0.0;
    current_load = 0;
  }

  void setTarget(float raw_angle) {
    float calibrated_angle = raw_angle + zero_offset;
    
    target_angle = std::clamp(calibrated_angle, min_angle, max_angle);
  }

  void applyCompliance(float spring_constant, int load_threshold) {
    if (abs(current_load) > load_threshold) {
      float offset = (abs(current_load) - load_threshold) * spring_constant;
      if (current_load > 0) offset = -offset;
      compliant_angle = target_angle + offset;
    } else {
      compliant_angle = target_angle;
    }
  }
};