#pragma once

#include "../../../libs/interfaces/SegExInterface.hpp"
#include "../../../libs/interfaces/SegmentationInterface.hpp"
#include "../../../libs/interfaces/ExtractionInterface.hpp"

class Segex: public SegExInterface {
    public:
        Segex(SegmentationInterface* segment, ExtractionInterface *extract);
        void execute(cv::Mat preProcessedImg);

    private:
        SegmentationInterface* _segmentation;
        ExtractionInterface* _extraction;

};