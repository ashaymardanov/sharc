//
// Created by arslan on 30.11.18.
//

#include "bzip2.hpp"


std::string Bzip2::compress(const std::string& data)
{
    namespace bio = boost::iostreams;

    std::stringstream compressed;
    std::stringstream origin(data);

    bio::filtering_streambuf<bio::input> out;
//    out.push(bio::bzip2_compressor(bio::bzip2_params(bio::bzip2::best_compression)));
    out.push(bio::bzip2_compressor(bio::bzip2_params()));
    out.push(origin);
    bio::copy(out, compressed);

    return compressed.str();
}

std::string Bzip2::decompress(const std::string& data)
{
    namespace bio = boost::iostreams;

    std::stringstream compressed(data);
    std::stringstream decompressed;

    bio::filtering_streambuf<bio::input> out;
    out.push(bio::bzip2_decompressor());
    out.push(compressed);
    bio::copy(out, decompressed);

    return decompressed.str();
}

