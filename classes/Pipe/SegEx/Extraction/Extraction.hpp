#pragma once

#include "../../../../libs/interfaces/ExtractionInterface.hpp"

class Extraction: public ExtractionInterface {
    public:
        Extraction() = default;
        void execute(objectsContours contours); 

    private:
        void extractPlayer(std::vector<std::vector<std::vector<cv::Point>>> rolesContour, std::vector<std::vector<cv::Point>> teamContour);
        void extractBall(std::vector<cv::Point> ballContour);
        void extractEnemy(std::vector<std::vector<cv::Point>> enemyContours);
};