//
// Created by arslan on 13.08.18.
//

#ifndef SHARC_KEYS_HPP
#define SHARC_KEYS_HPP

#include "global_include.hpp"

Counts<std::string> get_keys_naive(std::string text, size_t factor, size_t min_len, size_t max_len);

Counts<std::string> get_keys_by_lcp(std::string text, size_t factor, size_t min_len, size_t max_len);

Positions<std::string> get_keys_positions_by_lcp(std::string text, size_t factor, size_t min_len, size_t max_len);

Positions<std::string> get_keys_positions_by_lcp(const std::vector<std::pair<std::string, size_t>> &suffs, const std::vector<size_t> &lcp, size_t factor, size_t min_len, size_t max_len);

#endif //SHARC_KEYS_HPP
