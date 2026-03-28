#pragma once
#include "BD1_Biped.h"

struct GaitConfig {
    float step_length;
    float clearance;
    float resting_z;
};

class BD1_Gait {
public:
    BD1_Gait(float thigh_len, float calf_len);

    GaitConfig getConfig() const;

    Point3D calculateBezierStep(float t, Point3D start, Point3D end);

private:
    GaitConfig config;
};