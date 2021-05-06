#pragma once

#include <opencv2/core.hpp>

class PreProcessInterface {
    public:
    virtual void execute(cv::Mat src, cv::Mat &dst){};
};