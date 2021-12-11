#pragma once

#include "../../../libs/interfaces/PreProcessInterface.hpp"

class PreProcess : public PreProcessInterface
{
public:
    PreProcess() = default;
    void execute(cv::Mat src, cv::Mat &dst);
};
