//
// Created by arslan on 01.11.18.
//

#ifndef SHARC_CMTF_HPP
#define SHARC_CMTF_HPP

#include "../global_include.hpp"

/*
template <typename T>
std::vector<size_t> cmtf_encode(const std::vector<T> &text, const std::vector<T> &alphabet) {
    std::unordered_map<T, std::vector<T>> cmtf;
    std::unordered_map<T, Counts<T>> markov;

    std::vector<size_t> res;

    /// initialize model
    for(const auto &i: alphabet) {
        cmtf[i] = std::vector<T>(alphabet.size());
        for(size_t j=0; j<alphabet.size(); j++) {
            cmtf[i][j] = alphabet[j];
            markov[i][alphabet[j]] = 0;
        }
    }

    /// encode
    size_t code, index;
    T context = text[0];
    T cur_symbol;
    for(size_t i=1; i<text.size(); i++) {
        cur_symbol = text[i];
        auto it = std::find(cmtf[context].begin(), cmtf[context].end(), cur_symbol);
        index = it - cmtf[context].begin();

        res.emplace_back(index);

        /// update counter and model

        markov[context][cur_symbol] += 1;
        std::sort(cmtf[context].begin(), cmtf[context].end(), [&markov, &context](const T &a, const T &b){return markov.at(context).at(a)<markov.at(context).at(b);});

        context = cur_symbol;
    }

    return res;
}
*/


template <typename T>
std::vector<size_t> cmtf_encode(std::vector<T> text, std::vector<T> alphabet) {
#ifdef BUILD_WITH_EASY_PROFILER
    EASY_FUNCTION();
#endif

    std::vector<size_t> res;
    res.reserve(text.size());

    std::unordered_map<T, std::vector<T>> model;
    std::unordered_map<T, Counts<T>> markov;

    T prev = text[0];
    T cur;
    for(size_t i=1; i<text.size(); i++) {
        cur = text[i];
        if(markov[prev].count(cur)) {
            auto it = std::find(model[prev].begin(), model[prev].end(), cur);
            size_t index = it - model[prev].begin();

            res.emplace_back(index + 1);
        } else {
            auto it = std::find(alphabet.begin(), alphabet.end(), cur);
            size_t index = it - alphabet.begin();

            res.emplace_back(0);
            res.emplace_back(index);

            model[prev].emplace_back(cur);
        }

        markov[prev][cur] += 1;
        std::sort(model[prev].begin(), model[prev].end(), [&markov, &prev](const T &a, const T &b){return markov[prev][a]<markov[prev][b];});

        prev = cur;
    }

    return res;
}


#endif //SHARC_CMTF_HPP
