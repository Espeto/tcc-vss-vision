#pragma once

#include <opencv2/core.hpp>
#include "../types/types.h"

class ExtractionInterface {
    public:
    virtual void execute(objectsContours contours) = 0;
};