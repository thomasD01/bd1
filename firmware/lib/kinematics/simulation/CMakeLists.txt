cmake_minimum_required(VERSION 3.12)
project(bd1_kinematics_simulation)

find_package(Python3 COMPONENTS Interpreter Development REQUIRED)

execute_process(
    COMMAND ${Python3_EXECUTABLE} -m pybind11 --cmakedir
    OUTPUT_VARIABLE pybind11_DIR
    OUTPUT_STRIP_TRAILING_WHITESPACE
)

find_package(pybind11 REQUIRED)

pybind11_add_module(bd1_sim bindings.cpp ../src/BD1_IK.cpp ../src/BD1_Gait.cpp ../src/BD1_Biped.cpp)
