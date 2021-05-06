#include "helper.h"
#include <iostream>
#include <sstream>

void helpers::createImageFile(cv::Mat &image, double time, std::string fileName) {
    std::ostringstream fname;

    fname << "../Images/outputs/" << fileName << time << "_.png";
    cv::imwrite(fname.str(), image);
}

void helpers::drawObject(int x, int y, cv::Scalar color, cv::Mat &dst)
{
    cv::circle(dst, cv::Point(x, y), 5, color, 2);

    cv::putText(dst, std::to_string(x) + "," + std::to_string(y), cv::Point(x,y), 1, 1, color, 2);
}