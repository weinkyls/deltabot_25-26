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

pkg_check_modules(LIBCAMERA REQUIRED IMPORTED_TARGET libcamera)

include_directories(${CMAKE_SOURCE_DIR} ${LIBCAMERA_INCLUDE_DIRS} ${OpenCV_INCLUDE_DIRS})

add_subdirectory(lib/libcamera2opencv)
add_subdirectory(lib/c1lidar)

# first argument is what the exe will be called, the target
add_executable(stereo_camera 
src/stereo_camera.cpp
)

add_library(motor_library STATIC lib/wheelspeeds/Pwm.cpp)

target_link_libraries(stereo_camera PRIVATE 
cam2opencv 
c1lidarrpi
PkgConfig::LIBCAMERA 
motor_library
${OpenCV_LIBS}
${HDF5_LIBRARIES}
Threads::Threads
)

# this is for finding path for header files and non-compiled libraries
target_include_directories(stereo_camera PRIVATE
  ${OpenCV_INCLUDE_DIRS}
  ${CMAKE_CURRENT_LIST_DIR}/include
  ${CMAKE_CURRENT_LIST_DIR}/lib/libcamera2opencv
  ${CMAKE_CURRENT_LIST_DIR}/lib/c1lidar
  ${CMAKE_CURRENT_LIST_DIR}/lib/wheelspeeds
)

set_target_properties(stereo_camera PROPERTIES
  POSITION_INDEPENDENT_CODE TRUE
  )


# he was using target based cmake
# add_executable(printRPM printRPM.cpp)
# target_link_libraries(printRPM a1lidarrpi Threads::Threads)