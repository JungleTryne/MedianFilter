#include "ConstantTimeMedianFilter.h"

#include <utility>

ConstantTimeMedianFilter::ConstantTimeMedianFilter(cv::Mat image, int window_width, int window_height)
        : window_width_(window_width)
        , window_height_(window_height)
        , image_(std::move(image))
        , new_image_(image_.clone())
        , hist_(256, 0)
{
    for (int col = 0; col < image_.cols; ++col) {
        cols_hist_.emplace_back(256, 0);
    }
}

cv::Mat ConstantTimeMedianFilter::apply() {
    for (int channel = 0; channel < image_.channels(); ++channel) {
        for (int col = 0; col < image_.cols; ++col) {
            cols_hist_[col] = std::vector<int>(256, 0);
            for (int y = 0; y < window_height_ / 2 - 1; ++y) {
                auto pixel_value = image_.at<cv::Vec3b>(y, col).val[channel];
                cols_hist_[col][pixel_value]++;
            }
        }

        for (int y_row = 0; y_row < new_image_.rows; ++y_row) {
            hist_ = std::vector(256, 0);
            hist_counter_ = 0;

            for (int x = 0; x < image_.cols; ++x) {
                int old_y = y_row - window_height_ / 2 - 1;
                int new_y = y_row + window_height_ / 2;

                if (old_y >= 0 && old_y < image_.rows) {
                    auto old_pixel_value = image_.at<cv::Vec3b>(old_y, x).val[channel];
                    cols_hist_[x][old_pixel_value]--;
                }

                if (new_y >= 0 && new_y < image_.rows) {
                    auto new_pixel_value = image_.at<cv::Vec3b>(new_y, x).val[channel];
                    cols_hist_[x][new_pixel_value]++;
                }
            }

            for (int x_window = 0; x_window <= window_width_ / 2; ++x_window) {
                for (int i = 0; i < 256; ++i) {
                    hist_[i] += cols_hist_[x_window][i];
                    hist_counter_ += cols_hist_[x_window][i];
                }
            }

            median_pixel_ = getMedian();
            less_median_counter_ = 0;
            for (int i = 0; i < 256; ++i) {
                less_median_counter_ += i < median_pixel_ ? hist_[i] : 0;
            }

            new_image_.at<cv::Vec3b>(y_row, 0).val[channel] = median_pixel_;

            for (int x = 1; x < new_image_.cols; ++x) {
                updateHistogram(x);
                updateMedian();

                assert(median_pixel_ < 256);
                assert(median_pixel_ >= 0);

                new_image_.at<cv::Vec3b>(y_row, x).val[channel] = median_pixel_;
            }
        }

    }
    return new_image_;
}


void ConstantTimeMedianFilter::updateHistogram(int x) {
    int x_to_remove = x - window_width_ / 2 - 1;
    int x_to_add = x + window_width_ / 2;

    for (int i = 0; i < 256; ++i) {
        if (x_to_remove < 0 || x_to_remove >= image_.cols) {
            break;
        }

        hist_[i] -= cols_hist_[x_to_remove][i];
        hist_counter_ -= cols_hist_[x_to_remove][i];
        if (i < median_pixel_) {
            less_median_counter_ -= cols_hist_[x_to_remove][i];
        }
    }

    for (int i = 0; i < 256; ++i) {
        if (x_to_add < 0 || x_to_add >= image_.cols) {
            break;
        }

        hist_[i] += cols_hist_[x_to_add][i];
        hist_counter_ += cols_hist_[x_to_add][i];
        if (i < median_pixel_) {
            less_median_counter_ += cols_hist_[x_to_add][i];
        }
    }
}

int ConstantTimeMedianFilter::getMedian() const {
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
    return -1;
}

void ConstantTimeMedianFilter::updateMedian() {
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
