cmake_minimum_required(VERSION 3.25)

if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  set(CMAKE_BUILD_TYPE "RelWithDebInfo")
endif()

project(MetaDataPrinter
	DESCRIPTION "Prints the metadata of every frame on the screen"
	LANGUAGES CXX)

set (CMAKE_CXX_STANDARD 17)

set (CMAKE_CXX_FLAGS "-Wall -Winvalid-pch -Wnon-virtual-dtor -Wextra -Wno-unused-parameter")

add_executable(printdata
  printdata.cpp
)

include_directories(${CMAKE_SOURCE_DIR} ${CMAKE_SOURCE_DIR}/.. ${LIBCAMERA_INCLUDE_DIRS} ${OpenCV_INCLUDE_DIRS})

target_include_directories(printdata PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/..)
include(GNUInstallDirs)

target_link_libraries(printdata PkgConfig::LIBCAMERA)
target_link_libraries(printdata ${OpenCV_LIBS})
target_link_libraries(printdata cam2opencv)
