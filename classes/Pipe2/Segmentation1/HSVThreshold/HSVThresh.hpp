#pragma once

#include "../../../../libs/interfaces/ThresholdInterface.hpp"

class HSVThresh: public ThresholdInterface {
    public:
        HSVThresh();
        void execute(cv::Mat src, cv::Mat &dst, cv::Scalar minRange, cv::Scalar maxRange);
};