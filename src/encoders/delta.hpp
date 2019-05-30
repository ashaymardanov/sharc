//
// Created by arslan on 04.11.18.
//

#ifndef SHARC_DELTA_HPP
#define SHARC_DELTA_HPP

#include <limits>

#include "../global_include.hpp"

/*
template <typename T>
std::vector<T> delta_encode(const std::vector<T> &text, T mod=0) {
#ifdef BUILD_WITH_EASY_PROFILER
    EASY_FUNCTION();
#endif

    mod = mod ? mod : std::numeric_limits<T>::max();

    std::vector<T> res(text.size());

    res[0] = (T) text[0];

#ifdef BUILD_WITH_OPENMP
    #pragma omp parallel for
#endif
    for(size_t i=1; i<text.size(); i++) {
        res[i] = (T) ( ((long int)text[i] - (long int)text[i-1]) % (long int)mod );
    }

    return res;
}
*/

template <typename T1, typename T2>
T1 delta_encode(const T1 &text, T2 mod=0) {
     mod = mod ? mod : std::numeric_limits<T2>::max();

    T1 vec(text.begin(), text.end());

    typename T1::const_iterator prev = text.begin();

    for(typename T1::iterator it = vec.begin()+1; it!=vec.end(); it++, prev++) {
        *it = (*it - *prev) % mod;
    }


    return vec;
}

#endif //SHARC_DELTA_HPP
