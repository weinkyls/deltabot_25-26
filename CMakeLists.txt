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
  ${CMAKE_CURRENT_LIST_DIR}/lib/wheeleddrive 
  )

set(LIBSRC
  a1lidarrpi.cpp
  rplidarsdk/rplidar_driver.cpp
  rplidarsdk/arch/linux/net_socket.cpp
  rplidarsdk/arch/linux/timer.cpp
  rplidarsdk/arch/linux/net_serial.cpp
  rplidarsdk/hal/thread.cpp
)

add_library(a1lidarrpi STATIC ${LIBSRC})

set_target_properties(a1lidarrpi PROPERTIES
  POSITION_INDEPENDENT_CODE TRUE
  PUBLIC_HEADER a1lidarrpi.h)

target_link_libraries(a1lidarrpi Threads::Threads rt)

# first argument is what the exe will be called, the target
add_executable(stereo_camera src/stereo_camera.cpp)
target_link_libraries(stereo_camera PRIVATE cam2opencv PkgConfig::LIBCAMERA)

# he was using target based cmake
# add_executable(printRPM printRPM.cpp)
# target_link_libraries(printRPM a1lidarrpi Threads::Threads)