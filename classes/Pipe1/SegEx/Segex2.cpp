#include "Segex2.hpp"
#include <opencv2/highgui.hpp>
#include <iostream>


Segex2::Segex2(SegmentationInterface *segment, ExtractionInterface *extract)
{
    this->_segmentation = segment;
    this->_extraction = extract;

    this->_seg2 = (Segmentation2*) this->_segmentation;

    this->frame_counter = 1;
};

void Segex2::execute(cv::Mat preProcessedImg)
{
    objectsContours contours;
    double t;

    t = (double)cv::getTickCount();
    contours = this->_seg2->execute(preProcessedImg);
    t = ((double)cv::getTickCount() - t) / cv::getTickFrequency();

    std::cout << "Reduce seg time[" << frame_counter++ << "]: " << t << std::endl;

    if (!this->_seg2->isAllFound())
    {
        t = (double)cv::getTickCount();
        contours = this->auxSegmentation.execute(preProcessedImg);
        t = ((double)cv::getTickCount() - t) / cv::getTickFrequency();

        std::cout << "Segmentation time: " << t << std::endl;
    }
    

    t = (double)cv::getTickCount();
    this->_extraction->execute(contours);
    t = ((double)cv::getTickCount() - t) / cv::getTickFrequency();

    std::cout << "Extraction time: " << t << std::endl;
}