#include <BD1_Biped.h>
#include <BD1_Gait.h>
#include "MotorManager.h"
#include <Arduino_RouterBridge.h>

// --- System Configuration ---
const float L1 = 120.0;
const float L2 = 110.0;
const float PELVIS_WIDTH = 90.0;
const float STEP_DURATION = 0.5;

// --- Brain & Muscle Objects ---
BD1_Biped biped(L1, L2, PELVIS_WIDTH);
BD1_Gait gait(L1, L2);
GaitConfig config;
MotorManager motor_manager;

// --- Joint Definitions ---
Joint l_knee_joint(1, 0.0f, -90.0f, 90.0f);
Joint l_hip_pitch_joint(2, 0.0f, -90.0f, 90.0f);
Joint l_hip_yaw_joint(3, 0.0f, -90.0f, 90.0f);
Joint r_knee_joint(4, 0.0f, -90.0f, 90.0f);
Joint r_hip_pitch_joint(5, 0.0f, -90.0f, 90.0f);
Joint r_hip_yaw_joint(6, 0.0f, -90.0f, 90.0f);

// --- State Variables ---
float current_t = 0.0;
unsigned long last_time = 0;

void setup() {
    Bridge.begin();
    Monitor.begin();
    
    // Register Hardware
    motor_manager.addJoint(&l_hip_yaw_joint);
    motor_manager.addJoint(&l_hip_pitch_joint);
    motor_manager.addJoint(&l_knee_joint);
    motor_manager.addJoint(&r_hip_yaw_joint);
    motor_manager.addJoint(&r_hip_pitch_joint);
    motor_manager.addJoint(&r_knee_joint);
    motor_manager.begin();

    Monitor.println("🤖 BD-1 Continuous Gait Engine Booting...");
    config = gait.getConfig();
    last_time = micros();
}

// =========================================================
// 🧠 THE BRAIN
// =========================================================
void calculateKinematics() {
    float D = config.step_length;
    int step_index = (int)current_t;
    float local_t = current_t - step_index;
    
    // 1. Calculate Body Position
    Point3D body = {
        current_t * D, 
        sin(current_t * PI) * (PELVIS_WIDTH / 2.0f), 
        abs(config.resting_z)
    };
    
    // 2. Calculate Foot Positions
    Point3D l_foot, r_foot;
    bool is_right_swing = (step_index % 2 == 0);
    
    if (is_right_swing) {
        l_foot = {step_index * D, PELVIS_WIDTH / 2.0f, 0.0f};
        r_foot = gait.calculateBezierStep(
            local_t, 
            {max(0.0f, (step_index - 1) * D), -(PELVIS_WIDTH / 2.0f), 0.0f}, 
            {(step_index + 1) * D, -(PELVIS_WIDTH / 2.0f), 0.0f}
        );
    } else {
        r_foot = {step_index * D, -(PELVIS_WIDTH / 2.0f), 0.0f};
        l_foot = gait.calculateBezierStep(
            local_t, 
            {max(0.0f, (step_index - 1) * D), PELVIS_WIDTH / 2.0f, 0.0f}, 
            {(step_index + 1) * D, PELVIS_WIDTH / 2.0f, 0.0f}
        );
    }
    
    // 3. Solve IK
    BipedState state = biped.calculateStance(body, l_foot, r_foot);

    // 4. Update Joint Targets
    l_hip_yaw_joint.setTarget(state.left_leg.hip_yaw);
    l_hip_pitch_joint.setTarget(state.left_leg.hip_pitch);
    l_knee_joint.setTarget(state.left_leg.knee_pitch);

    r_hip_yaw_joint.setTarget(state.right_leg.hip_yaw);
    r_hip_pitch_joint.setTarget(state.right_leg.hip_pitch);
    r_knee_joint.setTarget(state.right_leg.knee_pitch);
}

// =========================================================
// ⚙️ THE MAIN LOOP
// =========================================================
void loop() {
    // 1. Check the Clock
    unsigned long current_time = micros();
    float dt = (current_time - last_time) / 1000000.0f; 
    last_time = current_time;

    // 2. Do the Math
    calculateKinematics();

    // 3. Move the Metal
    motor_manager.update(dt);
    
    // 4. Advance the Phase safely based on real time
    current_t += (dt / STEP_DURATION);
    if (current_t >= 4.0) current_t = 0.0; 
}