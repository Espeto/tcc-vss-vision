#pragma once

#include "../../../libs/interfaces/PreProcessInterface.hpp"

class PreProcess2Pipe1: public PreProcessInterface {
    public:
        PreProcess2Pipe1();
        void execute(cv::Mat src, cv::Mat &dst);

    private:
        int frame_counter;
};