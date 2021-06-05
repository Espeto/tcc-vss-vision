#pragma once

#include "../../../libs/interfaces/SegExInterface.hpp"
#include "../../../libs/interfaces/SegmentationInterface.hpp"
#include "../../../libs/interfaces/ExtractionInterface.hpp"

#include "./HSSegmentation/SegmentationPipe1.hpp"
#include "./HSSegReduced/Segmentation2.hpp"

class Segex2: public SegExInterface {
    public:
        Segex2(SegmentationInterface* segment, ExtractionInterface *extract);
        void execute(cv::Mat preProcessedImg);

    private:
        SegmentationInterface* _segmentation;
        ExtractionInterface* _extraction;

        Segmentation2* _seg2;

        SegmentationPipe1 auxSegmentation;

        int frame_counter;

};