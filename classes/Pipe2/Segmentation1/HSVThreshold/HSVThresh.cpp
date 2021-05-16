#include "HSVThresh.hpp"
#include "../../../../libs/globals/Global.h"

HSVThresh::HSVThresh(){}

void HSVThresh::execute(cv::Mat src, cv::Mat &dst, cv::Scalar minRange, cv::Scalar maxRange) {

    cv::inRange(src, minRange, maxRange, dst);
}