#include "SimpleMedianFilter.h"

#include <utility>

SimpleMedianFilter::SimpleMedianFilter(cv::Mat image, int window_width, int window_height)
    : image_(std::move(image))
    , window_height_(window_height)
    , window_width_(window_width)
{}

cv::Mat SimpleMedianFilter::apply() {
    cv::Mat new_image = image_.clone();

    for (int x = 0; x < new_image.cols; ++x) {
        for (int y = 0; y < new_image.rows; ++y) {
            new_image.at<cv::Vec3b>(y, x) = getMedianForWindow(x, y);
        }
    }

    return new_image;
}

cv::Vec3b SimpleMedianFilter::getMedianForWindow(int x, int y) {
    cv::Vec3b result;
    for (int channel = 0; channel < image_.channels(); ++channel) {
        std::vector<int> window_values = {};
        for (int window_x = 0; window_x < window_width_; ++window_x) {
            for (int window_y = 0; window_y < window_height_; ++window_y) {
                if (!correctCoord(x + window_x - (window_width_ / 2), y + window_y - (window_height_ / 2))) {
                    continue;
                }
                window_values.push_back(image_.at<cv::Vec3b>(
                        y + window_y - (window_height_ / 2),
                        x + window_x - (window_width_ / 2)
                ).val[channel]);
            }
        }
        std::sort(window_values.begin(), window_values.end());
        result.val[channel] = window_values[window_values.size() / 2];
    }

    return result;
}

bool SimpleMedianFilter::correctCoord(int x, int y) const {
    return x >= 0 && x < image_.cols && y >= 0 && y < image_.rows;
}
