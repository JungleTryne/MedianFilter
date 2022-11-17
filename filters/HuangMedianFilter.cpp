#include "HuangMedianFilter.h"

#include <numeric>
#include <utility>

HuangMedianFilter::HuangMedianFilter(cv::Mat image, int window_width, int window_height)
    : window_width_(window_width)
    , window_height_(window_height)
    , image_(std::move(image))
    , new_image_(image_.clone())
    , hist_(256, 0)
{}

cv::Mat HuangMedianFilter::apply() {
    for (int channel = 0; channel < image_.channels(); ++channel) {
        for (int y = 0; y < new_image_.rows; ++y) {
            applyForRow(y, channel);
        }
    }
    return new_image_;
}

void HuangMedianFilter::applyForRow(int y_row, int channel) {
    hist_ = std::vector(256, 0);
    hist_counter_ = 0;

    for (int x = 0; x <= window_width_ / 2; ++x) {
        for (int y = y_row - window_height_ / 2; y <= y_row + window_height_ / 2; ++y) {
            if (!correctCoords(x, y)) {
                continue;
            }
            auto pixel_value = image_.at<cv::Vec3b>(y, x).val[channel];
            hist_[pixel_value]++;
            hist_counter_++;
        }
    }

    median_pixel_ = getMedian();
    less_median_counter_ = 0;
    for (int i = 0; i < 256; ++i) {
        less_median_counter_ += i < median_pixel_ ? hist_[i] : 0;
    }

    new_image_.at<cv::Vec3b>(y_row, 0).val[channel] = median_pixel_;

    for (int x = 1; x < new_image_.cols; ++x) {
        updateHistogram(x, y_row, channel);
        updateMedian();

        assert(median_pixel_ < 256);
        assert(median_pixel_ >= 0);

        new_image_.at<cv::Vec3b>(y_row, x).val[channel] = median_pixel_;
    }
}

int HuangMedianFilter::getMedian() const {
    int counter = 0;
    for (int i = 0; i < 256; ++i) {
        counter += hist_[i];
        if (counter > hist_counter_ / 2) {
            while (i > 0 && hist_[i] == 0) {
                i--;
            }
            return i;
        }
    }

    assert(false);
}

void HuangMedianFilter::updateHistogram(int x, int y_row, int channel) {
    for (int y = y_row - window_height_ / 2; y <= y_row + window_height_ / 2; ++y) {
        if (!correctCoords(x - window_width_ / 2 - 1, y)) {
            continue;
        }

        auto pixel_value = image_.at<cv::Vec3b>(y, x - window_width_ / 2 - 1).val[channel];
        if (pixel_value < median_pixel_) {
            less_median_counter_--;
        }
        hist_[pixel_value]--;
        hist_counter_--;
    }

    for (int y = y_row - window_height_ / 2; y <= y_row + window_height_ / 2; ++y) {
        if (!correctCoords(x + window_width_ / 2, y)) {
            continue;
        }
        auto pixel_value = image_.at<cv::Vec3b>(y, x + window_width_ / 2).val[channel];
        if (pixel_value < median_pixel_) {
            less_median_counter_++;
        }
        hist_[pixel_value]++;
        hist_counter_++;
    }
}

void HuangMedianFilter::updateMedian() {
    if (less_median_counter_ >= hist_counter_ / 2) {
        while (less_median_counter_ > hist_counter_ / 2) {
            median_pixel_--;
            less_median_counter_ -= hist_[median_pixel_];
        }

        while (hist_[median_pixel_] == 0) {
            median_pixel_++;
        }
    } else {
        while (less_median_counter_ + hist_[median_pixel_] <= hist_counter_ / 2) {
            less_median_counter_ += hist_[median_pixel_];
            median_pixel_++;
        }

        while (hist_[median_pixel_] == 0) {
            median_pixel_--;
        }
    }
}

bool HuangMedianFilter::correctCoords(int x, int y) const {
    return x >= 0 && x < image_.cols && y >= 0 && y < image_.rows;
}

