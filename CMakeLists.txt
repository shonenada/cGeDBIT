cmake_minimum_required(VERSION 2.8)
project(sqai-in-cpp CXX)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")

configure_file(data/arab1.con
               data/rna.con
               ${CMAKE_CURRENT_BINARY_DIR}/data/arab1.con
               ${CMAKE_CURRENT_BINARY_DIR}/data/rna.con
               COPYONLY)

include_directories(src/HeaderFiles/metric src/HeaderFiles/objects)

add_library(sqai_metric
            src/SourceFiles/metric/DNAMetric.cpp
            src/SourceFiles/metric/RNAMetric.cpp
            src/SourceFiles/metric/Metric.cpp)

add_library(sqai_objects
            src/SourceFiles/objects/DNA.cpp
            src/SourceFiles/objects/RNA.cpp
            src/SourceFiles/objects/IndexObject.cpp)

add_executable(sqai src/SourceFiles/app/main.cpp)
target_link_libraries(sqai sqai_metric sqai_objects)
