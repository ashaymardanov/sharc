//
// Created by arslan on 04.11.18.
//

#ifndef SHARC_XOR_HPP
#define SHARC_XOR_HPP

#include "../global_include.hpp"

template <typename T>
std::vector<size_t> xor_encode(std::vector<T> text) {
#ifdef BUILD_WITH_EASY_PROFILER
    EASY_FUNCTION();
#endif

    std::vector<size_t> res(text.size());

    res[0] = text[0];

//#ifdef BUILD_WITH_OPENMP
//    #pragma omp parallel for
//#endif

#ifdef BUILD_WITH_OPENMP
    #pragma omp simd
#endif
    for(size_t i=1; i<text.size(); i++) {
        res[i] = text[i]^text[i-1];
    }

    return res;
}


template <typename ForwardIterator>
std::vector<size_t> xor_encode(ForwardIterator begin, ForwardIterator end) {
#ifdef BUILD_WITH_EASY_PROFILER
    EASY_FUNCTION();
#endif

    std::vector<size_t> res;
    res.emplace_back(*begin);

    ForwardIterator prev = begin;

    for(ForwardIterator it=begin+1; it!=end; it++) {
        res.emplace_back((*it)^(*prev));
        prev = it;
    }

    return res;
}


#endif //SHARC_XOR_HPP

