#pragma once

#include <opencv2/core.hpp>
#include <tuple>
#include <vector>

class ExtractionInterface {
    public:
    virtual void execute(std::tuple<std::vector<std::vector<std::vector<cv::Point>>>, std::vector<std::vector<cv::Point>>> contours) = 0;
};