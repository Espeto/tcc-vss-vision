#pragma once

#include <opencv2/core.hpp>

class PosProcessInterface {
    public:
    virtual void execute(cv::Mat src, cv::Mat &dst) = 0;
};