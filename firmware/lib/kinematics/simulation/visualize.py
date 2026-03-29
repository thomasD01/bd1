import matplotlib
matplotlib.use('Qt5Agg')

import bd1_sim
import numpy as np
import math
import matplotlib.pyplot as plt
from matplotlib.widgets import Slider

L1 = 120.0  
L2 = 110.0  
pelvis_width = 90.0

biped_controller = bd1_sim.BD1_Biped(L1, L2, pelvis_width)
gait_engine = bd1_sim.BD1_Gait(L1, L2)
gait_params = gait_engine.getConfig()
D = gait_params.step_length

fig = plt.figure(figsize=(10, 8))
ax = fig.add_subplot(111, projection='3d')
plt.subplots_adjust(left=0.15, bottom=0.25)

ax.set_xlim([-50, 400])
ax.set_ylim([-100, 100])
ax.set_zlim([0, 220])
ax.set_title('BD-1 Continuous Gait Engine')

xx, yy = np.meshgrid(range(-50, 451, 50), range(-100, 101, 50))
ax.plot_wireframe(xx, yy, np.zeros_like(xx), color='gray', alpha=0.3)

line_L, = ax.plot([], [], [], 'o-', color='cyan', linewidth=5, markersize=8)
line_R, = ax.plot([], [], [], 'o-', color='orange', linewidth=5, markersize=8)
line_pelvis, = ax.plot([], [], [], '-', color='black', linewidth=8)

ax_bz = plt.axes([0.15, 0.15, 0.65, 0.03])
ax_t  = plt.axes([0.15, 0.10, 0.65, 0.03])

slider_bz = Slider(ax_bz, 'Body Z', 50.0, 220.0, valinit=abs(gait_params.resting_z))
slider_t  = Slider(ax_t,  'Time (t)', 0.0, 4.0, valinit=0.0)

def update(val):
    t = slider_t.val
    step_index = int(math.floor(t))
    local_t = t - step_index
    
    body_x = t * D
    body_y = math.sin(t * math.pi) * (pelvis_width / 2.0) 
    body_z = slider_bz.val
    
    body = bd1_sim.Point3D(body_x, body_y, body_z)
    
    is_right_swing = (step_index % 2 == 0)
    
    if is_right_swing:
        l_x = step_index * D
        l_foot = bd1_sim.Point3D(l_x, pelvis_width / 2.0, 0.0)
        
        r_start_x = max(0.0, (step_index - 1) * D)
        r_end_x = (step_index + 1) * D
        
        start_pt = bd1_sim.Point3D(r_start_x, -(pelvis_width/2.0), 0.0)
        end_pt = bd1_sim.Point3D(r_end_x, -(pelvis_width/2.0), 0.0)
        r_foot = gait_engine.calculateBezierStep(local_t, start_pt, end_pt)
        
    else:
        r_x = step_index * D
        r_foot = bd1_sim.Point3D(r_x, -(pelvis_width/2.0), 0.0)
        
        l_start_x = max(0.0, (step_index - 1) * D)
        l_end_x = (step_index + 1) * D
        
        start_pt = bd1_sim.Point3D(l_start_x, pelvis_width/2.0, 0.0)
        end_pt = bd1_sim.Point3D(l_end_x, pelvis_width/2.0, 0.0)
        l_foot = gait_engine.calculateBezierStep(local_t, start_pt, end_pt)
    
    state = biped_controller.calculateStance(body, l_foot, r_foot)
    
    l_hip_y = body.y + (pelvis_width / 2.0)
    r_hip_y = body.y - (pelvis_width / 2.0)
    
    line_pelvis.set_data(np.array([body.x, body.x]), np.array([l_hip_y, r_hip_y]))
    line_pelvis.set_3d_properties(np.array([body.z, body.z]))
    
    line_L.set_data(np.array([body.x, state.left_knee_pos.x, l_foot.x]), 
                    np.array([l_hip_y, state.left_knee_pos.y, l_foot.y]))
    line_L.set_3d_properties(np.array([body.z, state.left_knee_pos.z, l_foot.z]))
    
    line_R.set_data(np.array([body.x, state.right_knee_pos.x, r_foot.x]), 
                    np.array([r_hip_y, state.right_knee_pos.y, r_foot.y]))
    line_R.set_3d_properties(np.array([body.z, state.right_knee_pos.z, r_foot.z]))
    
    fig.canvas.draw_idle()

slider_t.on_changed(update)
slider_bz.on_changed(update)
update(0)
plt.show()