//
// Created by arslan on 01.12.18.
//

#include "compressor_support.hpp"

#include "keys.hpp"
#include "statistics.hpp"
#include "support.hpp"
#include "lcp.hpp"


Positions<std::string> get_keys(
        const std::vector<std::pair<std::string, size_t>> &blocks,
        const std::vector<size_t> &lcp,
        const std::string &text,
        size_t factor, size_t min_len, size_t max_len, wei_fun_type wei_fun) {
#ifdef BUILD_WITH_EASY_PROFILER
    EASY_FUNCTION();
#endif

    max_len = std::min(max_len, text.size());
    min_len = std::min(min_len, text.size());

    std::string key;

    /// for wei_fun
    const size_t B = 8;
    size_t bi = 2;
    size_t L = text.size();

    /// minimal_allowed_weight used to filter keys on each iteration
    float minimal_allowed_weight = wei_fun(std::string(min_len, '.'), factor, text.size(), bi, B);

    /// get max length of key at i-th position in text
    std::vector<size_t> max_key_len_at_pos(text.size(), 0);

//    max_key_len_at_pos[0] = lcp[0];
    max_key_len_at_pos[blocks[0].second] = lcp[0];
    for(size_t i=1; i<blocks.size(); i++) {
        max_key_len_at_pos[blocks[i].second] = std::max(lcp[i], lcp[i-1]);
//        std::cout << "max_key_len_at_pos[" << blocks[i].second << "] = " << max_key_len_at_pos[blocks[i].second] << std::endl;
    }

    /// get keys positions
    Positions<std::string> keys_positions = get_keys_positions_by_lcp(blocks, lcp, factor, min_len, max_len);

    /// ????
    Counts<std::string> keys_counts;

    /// calculate keys counts
    for(const auto &i: keys_positions) {
        keys_counts[i.first] = i.second.size();
    }

    /// calculate keys weights
    fCounts<std::string> keys_weights = get_weights(keys_counts, text.length(), bi, B, wei_fun);

    std::vector<std::pair<std::string, float>> weights(keys_weights.begin(), keys_weights.end());

    /// resulting order of keys with max weight on each iteration
//    std::vector<std::string> keys_order;

    /// resulting positions of filtered keys
    Positions<std::string> keys;

#ifdef DEBUG_OUTPUT
    std::cout << "Size of blocks: " << blocks.size() << std::endl;
    std::cout << "Size of lcp: " << lcp.size() << std::endl;
    std::cout << "Size of max_key_len_at_pos: " << max_key_len_at_pos.size() << std::endl;
    std::cout << "Size of keys_positions: " << keys_positions.size() << std::endl;
    std::cout << "Size of keys_counts: " << keys_counts.size() << std::endl;
    std::cout << "Size of keys_weights: " << keys_weights.size() << std::endl;
    std::cout << "Size of weights: " << weights.size() << std::endl;
#endif

#ifdef BUILD_WITH_EASY_PROFILER
    EASY_BLOCK("Main loop");
#endif
    /// main loop
    while(!weights.empty()) {
        bi = (size_t) log2(keys.size()) + 2;
#ifdef DEBUG_OUTPUT
        std::cout << "Size of weights: " << weights.size() << std::endl;
#endif

        /// sort vector to get key with max weight
        std::sort(weights.begin(), weights.end(), [](const auto &a, const auto &b){return a.second<b.second;});

        /// get key with max weight
        key = weights.back().first;

#ifdef DEBUG_OUTPUT
        std::cout << "next key: <" << key << "> " << weights.back().second << "<" << keys_counts[key] << ">" << std::endl;
#endif

        /// ... and remove it from weights
        weights.pop_back();

//        keys_order.push_back(key);
        keys_counts[key] = 0;

        keys[key] = std::unordered_set(keys_positions[key]); //.begin(), keys_positions[key].end());
        keys_positions.erase(key);

        L -= keys[key].size()*key.size();

#ifdef BUILD_WITH_EASY_PROFILER
        EASY_BLOCK("Positions recalculation");
#endif

        /// recalculate positions (i.e. count) for each key
        for(const auto &pos: keys[key]) { /// through all positions of current key
            std::string cur_key;
            /// at pos
            for(size_t j=min_len; j<=max_key_len_at_pos[pos] && pos+j<=text.size(); j++) {
                cur_key = text.substr(pos, j);
                if(keys_positions.count(cur_key)) {
                    keys_positions[cur_key].erase(pos);
                }
            }

            /// forward
            for(size_t i=1; i<key.size() && pos+i<=text.size(); i++) {
                for(size_t j=min_len; j<=max_key_len_at_pos[pos+i]; j++) {
                    cur_key = text.substr(pos+i, j);
                    if(keys_positions.count(cur_key)) {
                        keys_positions[cur_key].erase(pos+i);
                    }
                }
            }
            /// backward
            for(size_t i=1; i<=pos && i<=max_len; i++) {
                for(size_t j=std::max(i, min_len); j<=max_key_len_at_pos[pos-i]; j++) {
                    cur_key = text.substr(pos-i, j);
                    if(keys_positions.count(cur_key)) {
                        keys_positions[cur_key].erase(pos-i);
                    }
                }
            }

        }

#ifdef BUILD_WITH_EASY_PROFILER
        EASY_END_BLOCK;
#endif

#ifdef BUILD_WITH_EASY_PROFILER
        EASY_BLOCK("filter keys by count");
#endif
        /// filter keys by count
        for(auto it = keys_positions.begin(); it!=keys_positions.end(); ) {
            if(it->second.size()<factor) {
//                keys_counts.erase(it->first);
                keys_counts[it->first] = 0;
                it = keys_positions.erase(it);
            } else {
                keys_counts[it->first] = it->second.size();
                ++it;
            }
        }

#ifdef BUILD_WITH_EASY_PROFILER
        EASY_END_BLOCK;
#endif


#ifdef BUILD_WITH_EASY_PROFILER
        EASY_BLOCK("Keys weights recalc");
#endif
        /// renew weights vector
        get_weights(weights, keys_counts, L, bi, B, wei_fun);
/*
        /// delete keys with weight less than minimal
        for(const auto &i: keys_weights) {
            if(i.second<minimal_allowed_weight) {
                keys_counts.erase(i.first);
                keys_positions.erase(i.first);
            }
        }
*/


#ifdef BUILD_WITH_EASY_PROFILER
        EASY_END_BLOCK;
#endif

#ifdef BUILD_WITH_EASY_PROFILER
        EASY_BLOCK("filter weights vector");
#endif

        /// filter weights vector according to minimal_allowed_weight
        weights.erase(std::remove_if(weights.begin(), weights.end(), [minimal_allowed_weight](const auto &a){return a.second<minimal_allowed_weight;}), weights.end());

#ifdef BUILD_WITH_EASY_PROFILER
        EASY_END_BLOCK;
#endif

//#ifdef DEBUG_OUTPUT
        std::cout << "Size of weights after erase: " << weights.size() << std::endl;
//#endif
        /// end of iteration
    } /// end of main loop


    size_t sum_keys_len = 0;
    for(const auto &i: keys) {
        sum_keys_len += i.first.size();
    }

    std::cout << "sum_keys_len = " << sum_keys_len << std::endl;

    return keys;


}

Positions<std::string> get_keys(const std::string &text, size_t factor, size_t min_len, size_t max_len, wei_fun_type wei_fun) {
#ifdef BUILD_WITH_EASY_PROFILER
    EASY_FUNCTION();
#endif

    max_len = std::min(max_len, text.size());
    min_len = std::min(min_len, text.size());


    /// block size, in bytes
    const size_t block_size = max_len;

    /// split text on blocks and sort them
    std::vector<std::pair<std::string, size_t>> blocks;
    blocks = get_sorted_blocks(text, block_size);

    /// get lcp array
    std::vector<size_t> lcp;
    lcp = get_lcp_naive(blocks);

    return get_keys(blocks, lcp, text, factor, min_len, max_len, wei_fun);

}


std::vector<std::pair<size_t, size_t>> tokenize(const std::string &text, Positions<std::string> &keys) {
#ifdef BUILD_WITH_EASY_PROFILER
    EASY_FUNCTION();
#endif

    /// tokenize text according to keys positions
    std::vector<std::pair<size_t, size_t>> tokens_positions;

//    std::cout << "d = {" << std::endl;
    for(const auto &i: keys) {
        size_t l = i.first.size();
//        std::cout << u""""" << i.first << u"""" : {";
        for(const auto &j: i.second) {
            tokens_positions.emplace_back(j, l);
//            std::cout << j << ", ";
        }
//        std::cout << "}," << std::endl;
    }
//    std::cout << "}" << std::endl;

    std::sort(tokens_positions.begin(), tokens_positions.end(), [](const auto &a, const auto &b){return a.first<b.first;});

    return tokens_positions;
}


std::tuple<size_t, size_t, std::string> encode_by_suffarray(const std::vector<std::pair<std::string, size_t>> &suffs, const std::string &str) {

    auto interval = search_interval(suffs, str);
//    std::vector<std::pair<std::string, size_t>> sub_suffs(suffs.begin()+interval.first, suffs.begin()+interval.second);

    auto begin = suffs.begin()+interval.first;
    auto end = suffs.begin()+interval.second;

    auto lcps = get_lcps_naive(begin, end, str);

    int max_lcp = 0;
    int max_pos = -1;
    int max_suff_n = -1;

    for(size_t i=0; i<lcps.size(); i++) {
        if (lcps[i]>max_lcp) {
            max_lcp = lcps[i];
            max_pos = i;
            max_suff_n = (begin+i)->second;
        } else if (lcps[i] == max_lcp && (begin+i)->second < max_suff_n) {
            max_lcp = lcps[i];
            max_pos = i;
            max_suff_n = (begin+i)->second;
        }
    }

    if(max_pos == -1 || max_suff_n == -1) {
        return std::make_tuple(0, 0, str);
    }

    return std::make_tuple(max_suff_n, max_lcp, str.substr(max_lcp, str.size()));
}