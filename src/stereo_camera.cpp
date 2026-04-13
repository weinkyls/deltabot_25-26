#include "stereo_camera.h"
#include <iostream>

// constructor
StereoSystem::StereoSystem() {
    // using lambda expression to connect left camera obj to leftCallback, in this case
    leftCamera.registerFrameCallback([this](const cv::Mat &frame) {
        this->leftCallback(frame);
    });
    
    rightCamera.registerFrameCallback([this](const cv::Mat &frame) {
        this->rightCallback(frame);
    });
}

// destructor
StereoSystem::~StereoSystem() {
    stop();
}

bool StereoSystem::start() {
    // configure left Camera (/dev/video0 and subdev2)
    OpenCVparameters paramsLeft;
    paramsLeft.deviceID = 0;
    paramsLeft.fourcc = cv::VideoWriter::fourcc('G', 'R', 'E', 'Y');
    paramsLeft.framerate = 10;

    std::vector<V4LParameter> v4lLeft = {
        {"/dev/v4l-subdev2", V4L2_CID_GAIN, 0.5},
        {"/dev/v4l-subdev2", V4L2_CID_HFLIP, 1},
        {"/dev/v4l-subdev2", V4L2_CID_VFLIP, 1}
    };

    // configure right Camera (/dev/video11 and subdev7)
    OpenCVparameters paramsRight;
    paramsRight.deviceID = 11;
    paramsRight.fourcc = cv::VideoWriter::fourcc('G', 'R', 'E', 'Y');
    paramsRight.framerate = 10;

    std::vector<V4LParameter> v4lRight = {
        {"/dev/v4l-subdev7", V4L2_CID_GAIN, 0.5},
        {"/dev/v4l-subdev7", V4L2_CID_HFLIP, 1},
        {"/dev/v4l-subdev7", V4L2_CID_VFLIP, 1}
    };

    // Start both hardware streams
    std::cout << "initialising cameras" << std::endl;
    leftCamera.start(paramsLeft, v4lLeft);
    rightCamera.start(paramsRight, v4lRight);

    return true;
}

void StereoSystem::stop() {
    leftCamera.stop();
    rightCamera.stop();
    std::cout << "cameras stopped" << std::endl;
}

// internal callback for the left camera thread
void StereoSystem::leftCallback(const cv::Mat &frame) {
    // prevent conflict between camera thread and main thread
    std::lock_guard<std::mutex> lock(frameMutex);
    // copy frame since v4l2 will overwrite the buffer
    currentLeftFrame = frame.clone(); 
}

void StereoSystem::rightCallback(const cv::Mat &frame) {
    std::lock_guard<std::mutex> lock(frameMutex);
    currentRightFrame = frame.clone();
}

bool StereoSystem::getLatestFrames(cv::Mat &leftOut, cv::Mat &rightOut) {
    std::lock_guard<std::mutex> lock(frameMutex);
    
    // check if the cameras have delivered a frame yet
    if (currentLeftFrame.empty() || currentRightFrame.empty()) {
        return false; 
    }

    currentLeftFrame.copyTo(leftOut);
    currentRightFrame.copyTo(rightOut);
    
    return true;
}
