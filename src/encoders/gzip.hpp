//
// Created by arslan on 30.11.18.
//

#ifndef SHARC_GZIP_HPP
#define SHARC_GZIP_HPP

#include <sstream>
#include <boost/iostreams/filtering_streambuf.hpp>
#include <boost/iostreams/copy.hpp>
#include <boost/iostreams/filter/gzip.hpp>

class Gzip {
public:

    static std::string compress(const std::string& data);

    static std::string decompress(const std::string& data);
};

#endif //SHARC_GZIP_HPP
