#pragma once

#include <string>
#include <opencv2/imgcodecs.hpp>

namespace helpers {
    void createImageFile(cv::Mat &image, double time, std::string fileName);
}