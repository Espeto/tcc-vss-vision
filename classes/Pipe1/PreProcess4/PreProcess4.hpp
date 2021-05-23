#pragma once

#include "../../../libs/interfaces/PreProcessInterface.hpp"

class PreProcess4: public PreProcessInterface {
    public:
        PreProcess4();
        void execute(cv::Mat src, cv::Mat &dst);
};
