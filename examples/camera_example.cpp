#include <QApplication>
#include <QWidget>
// #include <QHBoxLayout> // in window.h
#include <iostream>
#include <thread>
#include <chrono>
#include "window.h"

#include "stereo_camera.h"

int main(int argc, char *argv[]) {
    // so i don;t have to run QT_QPA_PLATFORM=linuxfb ./camera_example 
    qputenv("QT_QPA_PLATFORM", "linuxfb");
    // start qt
    QApplication app(argc, argv);

    std::cout << "stereo camera test" << std::endl;

    StereoSystem stereo;

    if (!stereo.start()) {
        std::cerr << "failed to start cameras" << std::endl;
        return -1;
    }

    cv::Mat leftFrame, rightFrame;

    // big window to show left and right camera feeds
    QWidget bigWindow;
    QHBoxLayout *bigLayout = new QHBoxLayout(&bigWindow);

    // create left and right windows
    Window *leftWindow = new Window();
    Window *rightWindow = new Window();

    // add left and right window to big window
    bigLayout->addWidget(leftWindow);
    bigLayout->addWidget(rightWindow);

    bigWindow.show();

    while (true) {
        if (stereo.getLatestFrames(leftFrame, rightFrame)) {
            // use arrow bc leftWindow is a pointer
            leftWindow->updateImage(leftFrame);
            rightWindow->updateImage(rightFrame);
        }

        app.processEvents();

        // sleep to avoid tight polling loop
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
    return 0;
}