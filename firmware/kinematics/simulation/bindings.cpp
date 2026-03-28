#include <pybind11/pybind11.h>

#include "../src/BD1_Biped.h"
#include "../src/BD1_Gait.h"
#include "../src/BD1_IK.h"

namespace py = pybind11;

PYBIND11_MODULE(bd1_sim, m) {
    py::class_<LegAngles>(m, "LegAngles")
        .def_readwrite("hip_yaw", &LegAngles::hip_yaw)
        .def_readwrite("hip_pitch", &LegAngles::hip_pitch)
        .def_readwrite("knee_pitch", &LegAngles::knee_pitch);

    py::class_<Point3D>(m, "Point3D")
        .def(py::init<float, float, float>())
        .def_readwrite("x", &Point3D::x)
        .def_readwrite("y", &Point3D::y)
        .def_readwrite("z", &Point3D::z);

    py::class_<BipedState>(m, "BipedState")
        .def_readwrite("left_leg", &BipedState::left_leg)
        .def_readwrite("right_leg", &BipedState::right_leg)
        .def_readwrite("left_knee_pos", &BipedState::left_knee_pos)
        .def_readwrite("right_knee_pos", &BipedState::right_knee_pos);

    py::class_<BD1_Biped>(m, "BD1_Biped")
        .def(py::init<float, float, float>(), py::arg("thigh_len"), py::arg("calf_len"), py::arg("pelvis_width"))
        .def("calculateStance", &BD1_Biped::calculateStance);
    
    py::class_<GaitConfig>(m, "GaitConfig")
        .def_readwrite("step_length", &GaitConfig::step_length)
        .def_readwrite("clearance", &GaitConfig::clearance)
        .def_readwrite("resting_z", &GaitConfig::resting_z);

    py::class_<BD1_Gait>(m, "BD1_Gait")
        .def(py::init<float, float>(), py::arg("thigh_len"), py::arg("calf_len"))
        .def("getConfig", &BD1_Gait::getConfig)
        .def("calculateBezierStep", &BD1_Gait::calculateBezierStep);
}