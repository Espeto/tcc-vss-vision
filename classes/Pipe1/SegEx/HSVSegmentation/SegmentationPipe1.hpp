#pragma once

#include "../../../../libs/interfaces/SegmentationInterface.hpp"

class SegmentationPipe1 : public SegmentationInterface
{
public:
    SegmentationPipe1();
    std::tuple<std::vector<std::vector<cv::Point>>, std::vector<std::vector<cv::Point>>> execute(cv::Mat preProcessedImg);

private:
    const int MIN_DIRECT_AREA = 190;
    const int MAX_DIRECT_AREA = 240;

    const int MIN_BALL_AREA = 200;
    const int MAX_BALL_AREA = 700;

    const int MIN_OBJECT_AREA = 600;
    const int MAX_OBEJCT_AREA = 20000;
};