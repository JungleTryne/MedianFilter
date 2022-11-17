#include <opencv2/opencv.hpp>
#include "IMedianFilter.h"

#pragma once
class ConstantTimeMedianFilter : public IMedianFilter {
public:
    explicit ConstantTimeMedianFilter(cv::Mat image, int window_width, int window_height);
    cv::Mat apply() override;
private:
    [[nodiscard]] int getMedian() const;

    void updateMedian();
    void updateHistogram(int x);
private:
    cv::Mat image_;
    cv::Mat new_image_;
    int window_width_;
    int window_height_;

    std::vector<std::vector<int>> cols_hist_;
    std::vector<int> hist_;

    int less_median_counter_ = 0;
    int median_pixel_ = 0;
    int hist_counter_ = 0;
};
