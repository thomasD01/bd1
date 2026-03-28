#include "BD1_IK.h"
#include <cmath>

BD1_Kinematics::BD1_Kinematics(float thigh_len, float calf_len) {
    L1 = thigh_len;
    L2 = calf_len;
}

LegAngles BD1_Kinematics::solveIK(float x, float y, float z) {
    LegAngles angles;

    angles.hip_yaw = std::atan2(y, -z);

    float D_squared = (x * x) + (z * z);
    float cos_knee = (D_squared - (L1 * L1) - (L2 * L2)) / (2.0f * L1 * L2);
    
    if (cos_knee > 1.0f) cos_knee = 1.0f;
    if (cos_knee < -1.0f) cos_knee = -1.0f;
    
    angles.knee_pitch = -(M_PI - std::acos(cos_knee)); 

    float alpha = std::atan2(x, -z);
    float beta = std::acos(((L1 * L1) + D_squared - (L2 * L2)) / (2.0f * L1 * std::sqrt(D_squared)));
    
    angles.hip_pitch = alpha - beta;

    return angles;
}