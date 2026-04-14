#include <QApplication>
#include <QWidget>
#include <QShortcut>
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
    // shortcut to escape the program
    QShortcut *esc = new QShortcut(QKeySequence(Qt::Key_Escape), &bigWindow);
    QObject::connect(esc, &QShortcut::activated, [&bigWindow](){
        bigWindow.close();
    });

    // create left and right windows
    Window *leftWindow = new Window();
    Window *rightWindow = new Window();

    // add left and right window to big window
    bigLayout->addWidget(leftWindow);
    bigLayout->addWidget(rightWindow);

    bigWindow.show();

    while (bigWindow.isVisible()) {
        if (stereo.getLatestFrames(leftFrame, rightFrame)) {
            // use arrow bc leftWindow is a pointer
            leftWindow->updateImage(leftFrame);
            rightWindow->updateImage(rightFrame);
        }

        app.processEvents();
        // sleep to avoid tight polling loop
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
    stereo.stop();
    return 0;
}