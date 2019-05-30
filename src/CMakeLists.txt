
add_subdirectory(bwt)
add_subdirectory(encoders)
add_subdirectory(proto)


add_library(keys STATIC keys.cpp keys.hpp lcp.cpp lcp.hpp)
add_library(statistics STATIC statistics.cpp statistics.hpp)
add_library(support STATIC support.cpp support.hpp)
add_library(lcp STATIC lcp.cpp lcp.hpp)
add_library(markov STATIC markov.cpp markov.hpp)


add_library(compressor STATIC compressor.cpp compressor.hpp compressor_support.cpp compressor_support.hpp)

target_include_directories(compressor PUBLIC bwt)
target_include_directories(compressor PUBLIC encoders)
#target_include_directories(compressor PUBLIC proto)
target_include_directories(compressor PUBLIC "${CMAKE_CURRENT_BINARY_DIR}/proto")

target_link_libraries(compressor encoders keys lcp statistics support markov pack)


#[[add_library(keys OBJECT keys.cpp keys.hpp)
add_library(statistics OBJECT statistics.cpp statistics.hpp)
add_library(support OBJECT support.cpp support.hpp)
add_library(lcp OBJECT lcp.cpp lcp.hpp)
add_library(markov OBJECT markov.cpp markov.hpp)

target_link_libraries(compressor
#        $<TARGET_OBJECTS:encoders>
        encoders
        $<TARGET_OBJECTS:keys>
        $<TARGET_OBJECTS:lcp>
        $<TARGET_OBJECTS:statistics>
        $<TARGET_OBJECTS:support>
        proto
)]]
