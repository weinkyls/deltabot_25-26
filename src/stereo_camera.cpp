#include "libcam2opencv.h"
#include <iostream>
#include <chrono>

#include "stereo_camera.h"

struct camera_callback : Libcam2OpenCV::Callback {
    virtual void hasFrame(const cv::Mat &frame, const libcamera::ControlList &) {
        if (!frame.empty()){
            cv::imshow("camera feed", frame);
            cv::waitKey(1);
        }
    }
};

int main(void) {
    Libcam2OpenCV camera;
    camera_callback myCallback;

    camera.registerCallback(&myCallback);
    camera.start();

    while(true){
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
    
    return 0;
}
