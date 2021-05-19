#pragma once

#include <opencv2/core.hpp>
#include <vector>

struct objectsContours {
    std::vector<std::vector<std::vector<cv::Point>>> rolesContour;
    std::vector<std::vector<cv::Point>> teamContours;
    std::vector<cv::Point> ballContour;
    std::vector<std::vector<cv::Point>> enemyContours;
};