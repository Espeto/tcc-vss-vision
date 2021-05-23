#include "PreProcess4.hpp"
#include "../../../libs/libvision/PreProcess.h"
#include <opencv2/highgui.hpp>
#include "../../../libs/helpers/helper.h"

PreProcess4::PreProcess4(){}

void PreProcess4::execute(cv::Mat src, cv::Mat &dst) {

    cv::cvtColor(src, dst, cv::COLOR_BGR2HSV);
}