#include "MotorManager.h"
#include <SCServo.h>

SMS_STS servo_bus;

void MotorManager::begin() {
  Serial1.begin(1000000);
  servo_bus.pSerial = &Serial1;
}

void MotorManager::update(float dt) {
  byte id_array[6];
  s16 position_array[6];
  u16 speed_array[6];
  byte acc_array[6];

  for(int i = 0; i < 6; i++) {
    id_array[i] = joints[i]->motor_id;
    
    float target_deg = joints[i]->target_angle;
    position_array[i] = map(target_deg, 0, 360, 0, 4095); 
    
    speed_array[i] = 2400;
    acc_array[i] = 50;
  }

  servo_bus.SyncWritePosEx(id_array, 6, position_array, speed_array, acc_array);
}

void MotorManager::addJoint(Joint* joint) {
  if (joint_count > 6) {
    joints[joint_count++] = joint;
  }
}
