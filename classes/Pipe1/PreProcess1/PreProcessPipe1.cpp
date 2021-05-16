#include "PreProcessPipe1.hpp"
#include "../../../libs/libvision/PreProcess.h"
#include <opencv2/highgui.hpp>

PreProcessPipe1::PreProcessPipe1(){}

void PreProcessPipe1::execute(cv::Mat src, cv::Mat &dst) {

    cv::Mat tmp;
    cv::Mat lookUpTable(1, 256, CV_8U);

    uchar* p = lookUpTable.ptr();

    for (int i = 0; i < 256; ++i) {
        p[i] = cv::saturate_cast<uchar>(pow(i / 255.0, 5.5) * 255.0);
    }

    tmp = src.clone();
    cv::LUT(src, lookUpTable, tmp);

    cv::imshow("Temp", tmp);

    PreProcess::applyBlur(tmp, dst, 3, PreProcess::smoothType::MEDIAN);

    cv::cvtColor(dst, dst, cv::COLOR_BGR2HSV);
}