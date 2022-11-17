#pragma once

#include <opencv2/core/mat.hpp>

class IMedianFilter {
public:
    virtual cv::Mat apply() = 0;
    virtual ~IMedianFilter() = default;
};