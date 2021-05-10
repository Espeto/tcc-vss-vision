#pragma once

#include <string>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>

namespace helpers {
    void createImageFile(cv::Mat &image, int i, std::string fileName);
    void createImageFile(cv::Mat &image, double time, std::string fileName);
    void drawObject(int x, int y, cv::Scalar color, cv::Mat &dst);
}