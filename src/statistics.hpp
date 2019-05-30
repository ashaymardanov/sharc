//
// Created by arslan on 13.08.18.
//

#ifndef SHARC_STATISTICS_HPP
#define SHARC_STATISTICS_HPP

#include "global_include.hpp"



typedef float (*wei_fun_type)(const std::string &key, size_t n, size_t L, size_t bi, size_t B);

fCounts<std::string> get_weights(Counts<std::string> &counts, size_t L, size_t bi, size_t B, wei_fun_type wei_fun);
void get_weights(std::vector<std::pair<std::string, float>> &weights, const Counts<std::string> &counts, size_t L, size_t bi, size_t B, wei_fun_type wei_fun);

float get_weight1(const std::string &key, size_t n, size_t L, size_t bi, size_t B);
float get_weight2(const std::string &key, size_t n, size_t L, size_t bi, size_t B);
float get_weight3(const std::string &key, size_t n, size_t L, size_t bi, size_t B);
float get_weight4(const std::string &key, size_t n, size_t L, size_t bi, size_t B);
float get_weight5(const std::string &key, size_t n, size_t L, size_t bi, size_t B);
float get_weight6(const std::string &key, size_t n, size_t L, size_t bi, size_t B);


static std::unordered_map<std::string, wei_fun_type> WEIGHT_FUNCTIONS = {
        {"1", get_weight1},
        {"2", get_weight2},
        {"3", get_weight3},
        {"4", get_weight4},
        {"5", get_weight5},
        {"6", get_weight6}
};

#endif //SHARC_STATISTICS_HPP
