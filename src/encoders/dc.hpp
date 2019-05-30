//
// Created by arslan on 30.10.18.
//

#ifndef SHARC_DC_HPP
#define SHARC_DC_HPP

#include "../global_include.hpp"

template <typename T>
std::vector<size_t> dc_esc_encode(const std::vector<T> &text, const std::vector<T> &alphabet, size_t z1, size_t z2, size_t max_dist) {
#ifdef BUILD_WITH_EASY_PROFILER
    EASY_FUNCTION();
#endif

    if(max_dist==0) {
        max_dist = alphabet.size();
    }

    std::unordered_map<T, size_t> alphabet_map;
    for(size_t i=0; i<alphabet.size(); i++) {
        alphabet_map[alphabet[i]] = i+1;
    }

//    size_t escape_symbol = max_dist + 1000;
//    size_t reintroduce_symbol = max_dist + 2000;
    size_t escape_symbol = z1;
    size_t reintroduce_symbol = z2;

    std::vector<size_t> res;
    res.reserve(text.size()/2);

    std::vector<bool> visited(text.size(), false);


//    for(const auto &i: alphabet) {
//        auto index = std::find(text.begin(), text.end(), i) - text.begin();
//        res.emplace_back(index);
//        visited[index] = true;
//    }


    for(size_t i=0; i<text.size();) {

        if(visited[i]) { /// if already visited (i.e. encoded)
            /// then no need to re-introduce symbol
            auto it = std::find(text.begin() + i + 1, text.end(), text[i]); /// look for next occurence

            if(it != text.end()) { /// if this one is not the last
                auto index = it - text.begin();

                if (index - i == 1) { /// if symbol occurs right at the next pos, then no need to encode
                    visited[index] = true;

                } else { ///
                    auto zeros_count = std::count(visited.begin() + i, visited.begin() + index + 1, false); /// count for non-visited (i.e. yet non-encoded) positions
                    if (zeros_count <= max_dist) { /// restrict max distance
                        res.emplace_back(zeros_count);
                        visited[index] = true;

                    } else {
                        res.emplace_back(escape_symbol);

                    }
                }
            } else { /// if this one is the last
                res.emplace_back(escape_symbol);
            }

            i += 1;

        } else { /// if not visited (i.e. not encoded)
            /// reintroduce symbol
            std::cout << "re-introduce " << text[i] << " at pos " << i << std::endl;
            res.emplace_back(reintroduce_symbol);
            res.emplace_back(alphabet_map[text[i]]);
            visited[i] = true;
        }
    }

    return res;
}




template <typename T>
std::vector<T> dc_esc_decode(std::vector<size_t> text, std::vector<T> alphabet, size_t decoded_size, size_t escape_symbol, size_t reintroduce_symbol) {

}

#endif //SHARC_DC_HPP