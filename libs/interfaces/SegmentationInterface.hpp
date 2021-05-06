#pragma once

#include <opencv2/core.hpp>
#include <tuple>
#include <vector>

class SegmentationInterface {
    public:
    virtual std::tuple<std::vector<std::vector<cv::Point>>, std::vector<std::vector<cv::Point>>> execute(cv::Mat preProcessedImg) = 0;
};