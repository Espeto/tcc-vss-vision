#include "PreProcessPipe1.hpp"
#include "../../../libs/libvision/PreProcess.h"

PreProcessPipe1::PreProcessPipe1(){}

void PreProcessPipe1::execute(cv::Mat src, cv::Mat &dst) {

    cv::Mat tmp;

    PreProcess::applyBlur(src, tmp, 3, PreProcess::smoothType::GAUSSIAN);

    cv::cvtColor(tmp, dst, cv::COLOR_BGR2HSV);
}