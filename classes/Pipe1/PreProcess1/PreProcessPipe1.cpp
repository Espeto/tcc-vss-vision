#include "PreProcessPipe1.hpp"
#include "../../../libs/libvision/PreProcess.h"
#include <opencv2/highgui.hpp>
#include "../../../libs/helpers/helper.h"

PreProcessPipe1::PreProcessPipe1(){}

void PreProcessPipe1::execute(cv::Mat src, cv::Mat &dst) {

    cv::Mat tmp;

    PreProcess::gammaCorrection(src, tmp, 6.0);

    cv::imshow("Temp1", tmp);

    PreProcess::applyBlur(tmp, dst, 7, PreProcess::smoothType::GAUSSIAN);

    cv::imshow("Temp2", dst);

    cv::cvtColor(dst, dst, cv::COLOR_BGR2HSV);
}