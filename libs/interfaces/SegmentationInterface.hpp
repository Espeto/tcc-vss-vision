#pragma once

#include <opencv2/core.hpp>
#include "../types/types.h"

class SegmentationInterface {
    public:
    virtual objectsContours execute(cv::Mat preProcessedImg) = 0;
};