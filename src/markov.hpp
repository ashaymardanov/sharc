//
// Created by arslan on 22.10.18.
//

#ifndef SHARC_MARKOV_HPP
#define SHARC_MARKOV_HPP

#include "global_include.hpp"

template < typename ForwardIterator, typename T = typename std::iterator_traits<ForwardIterator>::value_type >
std::unordered_map< T, Counts<T> > build_markov(ForwardIterator begin, ForwardIterator end) {
#ifdef BUILD_WITH_EASY_PROFILER
    EASY_FUNCTION();
#endif

    std::unordered_map<T, Counts<T>> markov;
    ForwardIterator prev = begin;

    for(ForwardIterator cur=begin+1; cur!=end; cur++) {
        markov[*prev][*cur] += 1;
        prev = cur;
    }

    return markov;
}


template <typename T>
std::unordered_map< T, Counts<T> > build_markov(std::vector<T> text) {
#ifdef BUILD_WITH_EASY_PROFILER
    EASY_FUNCTION();
#endif

    std::unordered_map<T, Counts<T>> markov;

    for(size_t i=1; i<text.size(); i++) {
        markov[text[i-1]][text[i]] += 1;
    }

    return markov;
}


template <typename T>
std::unordered_map<T, Counts<T>> build_markov_context(std::unordered_map<T, Counts<T>> markov) {
#ifdef BUILD_WITH_EASY_PROFILER
    EASY_FUNCTION();
#endif

    std::unordered_map<T, Counts<T>> reordered_markov;

    for(const auto &i: markov) {
        std::cout << "i " << i.first << " " << i.second.size() << std::endl;
        std::vector<std::pair<T, size_t>> rel_keys_order(i.second.begin(), i.second.end());
        std::sort(rel_keys_order.begin(), rel_keys_order.end(), [](const auto &a, const auto &b){return a.second>b.second;});

        reordered_markov[i.first] = Counts<T>();

        for(size_t j=0; j<rel_keys_order.size(); j++) {
            reordered_markov[i.first][rel_keys_order[j].first] = j;
        }
    }

    return reordered_markov;
}

template <typename T>
std::vector<size_t> encode_by_markov_context(std::vector<T> text, std::unordered_map<T, Counts<T>> reordered_markov) {
#ifdef BUILD_WITH_EASY_PROFILER
    EASY_FUNCTION();
#endif

    std::vector<size_t> encoded;
    encoded.reserve(text.size());

    for(size_t i=1; i<text.size(); i++) {
        encoded.emplace_back(reordered_markov[text[i-1]][text[i]]);
    }

    return encoded;
}

#endif //SHARC_MARKOV_HPP
