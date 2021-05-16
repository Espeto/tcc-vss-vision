#pragma once

#include <opencv2/core.hpp>
#include <vector>

class ContoursInterface {
    public:
    virtual std::tuple<std::vector<std::vector<cv::Point>>, std::vector<std::vector<cv::Point>>> execute(cv::Mat img)=0;
};