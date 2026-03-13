#ifndef STEREO_CAMERA_H
#define STEREO_CAMERA_H
#include "libcam2opencv.h"
#include <opencv2/opencv.hpp>

struct camera_callback : Libcam2OpenCV::Callback {
    virtual void hasFrame(const cv::Mat &frame, const libcamera::ControlList &) {
        if (!frame.empty()){
            cv::imshow("camera feed", frame);
            cv::waitKey(1);
        }
    }
};
#endif