cmake_minimum_required(VERSION 3.23)
project(MedianFilter)

set(CMAKE_CXX_STANDARD 20)

find_package( OpenCV REQUIRED )
include_directories( ${OpenCV_INCLUDE_DIRS} )

find_package(Boost COMPONENTS program_options REQUIRED)
include_directories(${Boost_INCLUDE_DIR})

add_executable(
        MedianFilter
        main.cpp
        filters/SimpleMedianFilter.cpp
        filters/HuangMedianFilter.cpp
        filters/ConstantTimeMedianFilter.cpp
        filters/ConstantTimeMedianFilter.h
        filters/Algorithm.h
        filters/IMedianFilter.h filters/OpenCVMedianFilter.cpp filters/OpenCVMedianFilter.h)

target_link_libraries(MedianFilter ${OpenCV_LIBS} ${Boost_LIBRARIES} )

