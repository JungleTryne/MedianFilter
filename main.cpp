#include "filters/Algorithm.h"
#include "filters/ConstantTimeMedianFilter.h"
#include "filters/HuangMedianFilter.h"
#include "filters/SimpleMedianFilter.h"
#include "filters/OpenCVMedianFilter.h"

#include <opencv2/opencv.hpp>
#include <boost/program_options.hpp>
#include <chrono>

namespace po = boost::program_options;

int main(int argc, const char *argv[]) {
    bool dont_show_image = false;

    po::options_description desc("Allowed options");
    desc.add_options()
            ("help", "produce help message")
            ("algorithm", po::value<MedianFilterAlgorithm>(), "set algorithm for "
                                                    "median computation: [SIMPLE/HUANG/CONSTANT/OPENCV]")
            ("radius", po::value<int>(), "set median radius")
            ("filepath", po::value<std::string>(), "path to the picture")
            ("dont_show_image", po::bool_switch(&dont_show_image), "don't show image flag")
    ;

    po::variables_map vm;
    po::store(po::parse_command_line(argc, argv, desc), vm);
    po::notify(vm);

    if (vm.contains("help")) {
        std::cout << desc << std::endl;
        return 0;
    }

    if (!vm.contains("algorithm")) {
        std::cout << "Please specify algorithm" << std::endl;
        return 0;
    }

    if (!vm.contains("radius")) {
        std::cout << "Please specify radius" << std::endl;
        return 0;
    }

    if (!vm.contains("filepath")) {
        std::cout << "Please specify path to the image" << std::endl;
        return 0;
    }

    cv::Mat image = cv::imread(vm["filepath"].as<std::string>(), 1);
    int radius = vm["radius"].as<int>();

    std::unique_ptr<IMedianFilter> median_filter;
    auto algorithm = vm["algorithm"].as<MedianFilterAlgorithm>();
    switch (algorithm) {
        case MedianFilterAlgorithm::SIMPLE:
            median_filter = std::make_unique<SimpleMedianFilter>(image, radius, radius);
            break;
        case MedianFilterAlgorithm::HUANG:
            median_filter = std::make_unique<HuangMedianFilter>(image, radius, radius);
            break;
        case MedianFilterAlgorithm::CONSTANT:
            median_filter = std::make_unique<ConstantTimeMedianFilter>(image, radius, radius);
            break;
        case MedianFilterAlgorithm::OPENCV:
            median_filter = std::make_unique<OpenCVMedianFilter>(image, radius);
            break;
        default:
            std::cout << "Invalid algorithm" << std::endl;
            return 0;
    }

    namedWindow("Display Image", cv::WINDOW_AUTOSIZE);

    auto start = std::chrono::high_resolution_clock::now();
    auto new_image = median_filter->apply();
    auto stop = std::chrono::high_resolution_clock::now();

    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
    std::cout << duration.count() << std::endl;

    if (!dont_show_image) {
        imshow("Display Image", new_image);
        cv::waitKey(0);
        cv::imwrite("blurred.bmp", new_image);
    }

    return 0;
}
