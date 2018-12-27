cmake_minimum_required(VERSION 3.11)
project(N-Body)

find_package(CUDA 9.0 REQUIRED)

find_package(OpenMP)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${OpenMP_EXE_LINKER_FLAGS}")

# list(APPEND CUDA_NVCC_FLAGS "-dc")

# set(CUDA_NVCC_FLAGS "${CUDA_NVCC_FLAGS} -dc" )

## If you want to link SFML statically
# set(SFML_STATIC_LIBRARIES FALSE)

## In most cases better set in the CMake cache
# set(SFML_DIR "C:\Users\40203\Downloads\SFML-2.5.1-windows-vc15-64-bit\SFML-2.5.1\lib\cmake\SFML")

find_package(SFML 2.5.1 COMPONENTS graphics window system REQUIRED)
cuda_add_executable(CUDA-N-Body_simulator GPGPU/N-Body_Simulator.cpp GPGPU/Body.h GPGPU/Body.cpp GPGPU/Simulation.cu GPGPU/Simulation.h GPGPU/stdafx.cpp GPGPU/stdafx.h GPGPU/targetver.h)
add_executable(MULTI-THREAD-N-Body_simulator Multi-Threaded/N-Body_Simulator.cpp Multi-Threaded/Body.h Multi-Threaded/Body.cpp Multi-Threaded/Simulation.cpp Multi-Threaded/Simulation.h Multi-Threaded/stdafx.cpp Multi-Threaded/stdafx.h Multi-Threaded/targetver.h)
add_executable(SEQUENTIAL-N-Body_simulator sequential/N-Body_Simulator.cpp sequential/Body.h sequential/Body.cpp sequential/Simulation.cpp sequential/Simulation.h sequential/stdafx.cpp sequential/stdafx.h sequential/targetver.h)

target_link_libraries(CUDA-N-Body_simulator sfml-graphics sfml-window sfml-system)
target_link_libraries(MULTI-THREAD-N-Body_simulator sfml-graphics sfml-window sfml-system)
target_link_libraries(SEQUENTIAL-N-Body_simulator sfml-graphics sfml-window sfml-system)