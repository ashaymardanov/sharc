//
// Created by arslan on 30.10.18.
//

#ifndef SHARC_RLE_HPP
#define SHARC_RLE_HPP

#include "../global_include.hpp"

template <typename T>
std::vector<T> onetwo_encode_number(size_t num, T z1, T z2) {
    std::vector<T> res;

    while(num>0) {
        if(num & 1) {
            res.push_back(z1);
            num -= 1;
        } else {
            res.push_back(z2);
            num -= 2;
        }
        num >>= 1;
    }

    return res;
}


template <typename T>
std::vector<T> rle12_encode(const std::vector<T> &text, T z1, T z2, size_t factor) {
#ifdef BUILD_WITH_EASY_PROFILER
    EASY_FUNCTION();
#endif


    std::vector<T> res;
    res.reserve(text.size());

    T cur_elem = text[0];
    size_t run_len=0;

    for(const auto &i: text) {

        if(i==cur_elem) {
            run_len += 1;
        } else {
            if(run_len>=factor) {
                auto run_len_12 = onetwo_encode_number(run_len, z1, z2);
                for(const auto &j: run_len_12) {
                    res.push_back(j);
                }
                res.push_back(cur_elem);
            } else {
                for(size_t j=0; j<run_len; j++) {
                    res.push_back(cur_elem);
                }
            }
            run_len = 1;
            cur_elem = i;
        }
    }


    if(run_len>=factor) {
        auto run_len_12 = onetwo_encode_number(run_len, z1, z2);
        for(const auto &j: run_len_12) {
            res.push_back(j);
        }
        res.push_back(cur_elem);
    } else {
        for(size_t j=0; j<run_len; j++) {
            res.push_back(cur_elem);
        }
    }

    return res;
}


/*
template <typename T>
std::vector<T> rle_encode(const std::vector<T> &text) {
#ifdef BUILD_WITH_EASY_PROFILER
    EASY_FUNCTION();
#endif
    std::vector<T> res;
    res.reserve(text.size());

    T cur_elem = text[0];
    size_t run_len=0;

    for(const auto &i: text) {

        if(i==cur_elem) {
            run_len += 1;
        } else {
            res.push_back(run_len);
            res.push_back(cur_elem);

            run_len = 1;
            cur_elem = i;
        }
    }

    return res;
}
*/

#endif //SHARC_RLE_HPP

