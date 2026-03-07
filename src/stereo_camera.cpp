#include "stereo_camera.h"
#include <iostream>

struct camera_callback : Libcam2OpenCV::Callback {
    virtual void hasFrame(const cv::Mat &frame, const libcamera:ControlList &) {
        if (nullptr != window) {
            window->updateImage(frame);
        }
    }
};