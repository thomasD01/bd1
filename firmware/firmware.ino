#include <BD1_Biped.h>
#include <BD1_Gait.h>
#include <Arduino_RouterBridge.h>

const float L1 = 120.0;
const float L2 = 110.0;
const float PELVIS_WIDTH = 90.0;

BD1_Biped biped(L1, L2, PELVIS_WIDTH);
BD1_Gait gait(L1, L2);
GaitConfig config;

float current_t = 0.0;
const float TIME_STEP = 0.02; // Slower, smoother steps for the log
int loop_counter = 0; 

void setup() {
    Bridge.begin();
    Monitor.begin();
    
    pinMode(LED_BUILTIN, OUTPUT);

    Monitor.println("🤖 BD-1 Continuous Gait Engine Booting...");
    config = gait.getConfig();
}

void loop() {
    // --- Heartbeat ---
    if (loop_counter % 25 == 0) {
        digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
    }
    loop_counter++;

    // --- Phase Variables ---
    float D = config.step_length;
    int step_index = (int)current_t; // 0, 1, 2, 3...
    float local_t = current_t - step_index; // 0.0 to 1.0 phase of current step
    
    // 1. Body Math (Forward motion + Sine wave sway)
    float body_x = current_t * D; 
    float body_y = sin(current_t * PI) * (PELVIS_WIDTH / 2.0f); 
    float body_z = abs(config.resting_z); 
    
    Point3D body = {body_x, body_y, body_z};
    
    // 2. Foot Math (State Machine) 
    Point3D l_foot, r_foot;
    bool is_right_swing = (step_index % 2 == 0);
    
    if (is_right_swing) {
        // Right foot swings via Bezier, Left foot is planted
        float l_x = step_index * D;
        l_foot = {l_x, PELVIS_WIDTH / 2.0f, 0.0f};
        
        float r_start_x = max(0.0f, (step_index - 1) * D);
        float r_end_x = (step_index + 1) * D;
        
        Point3D start_pt = {r_start_x, -(PELVIS_WIDTH / 2.0f), 0.0f};
        Point3D end_pt = {r_end_x, -(PELVIS_WIDTH / 2.0f), 0.0f};
        r_foot = gait.calculateBezierStep(local_t, start_pt, end_pt);
        
    } else {
        // Left foot swings via Bezier, Right foot is planted
        float r_x = step_index * D;
        r_foot = {r_x, -(PELVIS_WIDTH / 2.0f), 0.0f};
        
        float l_start_x = max(0.0f, (step_index - 1) * D);
        float l_end_x = (step_index + 1) * D;
        
        Point3D start_pt = {l_start_x, PELVIS_WIDTH / 2.0f, 0.0f};
        Point3D end_pt = {l_end_x, PELVIS_WIDTH / 2.0f, 0.0f};
        l_foot = gait.calculateBezierStep(local_t, start_pt, end_pt);
    }
    
    // 3. Solve Inverse Kinematics
    BipedState state = biped.calculateStance(body, l_foot, r_foot);
    
    // 4. Output to Monitor
    Monitor.print("Step: "); Monitor.print(step_index);
    Monitor.print(" t: "); Monitor.print(current_t, 2);
    Monitor.print(" L_Knee: "); Monitor.print(state.left_leg.knee_pitch * (180.0 / PI), 1);
    Monitor.print(" L_Hip: "); Monitor.print(state.left_leg.hip_pitch * (180.0 / PI), 1);
    Monitor.print(" R_Knee: "); Monitor.print(state.right_leg.knee_pitch * (180.0 / PI), 1);
    Monitor.print(" R_Hip: "); Monitor.println(state.right_leg.hip_pitch * (180.0 / PI), 1);
    
    // Advance time and loop every 4 steps
    current_t += TIME_STEP;
    if (current_t >= 4.0) current_t = 0.0; 
    
    delay(20); // 50Hz control loop
}