#include "PreProcess2Pipe1.hpp"
#include "../../../libs/libvision/PreProcess.h"
#include "../../../libs/helpers/helper.h"
#include <opencv2/highgui.hpp>

PreProcess2Pipe1::PreProcess2Pipe1(){
    this->frame_counter = 1;
}

void PreProcess2Pipe1::execute(cv::Mat src, cv::Mat &dst) {

    cv::Mat tmp;

    PreProcess::applyBlur(src, dst, 5, PreProcess::smoothType::AVERAGE);

    cv::imshow("Temp1", dst);

    // helpers::createImageFile(dst, frame_counter, "preproc/blurred/frame");

    PreProcess::gammaCorrection(dst, tmp, 6.0);

    cv::imshow("Temp2", tmp);

    // helpers::createImageFile(tmp, frame_counter, "preproc/gamma/frame");

    cv::cvtColor(tmp, dst, cv::COLOR_BGR2HSV);

    // helpers::createImageFile(dst, frame_counter, "preproc/converted/frame");

    this->frame_counter++;
}