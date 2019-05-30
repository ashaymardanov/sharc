//
// Created by arslan on 13.08.18.
//

#include "keys.hpp"
#include "lcp.hpp"

/*Counts<std::string> get_keys_naive(std::string text, size_t factor, size_t min_len, size_t max_len) {


}*/



Counts<std::string> get_keys_by_lcp(std::string text, size_t factor, size_t min_len, size_t max_len) {

    /// blocks
    std::vector<std::pair<std::string, size_t>> blocks;
    Counts<std::string> keys;

    /// lcp array
    std::vector<size_t> lcp;

    size_t block_size = max_len;

    std::string key;

    /// split text on blocks
    for(size_t i=0; i<text.size(); i++) {
        blocks.emplace_back(std::make_pair(text.substr(i, block_size), i));
    }

    /// sort blocks to find LCP
    std::sort(blocks.begin(), blocks.end());

    /// find LCP
    lcp = get_lcp_naive(blocks);

    /// count keys by naive
    for(size_t i=0; i<lcp.size(); i++) {

        for(auto j=min_len; j<=std::min(std::min(max_len, lcp[i]), blocks[i].first.length()); i++) {
            key = blocks[i].first.substr(0, j);

            keys[key] += 1;
        }
    }

    /// filter keys by count (factor)
    auto it = keys.begin();
    while (it != keys.end()) {
        if(it->second < factor) {
            it = keys.erase(it);
        }
        else {
            ++it;
        }
    }

    return keys;
}


Positions<std::string> get_keys_positions_by_lcp(std::string text, size_t factor, size_t min_len, size_t max_len) {

    /// blocks
    std::vector<std::pair<std::string, size_t>> blocks;

    ///
    Positions<std::string> keys;
//    std::vector<size_t> max_key_len(text.size(), 0);

    /// lcp array
    std::vector<size_t> lcp;

    size_t block_size = max_len;

    std::string key;

    /// split text on blocks
    for(size_t i=0; i<text.size(); i++) {
        blocks.emplace_back(std::make_pair(text.substr(i, block_size), i));
    }

    /// sort blocks to find LCP
    std::sort(blocks.begin(), blocks.end());

    /// find LCP
    lcp = get_lcp_naive(blocks);

    /// count keys by naive
    for(size_t i=0; i<lcp.size(); i++) {
//        max_key_len[blocks[i].second] = lcp[i];

        for(auto j=min_len; j<=std::min(std::min(max_len, lcp[i]), blocks[i].first.length()); i++) {
            key = blocks[i].first.substr(0, j);
            keys[key].emplace(i);

/*            if(keys.count(key)) {
//                keys[key].push_back(i);
                keys[key].emplace(i);
            }
            else {
//                keys[key] = std::vector<size_t>();
                keys[key] = std::unordered_set<size_t>();
            }*/
        }
    }

    /// filter keys by count (factor)
    auto it = keys.begin();
    while (it != keys.end()) {
        if(it->second.size() < factor) {
            it = keys.erase(it);
        }
        else {
            ++it;
        }
    }

    return keys;
}


Positions<std::string> get_keys_positions_by_lcp(const std::vector<std::pair<std::string, size_t>> &suffs, const std::vector<size_t> &lcp, size_t factor, size_t min_len, size_t max_len) {
#ifdef BUILD_WITH_EASY_PROFILER
    EASY_FUNCTION();
#endif


    ///
    std::unordered_map<std::string, std::vector<size_t>> keys;
    Positions<std::string> keys_positions;
    std::vector<size_t> max_key_len(suffs.size(), 0);

//    std::string key;

    /// count keys by naive
//#ifdef BUILD_WITH_OPENMP
//    #pragma omp parallel for
//#endif
    for(size_t i=0; i<suffs.size(); i++) {
//        max_key_len[suffs[i].second] = suffs[i];
//        size_t lim = std::min(std::min(max_len, lcp[i]), suffs[i].first.length());
        size_t lim = std::min(max_len, lcp[i]);

        #ifdef DEBUG_OUTPUT
        std::cout<< "lcp[" << i << "] = " << lcp[i] << "  " << lim << std::endl;
        #endif

        for(size_t j=min_len; j<=lim; j++) {
            std::string key = suffs[i].first.substr(0, j);
/*            if(keys.count(key)) {
//                keys[key].push_back(i);
                keys[key].emplace(i);
            }
            else {
//                keys[key] = std::vector<size_t>();
                keys[key] = std::unordered_set<size_t>();
                keys[key].emplace(i);
            }*/

/*
            if(!keys.count(key)) {
                keys[key] = std::unordered_set<size_t>();
            }
            keys[key].insert(suffs[i].second);
            keys[key].insert(suffs[i+1].second);
*/

//            if(!keys.count(key)) {
//                keys[key] = std::vector<size_t>();
//            }
//#ifdef BUILD_WITH_OPENMP
//    #pragma omp critical
//#endif
            {
                keys[key].emplace_back(suffs[i].second); /// todo: fix duplicates
                keys[key].emplace_back(suffs[i + 1].second); /// todo: fix duplicates
            }


            #ifdef DEBUG_OUTPUT
            std::cout << "new key: " << key << " " << keys[key].size() << std::endl;
            #endif
        }
    }

    /// filter keys by count (factor)
    for (auto it = keys.begin(); it != keys.end(); ) {

        #ifdef DEBUG_OUTPUT
        std::cout << "key: " << it->first << " " << it->second.size() << std::endl;
        #endif

        if(it->second.size() < factor) {
            it = keys.erase(it);
        }
        else {
            ++it;
        }
    }

    /// filter keys positions
    for(auto &i: keys) {
        std::string key = i.first;
        const size_t l = key.size();
        std::sort(i.second.begin(), i.second.end());

        keys_positions[key] = std::unordered_set<size_t>();

        size_t prev_pos = i.second[0];
        keys_positions[key].insert(prev_pos);
        for(size_t j=1; j<i.second.size(); j++) {
            if(i.second[j]-prev_pos>=l) {
                prev_pos = i.second[j];
                keys_positions[key].insert(prev_pos);
            }
        }
    }


    /// filter keys_positions by count (factor) again
    for (auto it = keys_positions.begin(); it != keys_positions.end(); ) {
//        std::cout << it->first << ": " << it->second.size() << std::endl;
        if(it->second.size() < factor) {
            it = keys_positions.erase(it);
        }
        else {
            ++it;
        }
    }

    return keys_positions;
}