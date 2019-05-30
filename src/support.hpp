//
// Created by arslan on 13.08.18.
//

#ifndef SHARC_SUPPORT_HPP
#define SHARC_SUPPORT_HPP

#include "global_include.hpp"

//std::string repeat_string(const std::string &i, size_t n);

std::vector<std::pair<std::string, size_t>> get_blocks(const std::string &text, size_t block_size);
std::vector<std::pair<std::string, size_t>> get_sorted_blocks(const std::string &text, size_t block_size);

std::vector<unsigned char> dump_bits(std::vector<bool> text);


template <typename T>
Counts<T> count_elements(std::vector<T> text) {
#ifdef BUILD_WITH_EASY_PROFILER
    EASY_FUNCTION();
#endif

    Counts<T> res;

    for(const auto &i: text) {
        res[i]++;
    }

    return res;
}

template <typename T1, typename T2>
Counts<T2> count_elements(std::unordered_map<T1, Counts<T2>> text) {
#ifdef BUILD_WITH_EASY_PROFILER
    EASY_FUNCTION();
#endif

    Counts<T2> res;

    for(const auto &i: text) {
        for(const auto &j: i.second) {
//            res[j.first]++;
            res[j.first] += j.second;
        }
    }

    return res;
}


//template <typename T1, typename T2>
//Counts<T2> count_total(std::unordered_map<T1, Counts<T2>> text) {
//#ifdef BUILD_WITH_EASY_PROFILER
//    EASY_FUNCTION();
//#endif
//
//    Counts<T2> res;
//
//    for(const auto &i: text) {
//        for(const auto &j: i.second) {
//            res[j.first]++;
//        }
//    }
//
//    return res;
//}

template <typename T>
std::pair<size_t, size_t> search_interval(std::vector<std::pair<T, size_t>> sorted_vec, T elem) {
    size_t l = 0;
    size_t r = sorted_vec.size();
    size_t mid = 0;
    while (l < r) {
        mid = (l+r)/2;
        if (elem > sorted_vec[mid].first) {
            l = mid + 1;
        } else {
            r = mid;
        }
    }

    size_t s = l;
    r = sorted_vec.size();
    while (l < r) {
        mid = (l+r)/2;
        if (elem < sorted_vec[mid].first) {
            r = mid;
        } else {
            l = mid + 1;
        }
    }

    return std::make_pair(s, r);
}

#endif //SHARC_SUPPORT_HPP
