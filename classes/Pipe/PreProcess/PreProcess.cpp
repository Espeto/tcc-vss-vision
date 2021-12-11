#include "PreProcess.hpp"
#include "../../../libs/libvision/ImgProcess.h"
#include <opencv2/highgui.hpp>
#include "../../../libs/helpers/helper.h"

void PreProcess::execute(cv::Mat src, cv::Mat &dst) {

    ImgProcess::applyBlur(src, dst, 5, ImgProcess::smoothType::GAUSSIAN);

    cv::cvtColor(dst, dst, cv::COLOR_BGR2HSV);
}