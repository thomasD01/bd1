#pragma once
#include "joint.h"
#include <BD1_Biped.h>

class MotorManager {
private:
  Joint* joints[6];
  size_t joint_count;

public:
  MotorManager() 
  {}

  void begin();
  void update(float dt);
  void addJoint(Joint* joint);
};