//
// Created by arslan on 24.08.18.
//

#include "lcp.hpp"

/// get longest common prefix of two strings
/// by symbol-to-symbol comparing
size_t get_lcp_naive(const std::string &s1, const std::string &s2) {
#ifdef BUILD_WITH_EASY_PROFILER
    EASY_FUNCTION();
#endif

    size_t pos = 0;
    size_t min_len = std::min(s1.size(), s2.size());
    while (pos < min_len && s1[pos] == s2[pos]) {
        pos++;
    }

    return pos;
}


/// get longest common prefix of vector of strings
/// by symbol-to-symbol comparing
std::vector<size_t> get_lcp_naive(const std::vector<std::pair<std::string, size_t>> &suffs) {
#ifdef BUILD_WITH_EASY_PROFILER
    EASY_FUNCTION();
#endif

    std::vector<size_t> lcp(suffs.size(), 0);

#ifdef BUILD_WITH_OPENMP
    #pragma omp parallel for
#endif
    for(size_t i=0; i<suffs.size()-1; i++) {
        lcp[i] = get_lcp_naive(suffs[i].first, suffs[i+1].first);
    }

    return lcp;
}

/// get longest common prefix of given string and each of strings in vector
/// by symbol-to-symbol comparing
std::vector<size_t> get_lcps_naive(const std::vector<std::pair<std::string, size_t>> &suffs, const std::string &str) {
#ifdef BUILD_WITH_EASY_PROFILER
    EASY_FUNCTION();
#endif

    std::vector<size_t> res(suffs.size(), 0);

#ifdef BUILD_WITH_OPENMP
    #pragma omp parallel for
#endif
    for(size_t i=0; i<suffs.size(); i++) {
        res[i] = get_lcp_naive(suffs[i].first, str);
    }

    return res;
};


std::vector<size_t> get_lcps_naive(
        std::vector<std::pair<std::string, size_t>>::const_iterator begin,
        std::vector<std::pair<std::string, size_t>>::const_iterator end,
        const std::string &str) {
#ifdef BUILD_WITH_EASY_PROFILER
    EASY_FUNCTION();
#endif

    std::vector<size_t> res(end-begin, 0);

    size_t i = 0;
    for(auto it=begin; it!=end; it++) {
        res[i] = get_lcp_naive(it->first, str);
        i++;
    }

    return res;
};

