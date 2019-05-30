//
// Created by arslan on 13.08.18.
//

#ifndef SHARC_GLOBAL_INCLUDE_HPP
#define SHARC_GLOBAL_INCLUDE_HPP

#include <cstddef>
#include <cmath>
#include <string>
#include <algorithm>
#include <set>
#include <vector>
#include <unordered_map>
#include <unordered_set>

#include <numeric>

#include <iostream>
#include <sstream>

#ifdef BUILD_WITH_OPENMP
    #include <omp.h>
#endif

#ifdef BUILD_WITH_EASY_PROFILER
    #include <easy/profiler.h>
#endif


template <typename DataType>
using Counts = std::unordered_map<DataType, size_t>;

template <typename DataType>
using fCounts = std::unordered_map<DataType, float>;

template <typename DataType>
using Positions = std::unordered_map<DataType, std::unordered_set<size_t>>;


#endif //SHARC_GLOBAL_INCLUDE_HPP
