cmake_minimum_required(VERSION 2.8)
project(cGeDBIT)

set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")

set (Boost_USE_STATIC_LIBS ON)
set (Boost_USE_MULTIHREADER ON)
set (Boost_USE_STATIC_RUNTIME OFF)

# Include the test data
configure_file(data/rawdata.txt
               ${CMAKE_CURRENT_BINARY_DIR}/data/rawdata.txt
               COPYONLY)

# Include head files in cGeDBIT
include_directories(src/HeaderFiles/index src/HeaderFiles/indexalgorithm
                    src/HeaderFiles/metric src/HeaderFiles/objects
                    src/HeaderFiles/query)

# Add index library in cGeDBIT
add_library(cGeDBIT_indexs
            src/SourceFiles/index/EcludedMiddleForest.cpp
            src/SourceFiles/index/Index.cpp
            src/SourceFiles/index/IndexNode.cpp
            src/SourceFiles/index/ListIndex.cpp
            src/SourceFiles/index/MVPIndex.cpp
            src/SourceFiles/index/MVPLeafNode.cpp
            src/SourceFiles/index/MVPInternalNode.cpp)

# Add indexalgorithm library in cGeDBIT
add_library(cGeDBIT_indexalgorithms
            src/SourceFiles/indexalgorithm/BalancePartitionMethod.cpp
            src/SourceFiles/indexalgorithm/EcludedMiddlePartitionMethod.cpp
            src/SourceFiles/indexalgorithm/FFTPivotSelectionMethod.cpp
            src/SourceFiles/indexalgorithm/PartitionResults.cpp)

# Add metric library in cGeDBIT
add_library(cGeDBIT_metrics
            src/SourceFiles/metric/Metric.cpp
            src/SourceFiles/metric/DNAMetric.cpp
            src/SourceFiles/metric/EditDistance.cpp
            src/SourceFiles/metric/EuclideanDistance.cpp
            src/SourceFiles/metric/ImageMetric.cpp
            src/SourceFiles/metric/MSMSMetric.cpp
            src/SourceFiles/metric/RNAMetric.cpp)

# Add object library in cGeDBIT
add_library(cGeDBIT_objects
            src/SourceFiles/objects/IndexObject.cpp
            src/SourceFiles/objects/DNA.cpp
            src/SourceFiles/objects/DoubleVector.cpp
            src/SourceFiles/objects/Image.cpp
            src/SourceFiles/objects/RNA.cpp
            src/SourceFiles/objects/StringObject.cpp
            src/SourceFiles/objects/TandemSpectra.cpp)

add_library(cGeDBIT_type
            src/SourceFiles/type/DoubleIndexObjectPair.cpp)

# Add query library cGeDBIT
add_library(cGeDBIT_querys
            src/SourceFiles/query/Query.cpp
            src/SourceFiles/query/RangeQuery.cpp)

# Add util library cGeDBIT
add_library(cGeDBIT_utils
            src/SourceFiles/util/getopt.cpp)


find_package (Boost 1.53 COMPONENTS serialization REQUIRED)
if (Boost_FOUND)
    include_directories (${Boost_INCLUDE_DIRS})
    add_executable(cGeDBIT src/app/BuildIndex.cpp)
    # Link libraries.
    target_link_libraries(cGeDBIT cGeDBIT_indexs cGeDBIT_indexalgorithms
                          cGeDBIT_metrics cGeDBIT_objects cGeDBIT_querys
                          cGeDBIT_type cGeDBIT_utils ${Boost_LIBRARIES})
endif()

