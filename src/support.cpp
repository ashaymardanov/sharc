//
// Created by arslan on 13.08.18.
//

#include "support.hpp"
#include <boost/dynamic_bitset/dynamic_bitset.hpp>

//std::string repeat_string(const std::string &i, size_t n) {};

std::vector<std::pair<std::string, size_t>> get_blocks(const std::string &text, size_t block_size) {
#ifdef BUILD_WITH_EASY_PROFILER
    EASY_FUNCTION();
#endif

    /// blocks
    std::vector<std::pair<std::string, size_t>> blocks(text.size());

    /// split text on blocks
/// omp doenst make things better here
#ifdef BUILD_WITH_OPENMP
    #pragma omp parallel for
#endif
    for (size_t i=0; i < text.size()-1; i++) {
//        blocks.emplace_back(std::make_pair(text.substr(i, block_size), i));
        blocks[i] = std::make_pair(text.substr(i, block_size), i);
    }

    return blocks;
}

std::vector<std::pair<std::string, size_t>> get_sorted_blocks(const std::string &text, size_t block_size) {
#ifdef BUILD_WITH_EASY_PROFILER
    EASY_FUNCTION();
#endif

    auto blocks = get_blocks(text, block_size);

    std::sort(blocks.begin(), blocks.end(),  [](const auto &a, const auto &b){return a.first<b.first;});

    return blocks;
}


std::vector<unsigned char> dump_bits(std::vector<bool> text) {

    std::vector<unsigned char> res;
    res.reserve(text.size()/8);

    boost::dynamic_bitset<> buf(8);

    for(size_t i=0; i<text.size()/8; i++) {
        for(size_t j=0; j<8; j++) {
            buf[i] = text[i*8+j];
        }
        res.emplace_back((unsigned char) buf.to_ulong());
    }

    return res;
}