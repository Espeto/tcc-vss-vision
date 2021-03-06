#include "Segex.hpp"
#include <opencv2/highgui.hpp>
#include <iostream>

Segex::Segex(SegmentationInterface *segment, ExtractionInterface *extract)
{
    this->_segmentation = segment;
    this->_extraction = extract;
};

void Segex::execute(cv::Mat preProcessedImg)
{
    objectsContours contours;
    //double t;

    //t = (double)cv::getTickCount();
    contours = this->_segmentation->execute(preProcessedImg);
    //t = ((double)cv::getTickCount() - t) / cv::getTickFrequency();

    //std::cout << "Segmentation time: " << t << std::endl;

    //t = (double)cv::getTickCount();
    this->_extraction->execute(contours);
    //t = ((double)cv::getTickCount() - t) / cv::getTickFrequency();

    //std::cout << "Extraction time: " << t << std::endl;
}