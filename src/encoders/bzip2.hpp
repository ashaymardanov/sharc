//
// Created by arslan on 30.11.18.
//

#ifndef SHARC_BZIP2_HPP
#define SHARC_BZIP2_HPP

#include <sstream>
#include <boost/iostreams/filtering_streambuf.hpp>
#include <boost/iostreams/copy.hpp>
#include <boost/iostreams/filter/bzip2.hpp>

#include "encoder.hpp"

class Bzip2 : public Encoder {
public:

    static std::string compress(const std::string& data);
    static std::string decompress(const std::string& data);

//    static std::vector<size_t> compress(const std::vector<size_t>& data);
//    static std::vector<size_t> decompress(const std::vector<size_t>& data);

};

#endif //SHARC_BZIP2_HPP
