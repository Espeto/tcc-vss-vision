#include "SegexPipe1.hpp"

SegexPipe1::SegexPipe1(SegmentationInterface *segment, ExtractionInterface *extract)
{
    this->_segmentation = segment;
    this->_extraction = extract;
};

void SegexPipe1::execute(cv::Mat preProcessedImg)
{

    std::tuple<std::vector<std::vector<cv::Point>>, std::vector<std::vector<cv::Point>>> contours;

    contours = this->_segmentation->execute(preProcessedImg);

    this->_extraction->execute(contours);
}