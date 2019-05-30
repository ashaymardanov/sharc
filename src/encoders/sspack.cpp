//
// Created by arslan on 05.12.18.
//

#include "sspack.hpp"

std::string sspack_encode(const std::vector<std::string> &text, unsigned char max_char) {

    unsigned char max_len = (max_char-1)>>1;

    std::stringstream res;
    std::string s;

    for(const auto &i: text) {
        s = i;
        size_t len= s.size();
        if(len>max_len) {
            while(s.size()>max_len) {
                res << (char) ((max_len << 1) + 0);
                res << s.substr(0, max_len);
                s = s.substr(max_len, s.size());
            }
        }
        res << (char) ((s.size() << 1) + 1);
        res << s.substr(0, s.size());
    }

    return res.str();
}


std::vector<std::string> sspack_decode(std::string text) {

    std::vector<std::string> res;
    std::stringstream buf;

    for(size_t i=0; i<text.size(); i++) {
        unsigned char len = ((unsigned char) text[i]) >> 1;
        buf << text.substr(i+1, len);

        if(text[i] & 1) {
            res.emplace_back(buf.str());

            buf.str(std::string());
            buf.clear();
        }

        i += len;
    }

    return res;
}