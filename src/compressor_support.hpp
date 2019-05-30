//
// Created by arslan on 01.12.18.
//

#ifndef SHARC_COMPRESSOR_SUPPORT_HPP
#define SHARC_COMPRESSOR_SUPPORT_HPP

#include "global_include.hpp"
#include "statistics.hpp"

Positions<std::string> get_keys(
        const std::vector<std::pair<std::string, size_t>> &blocks,
        const std::vector<size_t> &lcp,
        const std::string &text,
        size_t factor, size_t min_len, size_t max_len, wei_fun_type wei_fun);

Positions<std::string> get_keys(const std::string &text, size_t factor, size_t min_len, size_t max_len, wei_fun_type wei_fun);
std::vector<std::pair<size_t, size_t>> tokenize(const std::string &text, Positions<std::string> &keys);

std::tuple<size_t, size_t, std::string> encode_by_suffarray(const std::vector<std::pair<std::string, size_t>> &suffs, const std::string &str);

#endif //SHARC_COMPRESSOR_SUPPORT_HPP
