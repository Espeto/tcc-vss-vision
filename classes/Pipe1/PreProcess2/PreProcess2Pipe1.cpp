#include "PreProcess2Pipe1.hpp"
#include "../../../libs/libvision/PreProcess.h"
#include "../../../libs/helpers/helper.h"
#include <opencv2/highgui.hpp>

PreProcess2Pipe1::PreProcess2Pipe1(){
    this->frame_counter = 1;
}

void PreProcess2Pipe1::execute(cv::Mat src, cv::Mat &dst) {

    cv::Mat tmp;

    PreProcess::applyBlur(src, dst, 3, PreProcess::smoothType::GAUSSIAN);

    // helpers::createImageFile(dst, frame_counter, "preproc/blurred/frame");

    PreProcess::gammaCorrection(dst, tmp, 3.9);

    cv::imshow("Temp", dst);

    // helpers::createImageFile(tmp, frame_counter, "preproc/gamma/frame");

    cv::cvtColor(tmp, dst, cv::COLOR_BGR2HSV);

    // helpers::createImageFile(dst, frame_counter, "preproc/converted/frame");

    this->frame_counter++;
}