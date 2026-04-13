#include "stereo_camera.h"
#include <iostream>
#include <chrono>
#include <thread>

int main() {
    std::cout << "stereo camera test" << std::endl;

    StereoSystem stereo;

    if (!stereo.start()) {
        std::cerr << "failed to start cameras" << std::endl;
        return -1;
    }

    cv::Mat leftFrame, rightFrame;

    std::cout << "press 'esc' to exit the video feed" << std::endl;

    while (true) {
        if (stereo.getLatestFrames(leftFrame, rightFrame)) {
            cv::imshow("left camera preview", leftFrame);
            cv::imshow("right camera preview", rightFrame);
        }

        // if escape key is pressed
        if (cv::waitKey(1) == 27) {
            break;
        }
    }
    return 0;
}