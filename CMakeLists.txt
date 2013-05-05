cmake_minimum_required(VERSION 2.8)
project(cGeDBIT)

set(CMAKE_CXX_FLAGES "${CMAKE_CXX_FLAGS} -std=c++0x")

configure_file(data/rawdata.txt
               ${CMAKE_CURRENT_BINARY_DIR}/data/rawdata.txt
               COPYONLY)

include_directories(HeaderFiles/index HeaderFiles/indexalgorithm
                    HeaderFiles/metrix HeaderFiles/objects HeaderFiles/query
                    HeaderFiles/type HeaderFiles/uitl
                    /home/shonenada/Projects/nhpcc/boost_1_53_0)

add_library(cGeDBIT_indexs
            SourceFiles/index/EcludedMiddleForest.cpp
            SourceFiles/index/Index.cpp
            SourceFiles/index/IndexNode.cpp
            SourceFiles/index/ListIndex.cpp
            SourceFiles/index/MVPIndex.cpp
            SourceFiles/index/MVPLeafNode.cpp
            SourceFiles/index/MVPInternalNode.cpp)

add_library(cGeDBIT_indexalgorithm
            SourceFiles/indexalgorithm/BalancePartitionMethod.cpp
            SourceFiles/indexalgorithm/EcludedMiddlePartitionMethod.cpp
            SourceFiles/indexalgorithm/FFTPivotSelectionMethod.cpp
            SourceFiles/indexalgorithm/PartitionResualts.cpp)

add_library(cGeDBIT_metrics
            SourceFiles/metric/DNAMetric.cpp
            SourceFiles/metric/EditDistance.cpp
            SourceFiles/metric/EuclideanDistance.cpp
            SourceFiles/metric/ImageMetric.cpp
            SourceFiles/metric/MSMSMetric.cpp
            SourceFiles/metric/Metric.cpp
            SourceFiles/metric/RNAMetric.cpp)

add_library(cGeDBIT_objects
            SourceFiles/objects/DNA.cpp
            SourceFiles/objects/DoubleVector.cpp
            SourceFiles/objects/Image.cpp
            SourceFiles/objects/IndexObject.cpp
            SourceFiles/objects/RNA.cpp
            SourceFiles/objects/StringObject.cpp
            SourceFiles/objects/TandemSpectra.cpp)

add_library(cGeDBIT_querys
            SourceFiles/query/Query.cpp
            SourceFiles/query/RangeQuery.cpp
            SourceFiles/type/DoubleIndexObjectPair.cpp
            SourceFiles/util/getopt.cpp)

add_executable(cGeDBIT APP/BuildMVPIndex.cpp)
target_link_libraries(cGeDBIT cGeDBIT_indexs cGeDBIT_indexalgorithm
                      cGeDBIT_metrics cGeDBIT_objects cGeDBIT_querys)
