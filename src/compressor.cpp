//
// Created by arslan on 24.08.18.
//

#include "compressor.hpp"
#include "compressor_support.hpp"

//#include "keys.hpp"
#include "statistics.hpp"
#include "support.hpp"
#include "lcp.hpp"

#include "markov.hpp"

#include "encoders/encoders.hpp"

#include "bwt/bwt.hpp"

//#include "proto/model.pb.h"
#include "proto/pack.hpp"
#include <pack.hpp>


std::string compress_block(const std::string &text, size_t factor, size_t min_len, size_t max_len, wei_fun_type wei_fun) {
#ifdef BUILD_WITH_EASY_PROFILER
    EASY_FUNCTION();
#endif

#ifdef BUILD_WITH_EASY_PROFILER
    EASY_BLOCK("Search for keys");
#endif

    min_len = std::min(text.size(), min_len);
    max_len = std::min(text.size(), max_len);

    std::cout << "text size : " << text.size() << std::endl;


    std::string delta = delta_encode(text, 0);

//    std::string text(text.begin(), text.end());


    /// block size, in bytes
    const size_t block_size = max_len;

    /// split text on blocks and sort them
    std::vector<std::pair<std::string, size_t>> blocks;
    blocks = get_sorted_blocks(text, block_size);

    /// get lcp array
    std::vector<size_t> lcp;
    lcp = get_lcp_naive(blocks);

    Positions<std::string> keys = get_keys(blocks, lcp, text, factor, min_len, max_len, wei_fun);

//    auto keys = get_keys(delta, factor, min_len, max_len, wei_fun);

    std::vector<std::string> keys_order;
    keys_order.reserve(keys.size());

    for (const auto &i: keys) {
        keys_order.emplace_back(i.first);
    }

    std::sort(keys_order.begin(), keys_order.end(),
              [&keys](const auto &a, const auto &b) { return keys[a].size() > keys[b].size(); });

    std::cout << "End of main loop" << std::endl;
    std::unordered_map<std::string, size_t> keys_order_map;
    for (size_t i = 0; i < keys_order.size(); i++) {
        keys_order_map[keys_order[i]] = i;
    }
    keys_order_map[""] = keys_order.size();

#ifdef BUILD_WITH_EASY_PROFILER
    EASY_END_BLOCK;
#endif


#ifdef DEBUG_OUTPUT
    std::cout << "Keys order size:" << keys_order.size() << std::endl;
    std::cout << "d = {" << std::endl;
    for(const auto &i: keys_order) {
        std::cout << "\"" << i << "\": ("; //<< std::endl;
        for(const auto &j: keys[i]) {
            std::cout << j << ", ";
        }
        std::cout << ")," << std::endl;
    }
    std::cout << "}" << std::endl;
#endif


#ifdef BUILD_WITH_EASY_PROFILER
    EASY_BLOCK("Tokenize");
#endif
    /// tokenize text according to keys positions
    std::vector<std::pair<size_t, size_t>> tokens_positions = tokenize(text, keys);

    std::vector<size_t> tokens; // (delta.begin(), delta.end());
    tokens.reserve(tokens_positions.size());

    std::vector<std::string> nonencoded_text_parts;

    std::cout << ">>> TOKENS: ";
    size_t cur_pos = 0;
    std::string between;
    size_t encoded_sum_len = 0;
    size_t nonencoded_sum_len = 0;
    std::cout << "tokenized : ";

    for (const auto &i: tokens_positions) {
        if (cur_pos < i.first) { /// get non-encoded string which is between two keys and put it in nonencoded_text_parts vector
            between = text.substr(cur_pos, i.first - cur_pos);
            nonencoded_text_parts.push_back(between);
            nonencoded_sum_len += between.size();
            std::cout << "b" << cur_pos << ":" << i.first - cur_pos << "<" << between << "> ";
        } else {
            tokens.push_back(keys_order_map[""]); /// escape i.e. there is nothing between two keys
        }

        { /// put key in tokens vector
            between = text.substr(i.first, i.second);
            tokens.push_back(keys_order_map[between]);
            cur_pos = i.first + i.second;
            encoded_sum_len += i.second;
        }

        std::cout << "k" << i.first << ":" << i.second << "<" << between << "> ";
    }

    /// get last non-encoded string which is after last key in text
    if(tokens_positions.back().first+tokens_positions.back().second<text.size()) {
        between = text.substr(tokens_positions.back().first+tokens_positions.back().second, text.size());
        nonencoded_text_parts.push_back(between);
        nonencoded_sum_len += between.size();

        std::cout << "<" << between << "> ";
    }

    {
        std::cout << "Encoded by suffarray: " << std::endl;
        std::vector<std::pair<std::string, size_t>> suffarray(keys_order_map.begin(), keys_order_map.end());
        std::vector<std::tuple<size_t, size_t, std::string>> nonencoded_text_tuples;
        nonencoded_text_tuples.reserve(nonencoded_text_parts.size());
        for (const auto &item: nonencoded_text_parts) {
            nonencoded_text_tuples.emplace_back(encode_by_suffarray(suffarray, item));
            std::cout << "<" << item << "> -> (" << std::get<0>(nonencoded_text_tuples.back()) << ", "
                      << std::get<1>(nonencoded_text_tuples.back()) << ", "
                      << "<" << std::get<2>(nonencoded_text_tuples.back()) << ">) ";
        }
    }


    std::cout << std::endl;

    std::string nonencoded_text = sspack_encode(nonencoded_text_parts, 255);

    std::cout << "nonencoded_text : <" << nonencoded_text << ">" << std::endl;



#ifdef BUILD_WITH_EASY_PROFILER
    EASY_END_BLOCK;
#endif


#ifdef BUILD_WITH_EASY_PROFILER
    EASY_BLOCK("Last stage");
#endif

    /// build markov model
    auto markov = build_markov(tokens);
    /// relative keys order in markov model
    auto counts = count_elements(markov);



    std::cout << " c1 = {";
    for (const auto &i: counts) {
        std::cout << "\t" << i.first << ": " << i.second << "," << std::endl;
    }

    size_t total_size_bits0 = 0;

    size_t total_len = 0;
    std::cout << ">>>>> TOTAL NUMBER OF KEYS " << counts.size() << std::endl;
    std::cout << ">>>>> TOTAL SIZE OF MARKOV CONTEXTS " << total_len << std::endl;
    std::cout << ">>>>> total_size_bits0 " << total_size_bits0 << std::endl;
    std::cout << ">>>>> total_size_bytes0 " << total_size_bits0 / 8 << std::endl;



    std::vector<size_t> tokens_alphabet(counts.size()+1, 0); //(keys_order_map.begin(), keys_order_map.end());
    std::iota(tokens_alphabet.begin(), tokens_alphabet.end(), 0);

    auto encoded = tokens;

    townsend::algorithm::bwtEncode(encoded.begin(), encoded.end());


    std::cout << "encoded: " << encoded.size() << std::endl;
    std::cout << " e = [";
    for (const auto &i: encoded) {
        std::cout << i << ", ";
    }
    std::cout << "]" << std::endl;


    /// numbers for 1-2 rle encoding or for escape/re-introduce symbols
    size_t z1 = *std::max_element(encoded.begin(), encoded.end()) + 1;
    size_t z2 = z1 + 1;

    auto res = dc_esc_encode(encoded, tokens_alphabet, z1, z2, z1-1);



////////////////// re-enumeration
    auto counts2 = count_elements(res);

#ifdef BUILD_WITH_EASY_PROFILER
    EASY_BLOCK("re-enumeration");
#endif

    counts2 = count_elements(res);

#ifdef BUILD_WITH_EASY_PROFILER
    EASY_END_BLOCK;
#endif

//////////////////



    std::cout << "res: " << res.size() << std::endl;;
    std::cout << " r = [";
    for(const auto &i: res) {
        std::cout << i << ", ";
    }
    std::cout << "]" << std::endl;

    std::cout << std::endl;
    std::cout << std::endl;
    std::cout << std::endl;
    std::cout << std::endl;


    HuffmanEncoder huffman_encoder(counts2);
    huffman_encoder.Encode();

    std::vector<size_t> alphabet(counts2.size());
    std::iota(alphabet.begin(), alphabet.end(), 0);
    huffman_encoder.Canonicalize(alphabet);

    auto codes = huffman_encoder.outCodes;
    size_t sum_len_bits=0;

    std::set<size_t> huffman_keys_lens;
    std::unordered_map<size_t, size_t> huffman_keys_lens_count;

    std::cout << " c = {";

    for(size_t i=0; i<counts2.size(); i++) {
        std::cout << "\t" << i << ": " << counts2[i] << ",  #" << codes[i].size() << " : "; //<< std::endl;
        for(auto j=codes[i].rbegin(); j!=codes[i].rend(); j++) {
            std::cout << *j;
        }
        std::cout << std::endl;

        huffman_keys_lens.insert(codes[i].size());
        huffman_keys_lens_count[codes[i].size()] += 1;

        sum_len_bits += counts2[i]*codes[i].size();
    }
    std::cout << "}" << std::endl;

    std::cout << "sum_len_bits = " << sum_len_bits << std::endl;
    std::cout << "sum_len_bytes = " << sum_len_bits/8 << std::endl;

#ifdef BUILD_WITH_EASY_PROFILER
    EASY_END_BLOCK;
#endif
    std::string gz_orig = Gzip::compress(text);
    std::string bz2_orig = Bzip2::compress(text);

    std::string gz = Gzip::compress(nonencoded_text);
    std::string bz2 = Bzip2::compress(nonencoded_text);

    std::cout << "gz_orig.size() = " << gz_orig.size() << std::endl;
    std::cout << "bz2_orig.size() = " << bz2_orig.size() << std::endl;

    std::cout << "gzip.size() = " << gz.size() << std::endl;
    std::cout << "bzip2.size() = " << bz2.size() << std::endl;

    std::cout << "encoded sum_len = " << encoded_sum_len << std::endl;
    std::cout << "nonencoded sum_len = " << nonencoded_sum_len << std::endl;

    std::cout << "nonencoded_text.size() = " << nonencoded_text.size() << std::endl;
    std::cout << "encoded_sum_len/sum_len_bytes = " << encoded_sum_len << "/" << sum_len_bits/8 << " = " << (float) encoded_sum_len/(sum_len_bits/8) << std::endl;



    std::string s1 = "";
    std::string s;
    s = pack_block(
                counts2.size(),
                s1,
                (uint32_t) sum_len_bits % 8,
                s1,
                nonencoded_text
    );

    std::cin>>s;


    if (min_len >= 4) min_len /= 2;

    compress_block(nonencoded_text, factor, min_len, max_len, get_weight3);
    return s;
}



std::string compress(const std::string &text, size_t block_size, size_t factor, size_t min_len, size_t max_len, wei_fun_type wei_fun) {
#ifdef BUILD_WITH_EASY_PROFILER
    EASY_FUNCTION();
#endif

    const size_t blocks_count = text.size()/block_size;

    std::vector<std::string> compressed_blocks(blocks_count);

#ifdef BUILD_WITH_OPENMP
    #pragma omp parallel for
#endif
    for(size_t i=0; i<blocks_count; i++) {
        std::cout << "Start block " << i << std::endl;
        std::string block = text.substr(i*block_size, block_size);
        auto compressed_block = compress_block(block, factor, min_len, max_len, wei_fun);
        compressed_blocks[i] = compressed_block;
//#ifdef BUILD_WITH_OPENMP
//    #pragma omp critical
//#endif
        {

            std::cout << "Block " << i << " compressed" << std::endl;
            std::cout << compressed_block << std::endl;
        }
    }

    std::stringstream ss;
    for(const auto &i: compressed_blocks) {
        ss << i;
    }

    return ss.str();
}

