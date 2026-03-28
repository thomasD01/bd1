#pragma once

#include "types.h"

class BD1_Kinematics {
public:
    BD1_Kinematics(float thigh_len, float calf_len);
    LegAngles solveIK(float x, float y, float z);

private:
    float L1;
    float L2;
};