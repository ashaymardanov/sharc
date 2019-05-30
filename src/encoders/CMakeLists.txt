cmake_minimum_required(VERSION 3.11)
project(sharc)

set(CMAKE_CXX_STANDARD 17)

#include_directories("../")

#[[#add_library(encoders STATIC encoders.cpp encoders.hpp dc.cpp dc.hpp)
add_library(dc_encoder OBJECT dc.cpp dc.hpp)
add_library(rle_encoder OBJECT rle.cpp rle.hpp)
add_library(cmtf_encoder OBJECT cmtf.cpp cmtf.hpp)
add_library(delta_encoder OBJECT delta.cpp delta.hpp)
add_library(huffman_encoder OBJECT huffman.cpp huffman.cpp)
add_library(chunk_huffman_encoder OBJECT chunk_huffman.cpp chunk_huffman.hpp)

#add_library(encoders STATIC encoders.cpp encoders.hpp)
add_library(encoders STATIC encoders.cpp encoders.hpp)
target_link_libraries(encoders
        $<TARGET_OBJECTS:dc_encoder>
        $<TARGET_OBJECTS:rle_encoder>
        $<TARGET_OBJECTS:cmtf_encoder>
        $<TARGET_OBJECTS:delta_encoder>
        $<TARGET_OBJECTS:huffman_encoder>
        $<TARGET_OBJECTS:chunk_huffman_encoder>
)]]


add_library(dc_encoder STATIC dc.cpp dc.hpp)
add_library(rle_encoder STATIC rle.cpp rle.hpp)
add_library(cmtf_encoder STATIC cmtf.cpp cmtf.hpp)
add_library(delta_encoder STATIC delta.cpp delta.hpp)
add_library(huffman_encoder STATIC huffman.cpp huffman.cpp)
add_library(chunk_huffman_encoder STATIC chunk_huffman.cpp chunk_huffman.hpp)
add_library(gzip STATIC gzip.cpp gzip.hpp)
add_library(bzip2 STATIC bzip2.cpp bzip2.hpp)
add_library(sspack STATIC sspack.cpp sspack.hpp)

#add_library(encoders STATIC encoders.cpp encoders.hpp)
add_library(encoders STATIC encoders.cpp encoders.hpp)
target_link_libraries(encoders
        dc_encoder
        rle_encoder
        cmtf_encoder
        delta_encoder
        huffman_encoder
        chunk_huffman_encoder
        gzip
        bzip2
        sspack
)
