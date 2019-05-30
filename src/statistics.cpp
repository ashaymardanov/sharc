//
// Created by arslan on 13.08.18.
//

#include "statistics.hpp"

float get_weight1(const std::string &key, size_t n, size_t L, size_t bi, size_t B) {
    return (float) key.length()*n*B/((n+1)*bi + key.length()*B);
}

float get_weight2(const std::string &key, size_t n, size_t L, size_t bi, size_t B) {
    return (float) L*B/(L*B - key.length()*(n-1)*B + (n+1)*bi);
}

float get_weight3(const std::string &key, size_t n, size_t L, size_t bi, size_t B) {
    return (float) n*key.length();
}

float get_weight4(const std::string &key, size_t n, size_t L, size_t bi, size_t B) {
    return (float) n*key.length()*log2(key.length());
}

float get_weight5(const std::string &key, size_t n, size_t L, size_t bi, size_t B) {
    return (float) n*key.length()*log2(n);
}

float get_weight6(const std::string &key, size_t n, size_t L, size_t bi, size_t B) {
    return (float) n*key.length();
}


/// get weights of all keys in count
fCounts<std::string> get_weights(Counts<std::string> &counts, size_t L, size_t bi, size_t B, wei_fun_type wei_fun) {
#ifdef BUILD_WITH_EASY_PROFILER
    EASY_FUNCTION();
#endif

//    std::vector<std::pair<std::string, size_t>> counts_vec(counts.begin(), counts.end());
//    std::vector<std::pair<std::string, float>> weights_vec(counts.size());
//
//#ifdef BUILD_WITH_OPENMP
//    #pragma omp parallel for
//#endif
//    for(size_t i=0; i<counts.size(); i++) {
//        weights_vec[i].first = counts_vec[i].first;
//        weights_vec[i].second = wei_fun(counts_vec[i].first, counts_vec[i].second, L, bi, B);
//    }
//    fCounts<std::string> weights(weights_vec.begin(), weights_vec.end());


    fCounts<std::string> weights;
    for(const auto &i: counts) {
        weights[i.first] = wei_fun(i.first, i.second, L, bi, B);
    }

    return weights;
}



void get_weights(std::vector<std::pair<std::string, float>> &weights, const Counts<std::string> &counts, size_t L, size_t bi, size_t B, wei_fun_type wei_fun) {
#ifdef BUILD_WITH_EASY_PROFILER
    EASY_FUNCTION();
#endif


//    std::for_each(weights.begin(), weights.end(), [&counts, &L, &bi, &B, &wei_fun](auto &a){a.second=wei_fun(a.first, counts[a.first], L, bi, B);});

/// omp doenst make things better here
/*#ifdef BUILD_WITH_OPENMP
    #pragma omp parallel for
#endif*/
    for(size_t i=0; i<weights.size(); i++) {
        try {
//            weights[i].second = wei_fun(weights[i].first, counts[weights[i].first], L, bi, B);
            weights[i].second = wei_fun(weights[i].first, counts.at(weights[i].first), L, bi, B);
        } catch (...) {
            weights[i].second = -1.0;
        }
//        if(counts.count(weights[i].first)) {
//            weights[i].second = wei_fun(weights[i].first, counts[weights[i].first], L, bi, B);
//        } else {
//            weights[i].second = 0.0;
//        }
    }
}
