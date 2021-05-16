#pragma once

#include <opencv2/core.hpp>

class ThresholdInterface {
    public:
    virtual void execute(cv::Mat src, cv::Mat &dst, cv::Scalar minRange, cv::Scalar maxRange)=0;
};