#include "libcam2opencv.h"
#include <iostream>
#include <chrono>

#include "stereo_camera.h"

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
