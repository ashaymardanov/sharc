//
// Created by arslan on 24.08.18.
//

#ifndef SHARC_COMPRESSOR_HPP
#define SHARC_COMPRESSOR_HPP

#include "global_include.hpp"
#include "statistics.hpp"


std::string compress_block(const std::string &text, size_t factor, size_t min_len, size_t max_len, wei_fun_type wei_fun);

std::string compress(const std::string &text, size_t block_size, size_t factor, size_t min_len, size_t max_len, wei_fun_type wei_fun);

#endif //SHARC_COMPRESSOR_HPP
