cmake_minimum_required(VERSION 3.5)

if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    set(CMAKE_BUILD_TYPE "RelWithDebInfo")
endif()

project(RPLIDAR_A1_Rock5B LANGUAGES CXX)
include(GNUInstallDirs)
set(CMAKE_CXX_STANDARD 17)

find_package(Threads REQUIRED)
find_package(OpenCV REQUIRED)
find_package(HDF5 REQUIRED COMPONENTS C)

find_package(PkgConfig)

# no longer using libcamera
# pkg_check_modules(LIBCAMERA REQUIRED IMPORTED_TARGET libcamera)

include_directories(${CMAKE_SOURCE_DIR} ${OpenCV_INCLUDE_DIRS})

add_subdirectory(lib/opencv-camera-callback)
add_subdirectory(lib/c1lidar)
add_subdirectory(lib/wheelspeeds)

add_library(motor_library STATIC lib/wheelspeeds/Pwm.cpp)

add_library(epsilonbot SHARED 
  src/stereo_camera.cpp
  # src/lidar.cpp 
  # src/motor.cpp 
)

# first argument is what the exe will be called, the target
# Stereo Camera Demo
add_executable(camera_example examples/camera_example.cpp)
target_link_libraries(camera_example PRIVATE epsilonbot)

# LiDAR Demo
# add_executable(lidar_example examples/lidar_example.cpp)
# target_link_libraries(lidar_example PRIVATE epsilonbot)

# Motor Demo
# add_executable(motor_example examples/motor_example.cpp)
# target_link_libraries(motor_example PRIVATE epsilonbot)

target_link_libraries(epsilonbot PUBLIC
  cam2opencv 
  c1lidarrpi
  motor_library
  ${OpenCV_LIBS}
  ${HDF5_LIBRARIES}
  Threads::Threads
)

# this is for finding path for header files and non-compiled libraries
target_include_directories(epsilonbot PUBLIC
  ${OpenCV_INCLUDE_DIRS}
  ${CMAKE_CURRENT_LIST_DIR}/include
  ${CMAKE_CURRENT_LIST_DIR}/lib/opencv-camera-callback
  ${CMAKE_CURRENT_LIST_DIR}/lib/c1lidar
  ${CMAKE_CURRENT_LIST_DIR}/lib/wheelspeeds
)

set_target_properties(epsilonbot PROPERTIES
  POSITION_INDEPENDENT_CODE TRUE
)

# he was using target based cmake
# add_executable(printRPM printRPM.cpp)
# target_link_libraries(printRPM a1lidarrpi Threads::Threads)