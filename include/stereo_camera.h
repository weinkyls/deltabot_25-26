#ifndef STEREO_CAMERA_H
#define STEREO_CAMERA_H

#include <opencv2/opencv.hpp>
#include <mutex>
#include "camera.h"

class StereoSystem {
public:
    StereoSystem();
    ~StereoSystem();

    // initialise V4L2 nodes and starts both capture threads
    bool start();

    void stop();
 
    // returns false if frames aren't ready yet.
    bool getLatestFrames(cv::Mat &leftOut, cv::Mat &rightOut);

private:
    // camera class from camera.h
    Camera leftCamera;
    Camera rightCamera;

    // internal buffers to hold the data coming from the hardware
    cv::Mat currentLeftFrame;
    cv::Mat currentRightFrame;

    // mutex used since resources are shared between threads
    std::mutex frameMutex;

    // internal callback
    void leftCallback(const cv::Mat &frame);
    void rightCallback(const cv::Mat &frame);
};

#endif