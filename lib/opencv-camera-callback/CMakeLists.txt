cmake_minimum_required(VERSION 3.25)

if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  set(CMAKE_BUILD_TYPE "RelWithDebInfo")
endif()

project(opencv-camera-callback VERSION 2.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set (CMAKE_CXX_FLAGS "-Wall -pedantic")

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt6 COMPONENTS Widgets REQUIRED)
find_package(Threads REQUIRED)
find_package(OpenCV REQUIRED)
include_directories( ${OpenCV_INCLUDE_DIRS} )

add_executable(camera-viewer
  window.cpp
  camera-viewer.cpp
  camera.cpp
)

target_link_libraries(camera-viewer Qt6::Widgets)
target_link_libraries(camera-viewer ${OpenCV_LIBS} Threads::Threads)

install(TARGETS camera-viewer)

# creating static library for epsilonbot
add_library(opencv_callback STATIC camera.cpp window.cpp)
set_target_properties(opencv_callback PROPERTIES POSITION_INDEPENDENT_CODE ON)
# libraries needed for opencv_callback
target_link_libraries(opencv_callback PUBLIC 
${OpenCV_LIBS} 
Threads::Threads
Qt6::Widgets
)
# make public
target_include_directories(opencv_callback PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
