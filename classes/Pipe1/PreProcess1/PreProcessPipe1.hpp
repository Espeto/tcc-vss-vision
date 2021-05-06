#pragma once

#include "../../../libs/interfaces/PreProcessInterface.hpp"

class PreProcessPipe1: public PreProcessInterface {
    public:
        PreProcessPipe1();
        void execute(cv::Mat src, cv::Mat &dst);
};
