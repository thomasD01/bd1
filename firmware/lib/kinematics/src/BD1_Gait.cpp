#include "BD1_Gait.h"
#include <cmath>

BD1_Gait::BD1_Gait(float thigh_len, float calf_len) {
    float total_leg = thigh_len + calf_len;
    
    config.step_length = total_leg * 0.40f; 
    config.clearance   = total_leg * 0.12f;
    config.resting_z   = -(total_leg * 0.85f);
}

GaitConfig BD1_Gait::getConfig() const {
    return config;
}

Point3D BD1_Gait::calculateBezierStep(float t, Point3D start, Point3D end) {
    if (t < 0.0f) t = 0.0f;
    if (t > 1.0f) t = 1.0f;

    Point3D p0 = start;
    
    // Pull clearance directly from our encapsulated config
    Point3D p1;
    p1.x = start.x + (end.x - start.x) * 0.2f; 
    p1.y = start.y + (end.y - start.y) * 0.2f;
    p1.z = start.z + (config.clearance * 1.5f); 

    Point3D p2;
    p2.x = start.x + (end.x - start.x) * 0.8f;
    p2.y = start.y + (end.y - start.y) * 0.8f;
    p2.z = end.z + (config.clearance * 1.5f);

    Point3D p3 = end;

    float u = 1.0f - t;
    float tt = t * t;
    float uu = u * u;
    float uuu = uu * u;
    float ttt = tt * t;

    Point3D current_pos;
    current_pos.x = uuu * p0.x + 3.0f * uu * t * p1.x + 3.0f * u * tt * p2.x + ttt * p3.x;
    current_pos.y = uuu * p0.y + 3.0f * uu * t * p1.y + 3.0f * u * tt * p2.y + ttt * p3.y;
    current_pos.z = uuu * p0.z + 3.0f * uu * t * p1.z + 3.0f * u * tt * p2.z + ttt * p3.z;

    return current_pos;
}