#include "stereo_camera.h"
#include <iostream>
#include <chrono>
#include <thread>

int main() {
    std::cout << "[Demo] Starting Epsilonbot Stereo Vision Test..." << std::endl;

    StereoSystem stereo;

    if (!stereo.start()) {
        std::cerr << "[Demo] Failed to start cameras. Check your device nodes!" << std::endl;
        return -1;
    }

    cv::Mat leftFrame, rightFrame;
    int framesGrabbed = 0;

    auto startTime = std::chrono::steady_clock::now();
    while (std::chrono::steady_clock::now() - startTime < std::chrono::seconds(5)) {
        
        if (stereo.getLatestFrames(leftFrame, rightFrame)) {
            framesGrabbed++;
            std::cout << "Grabbed pair #" << framesGrabbed 
                      << " | Left: " << leftFrame.cols << "x" << leftFrame.rows 
                      << " | Right: " << rightFrame.cols << "x" << rightFrame.rows 
                      << "\r" << std::flush; // Overwrites the terminal line
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(30));
    }

    std::cout << "\n[Demo] Test complete. Grabbed " << framesGrabbed << " frame pairs in 5 seconds." << std::endl;

    std::cout << "Press 'ESC' to exit the video feed..." << std::endl;

    while (true) {
        if (stereo.getLatestFrames(leftFrame, rightFrame)) {
            // 1. Draw the windows
            cv::imshow("Left Camera Preview", leftFrame);
            cv::imshow("Right Camera Preview", rightFrame);
        }

        if (cv::waitKey(1) == 27) {
            break;
        }
    }
    
    return 0;
}