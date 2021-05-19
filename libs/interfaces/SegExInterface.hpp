#pragma once

#include <opencv2/core.hpp>

class SegExInterface {
    public:
    virtual void execute(cv::Mat preProcessedImg) = 0;
};