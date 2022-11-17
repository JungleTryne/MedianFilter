#include "OpenCVMedianFilter.h"

#include <opencv2/imgproc.hpp>

OpenCVMedianFilter::OpenCVMedianFilter(cv::Mat image, int radius)
        : image_(std::move(image))
        , radius_(radius)
{}

cv::Mat OpenCVMedianFilter::apply() {
    auto result = image_.clone();
    cv::medianBlur(image_, result, radius_);
    return result;
}
