#include "PreProcess1Pipe2.hpp"
#include "../../../libs/libvision/PreProcess.h"

PreProcess1Pipe2::PreProcess1Pipe2(){}

void PreProcess1Pipe2::execute(cv::Mat src, cv::Mat &dst) {

    cv::Mat tmp;

    gammaCorrection(src, 2.5);

    PreProcess::applyBlur(src, tmp, 3, PreProcess::smoothType::GAUSSIAN);

    cv::cvtColor(tmp, dst, cv::COLOR_BGR2HSV);
}

void PreProcess1Pipe2::gammaCorrection(cv::Mat &img, const double gamma_) {

    cv::Mat lookUpTable(1, 256, CV_8U);
    uchar* p = lookUpTable.ptr();

    for (int i = 0; i < 256; ++i) {
        p[i] = cv::saturate_cast<uchar>(pow(i / 255.0, gamma_) * 255.0);
    }

    cv::Mat tmp = img.clone();
    cv::LUT(img, lookUpTable, tmp);
}