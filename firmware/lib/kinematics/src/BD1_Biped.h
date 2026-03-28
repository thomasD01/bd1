#pragma once
#include "types.h"
#include "BD1_IK.h"

struct BipedState {
    LegAngles left_leg;
    LegAngles right_leg;
    Point3D left_knee_pos;
    Point3D right_knee_pos; 
};

class BD1_Biped {
public:
    BD1_Biped(float thigh_len, float calf_len, float pelvis_width);
    BipedState calculateStance(Point3D body_pos, Point3D l_foot_pos, Point3D r_foot_pos);

private:
    BD1_Kinematics leg_solver;
    float L1;
    float p_width;

    Point3D calcKneeGlobal(Point3D hip_global, LegAngles angles);
};