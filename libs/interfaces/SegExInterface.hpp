#pragma once

#include <opencv2/core.hpp>
#include <tuple>
#include <vector>

class SegExInterface {
    public:
    virtual void execute(cv::Mat preProcessedImg) = 0;
};