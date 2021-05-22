#pragma once

#include "../../../libs/interfaces/PreProcessInterface.hpp"

class PreProcess3Pipe1: public PreProcessInterface {
    public:
        PreProcess3Pipe1();
        void execute(cv::Mat src, cv::Mat &dst);
};
