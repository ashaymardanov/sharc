//
// Created by arslan on 18.11.18.
//

#ifndef SHARC_CHUNK_HUFFMAN_HPP
#define SHARC_CHUNK_HUFFMAN_HPP

#include "../global_include.hpp"
#include "../support.hpp"
#include "huffman.hpp"


template <typename T>
std::vector<std::vector<T>> split_chunks(const std::vector<T> &text, size_t max_chunk_len, size_t max_uniq_symbols) {
#ifdef BUILD_WITH_EASY_PROFILER
    EASY_FUNCTION();
#endif

    std::vector<std::vector<T>> res;

    size_t uniq_symbols = 0;
    size_t chunk_len = 0;
    T cur_symbol;
    std::vector<T> buf;
    for(size_t i=0; i<text.size();) {
        std::unordered_set<T> chunk_symbols;

        cur_symbol = text[i];
        chunk_symbols.emplace(cur_symbol);

        for(size_t chunk_len=0, uniq_symbols=1; chunk_len<max_chunk_len && uniq_symbols<=max_uniq_symbols;) {
            buf.emplace_back(cur_symbol);
            chunk_symbols.emplace(cur_symbol);

            chunk_len += 1;

            i++;
            cur_symbol = text[i];
            chunk_symbols.emplace(cur_symbol);
            uniq_symbols = chunk_symbols.size();
        }

        res.push_back(buf);
        buf.swap(std::vector<T>());
    }

    return res;
}


template <typename T>
std::vector<std::string> encode_chunks(std::vector<std::vector<T>> chunks, size_t max_huff_tables) {
    /// at first step, compress all chunks with different code tables

    /// calculate symbols counts for each chunk
    std::vector<Counts<T>> chunks_symbols_counts(chunks.size());

#ifdef BUILD_WITH_OPENMP
    #pragma omp parallel for
#endif
    for(size_t i=0; i<chunks.size(); i++) {
        chunks_symbols_counts[i] = count_elements(chunks[i]);
    }

/*
    /// calculate huffman codes for each chunk
    std::vector<HuffCodeMap<T>> huff_codes(chunks.size());

#ifdef BUILD_WITH_OPENMP
    #pragma omp parallel for
#endif
    for(size_t i=0; i<chunks.size(); i++) {
        HuffmanEncoder huffman_encoder(chunks_symbols_counts[i]);
        huffman_encoder.Encode();

        huff_codes[i] = huffman_encoder.outCodes;
    }
*/

    /// merge "similar" chunks by merging chunks Counts
    /// and comparing encoded size
    std::vector<size_t> chunk_huff_table(chunks.size(), 0);
    std::iota(chunk_huff_table.begin(), chunk_huff_table.end(), 0);




}



#endif //SHARC_CHUNK_HUFFMAN_HPP
