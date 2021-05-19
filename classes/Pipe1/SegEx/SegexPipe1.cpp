#include "SegexPipe1.hpp"
#include <opencv2/highgui.hpp>

SegexPipe1::SegexPipe1(SegmentationInterface *segment, ExtractionInterface *extract)
{
    this->_segmentation = segment;
    this->_extraction = extract;
};

void SegexPipe1::execute(cv::Mat preProcessedImg)
{
    objectsContours contours;

    contours = this->_segmentation->execute(preProcessedImg);

    this->_extraction->execute(contours);
}