#pragma once

#include "../../../../libs/interfaces/ExtractionInterface.hpp"

class ExtractionPipe1: public ExtractionInterface {
    public:
        ExtractionPipe1();
        void execute(std::tuple<std::vector<std::vector<cv::Point>>, std::vector<std::vector<cv::Point>>> contours); 
};