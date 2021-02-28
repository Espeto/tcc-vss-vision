#include "helper.h"
#include <iostream>
#include <sstream>

void helpers::createImageFile(cv::Mat &image, double time, std::string fileName) {
    std::ostringstream fname;

    std::cout << "Vai Gravar Imagem" << std::endl;

    fname << "Images/outputs/" << fileName << time << "_.png";
    cv::imwrite(fname.str(), image);
}