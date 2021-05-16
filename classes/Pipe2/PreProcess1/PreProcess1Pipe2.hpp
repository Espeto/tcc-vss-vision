#pragma once

#include "../../../libs/interfaces/PreProcessInterface.hpp"

class PreProcess1Pipe2: public PreProcessInterface {
    public:
        PreProcess1Pipe2();
        void execute(cv::Mat src, cv::Mat &dst);

    private:
        void gammaCorrection(cv::Mat &img, const double gamma_);
};