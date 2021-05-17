#include "PreProcessPipe1.hpp"
#include "../../../libs/libvision/PreProcess.h"
#include <opencv2/highgui.hpp>
#include "../../../libs/helpers/helper.h"

PreProcessPipe1::PreProcessPipe1(){}

void PreProcessPipe1::execute(cv::Mat src, cv::Mat &dst) {

    cv::Mat tmp;

    PreProcess::gammaCorrection(src, tmp, 5.5);

    cv::imshow("Temp", tmp);

    PreProcess::applyBlur(tmp, dst, 3, PreProcess::smoothType::MEDIAN);

    cv::cvtColor(dst, dst, cv::COLOR_BGR2HSV);
}