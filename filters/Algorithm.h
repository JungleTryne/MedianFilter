#pragma once

#include <iostream>

#include <boost/algorithm/string.hpp>

namespace alg = boost::algorithm;

enum class MedianFilterAlgorithm {
    SIMPLE,
    HUANG,
    CONSTANT,
    OPENCV
};

std::istream& operator>>(std::istream& in, MedianFilterAlgorithm& algorithm) {
    std::string token;
    in >> token;
    
    if (alg::to_lower_copy(token) == "simple") {
        algorithm = MedianFilterAlgorithm::SIMPLE;
    } else if (alg::to_lower_copy(token) == "huang") {
        algorithm = MedianFilterAlgorithm::HUANG;
    } else if (alg::to_lower_copy(token) == "constant") {
        algorithm = MedianFilterAlgorithm::CONSTANT;
    } else if (alg::to_lower_copy(token) == "opencv") {
        algorithm = MedianFilterAlgorithm::OPENCV;
    } else {
        in.setstate(std::ios_base::failbit);
    }

    return in;
}
