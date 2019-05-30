//
// Created by arslan on 24.08.18.
//

#ifndef SHARC_LCP_HPP
#define SHARC_LCP_HPP

#include "global_include.hpp"

size_t get_lcp_naive(const std::string &s1, const std::string &s2);
std::vector<size_t> get_lcp_naive(const std::vector<std::pair<std::string, size_t>> &suffs);
std::vector<size_t> get_lcps_naive(const std::vector<std::pair<std::string, size_t>> &suffs, const std::string &str);


std::vector<size_t> get_lcps_naive(
        std::vector<std::pair<std::string, size_t>>::const_iterator begin,
        std::vector<std::pair<std::string, size_t>>::const_iterator end,
        const std::string &str);


#endif //SHARC_LCP_HPP
