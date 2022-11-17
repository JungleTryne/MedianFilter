#pragma once

#include "IMedianFilter.h"

class OpenCVMedianFilter : public IMedianFilter {
public:
    explicit OpenCVMedianFilter(cv::Mat image, int radius);
    cv::Mat apply() override;

private:
    cv::Mat image_;
    int radius_;
};
