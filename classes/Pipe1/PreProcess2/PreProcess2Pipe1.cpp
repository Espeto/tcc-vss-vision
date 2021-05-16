#include "PreProcess2Pipe1.hpp"
#include "../../../libs/libvision/PreProcess.h"
#include <opencv2/highgui.hpp>

PreProcess2Pipe1::PreProcess2Pipe1(){}

void PreProcess2Pipe1::execute(cv::Mat src, cv::Mat &dst) {

    cv::Mat tmp;
    cv::Mat lookUpTable(1, 256, CV_8U);

    uchar* p = lookUpTable.ptr();

    for (int i = 0; i < 256; ++i) {
        p[i] = cv::saturate_cast<uchar>(pow(i / 255.0, 3.9) * 255.0);
    }

    PreProcess::applyBlur(src, dst, 3, PreProcess::smoothType::GAUSSIAN);

    tmp = dst.clone();
    cv::LUT(dst, lookUpTable, tmp);

    cv::imshow("Temp", tmp);

    cv::cvtColor(tmp, dst, cv::COLOR_BGR2HSV);
}