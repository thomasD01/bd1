#include "BD1_Biped.h"
#include <cmath>

BD1_Biped::BD1_Biped(float thigh_len, float calf_len, float pelvis_width) 
    : leg_solver(thigh_len, calf_len), L1(thigh_len), p_width(pelvis_width) {}

Point3D BD1_Biped::calcKneeGlobal(Point3D hip_global, LegAngles angles) {
    float knee_x_rel = L1 * std::sin(angles.hip_pitch);
    float knee_z_planar = -L1 * std::cos(angles.hip_pitch);
    float knee_y_rel = knee_z_planar * std::sin(angles.hip_yaw);
    float knee_z_rel = knee_z_planar * std::cos(angles.hip_yaw);
    
    return {
        hip_global.x + knee_x_rel,
        hip_global.y + knee_y_rel,
        hip_global.z + knee_z_rel
    };
}

BipedState BD1_Biped::calculateStance(Point3D body_pos, Point3D l_foot_pos, Point3D r_foot_pos) {
    BipedState state;

    Point3D l_hip_global = { body_pos.x, body_pos.y + (p_width / 2.0f), body_pos.z };
    Point3D r_hip_global = { body_pos.x, body_pos.y - (p_width / 2.0f), body_pos.z };

    float l_target_x = l_foot_pos.x - l_hip_global.x;
    float l_target_y = l_foot_pos.y - l_hip_global.y;
    float l_target_z = l_foot_pos.z - l_hip_global.z;

    float r_target_x = r_foot_pos.x - r_hip_global.x;
    float r_target_y = r_foot_pos.y - r_hip_global.y;
    float r_target_z = r_foot_pos.z - r_hip_global.z;

    state.left_leg = leg_solver.solveIK(l_target_x, l_target_y, l_target_z);
    state.right_leg = leg_solver.solveIK(r_target_x, r_target_y, r_target_z);

    state.left_knee_pos = calcKneeGlobal(l_hip_global, state.left_leg);
    state.right_knee_pos = calcKneeGlobal(r_hip_global, state.right_leg);

    return state;
}