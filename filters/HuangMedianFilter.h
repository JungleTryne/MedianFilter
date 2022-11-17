#include <opencv2/opencv.hpp>
#include "IMedianFilter.h"

#pragma once

class HuangMedianFilter : public IMedianFilter {
public:
    explicit HuangMedianFilter(cv::Mat image, int window_width, int window_height);
    cv::Mat apply() override;
private:
    void applyForRow(int y, int channel);

    [[nodiscard]] bool correctCoords(int x, int y) const;
    [[nodiscard]] int getMedian() const;

    void updateHistogram(int x, int y_row, int channel);
    void updateMedian();
private:
    cv::Mat image_;
    cv::Mat new_image_;
    int window_width_;
    int window_height_;

    std::vector<int> hist_;
    int less_median_counter_ = 0;
    int median_pixel_ = 0;
    int hist_counter_ = 0;
};
