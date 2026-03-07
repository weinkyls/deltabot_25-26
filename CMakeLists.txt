cmake_minimum_required(VERSION 3.1.0)

if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    set(CMAKE_BUILD_TYPE "RelWithDebInfo")
endif()

project(RPLIDAR_A1_Rock5B LANGUAGES CXX)
include(GNUInstallDirs)
set(CMAKE_CXX_STANDARD 11)

find_package(Threads REQUIRED)

# this is for finding path for header files and non-compiled libraries
target_include_directories(project_git PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}/include
  ${CMAKE_CURRENT_LIST_DIR}/lib/libcamera2opencv
  ${CMAKE_CURRENT_LIST_DIR}/lib/c1lidar
  ${CMAKE_CURRENT_LIST_DIR}/lib/wheelspeeds
)

add_library(motors STATIC lib/wheelspeeds/Pwm.cpp)
add_library(c1lidar STATIC lib/c1lidar/c1lidarrpi.cpp)
add_library(camera_callback STATIC lib/libcamera2opencv/libcamera2opencv.cpp)

set_target_properties(stereo_camera PROPERTIES
  POSITION_INDEPENDENT_CODE TRUE
  )

# first argument is what the exe will be called, the target
add_executable(stereo_camera src/stereo_camera.cpp)
target_link_libraries(stereo_camera PRIVATE cam2opencv PkgConfig::LIBCAMERA camera_callback)

# he was using target based cmake
# add_executable(printRPM printRPM.cpp)
# target_link_libraries(printRPM a1lidarrpi Threads::Threads)