#ifndef CAMERA_H
#define CAMERA_H

#include <cstdint>
#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>
#include <memory>
#include <vector>
#include <string>
#include <chrono>
#include <sstream>
#include <atomic>
#include <mutex>
#include <thread>

// public data types

struct frame_meta_data {
    // initialised at runtime
    uint64_t frame_id = 0;
    uint64_t monotonic_ns = 0;
};

struct frame {
    cv::Mat image;
    frame_meta_data meta;
};

struct frame_pair {
    frame cam0; // right (on rock5b+)
    frame cam1; // left
    int64_t delta_ns = 0;
};

struct camera_spec {
    int camera_count = 2;
    bool require_distinct_devices = true;
    std::vector<int> preferred_fps;
};

class camera_system {
    public:
    // constructor
    camera_system(const camera_spec& spec);

    bool initialise();
    void start();
    void stop();
    bool get_frame_pair(frame_pair& out);

    std::string last_error() const;
    std::string debug_summary() const;


    // destructor
    ~camera_system();

    private:
    camera_spec m_spec;

    // will be declared in camera.cpp
    class dual_camera_manager;
    std::unique_ptr<dual_camera_manager> m_dual;

    // store selected/pipeline info as strings
    std::string m_cam0_devnode;
    std::string m_cam1_devnode;

    bool m_initialised = false;
    bool m_running = false;
    std::string m_last_error;
};

#endif