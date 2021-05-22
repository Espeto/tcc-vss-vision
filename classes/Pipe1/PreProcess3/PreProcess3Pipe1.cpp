#include "PreProcess3Pipe1.hpp"
#include "../../../libs/libvision/PreProcess.h"
#include <opencv2/highgui.hpp>
#include "../../../libs/helpers/helper.h"

PreProcess3Pipe1::PreProcess3Pipe1(){}

void PreProcess3Pipe1::execute(cv::Mat src, cv::Mat &dst) {

    cv::Mat tmp;
    float alpha = -1.0;
    int beta = -150;

    src.convertTo(tmp, -1, alpha, beta);

    cv::imshow("Temp1", tmp);

    PreProcess::applyBlur(tmp, dst, 5, PreProcess::smoothType::GAUSSIAN);

    cv::imshow("Temp2", dst);

    cv::cvtColor(dst, dst, cv::COLOR_BGR2HSV);
}