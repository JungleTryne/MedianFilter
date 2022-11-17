#include <opencv2/opencv.hpp>
#include "IMedianFilter.h"

#pragma once

class SimpleMedianFilter : public IMedianFilter {
public:
    explicit SimpleMedianFilter(cv::Mat image, int window_width, int window_height);
    cv::Mat apply() override;
private:
    cv::Vec3b getMedianForWindow(int x, int y);
    [[nodiscard]] bool correctCoord(int x, int y) const;

private:
    cv::Mat image_;
    int window_width_;
    int window_height_;
};
