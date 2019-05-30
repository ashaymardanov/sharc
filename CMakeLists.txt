cmake_minimum_required(VERSION 3.11)
project(sharc CXX)

if(NOT WIN32)
    string(ASCII 27 Esc)
    set(ColourReset "${Esc}[m")
    set(ColourBold  "${Esc}[1m")
    set(Red         "${Esc}[31m")
    set(Green       "${Esc}[32m")
    set(Yellow      "${Esc}[33m")
    set(Blue        "${Esc}[34m")
    set(Magenta     "${Esc}[35m")
    set(Cyan        "${Esc}[36m")
    set(White       "${Esc}[37m")
    set(BoldRed     "${Esc}[1;31m")
    set(BoldGreen   "${Esc}[1;32m")
    set(BoldYellow  "${Esc}[1;33m")
    set(BoldBlue    "${Esc}[1;34m")
    set(BoldMagenta "${Esc}[1;35m")
    set(BoldCyan    "${Esc}[1;36m")
    set(BoldWhite   "${Esc}[1;37m")
endif()

set(CMAKE_CXX_STANDARD 17)

#ADD_DEFINITIONS(-DDEBUG_OUTPUT)
option(ENABLE_DEBUG_OUTPUT "Enable DEBUG_OUTPUT" OFF)
option(BUILD_WITH_OPENMP "Build with OpenMP" OFF)
option(BUILD_WITH_EASY_PROFILER "Build with easy_profiler" OFF)

if(ENABLE_DEBUG_OUTPUT)
    ADD_DEFINITIONS(-DDEBUG_OUTPUT)
endif()

if(BUILD_WITH_EASY_PROFILER)
    find_package(easy_profiler REQUIRED)
    ADD_DEFINITIONS(-DBUILD_WITH_EASY_PROFILER)
endif()

if(BUILD_WITH_OPENMP)
    add_definitions(-DBUILD_WITH_OPENMP)

    find_package(OpenMP)
    if (OPENMP_FOUND)
        set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
        set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
        set (CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${OpenMP_EXE_LINKER_FLAGS}")
    endif()

    add_compile_options( -fopenmp )
endif()


set(Boost_USE_STATIC_LIBS ON)
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_RUNTIME ON)
find_package(Boost COMPONENTS iostreams program_options REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})


find_package(ZLIB REQUIRED)
find_package(BZip2 REQUIRED)

add_compile_options(
        -fexceptions
        -fPIC
        -static
        -static-libstdc++
        -Wall
        -Wextra
        -march=native
        -lpthreads
        -lz
        -lbz2
        -lboost_iostreams
)

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS} ${CMAKE_CXX_FLAGS_DEBUG} -fprofile-arcs -ftest-coverage -lgcov -O0 -g -pg")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS} ${CMAKE_CXX_FLAGS_RELEASE} -O3 -s")

add_subdirectory(src)
include_directories(src)

add_executable(sharc main.cpp)
target_link_libraries(sharc compressor)
target_link_libraries(sharc ${Boost_LIBRARIES} boost_iostreams)
target_link_libraries(sharc z bz2)


#target_link_libraries(sharc compressor keys lcp statistics support markov)
#target_link_libraries(sharc compressor keys lcp statistics support markov cpp-optparse)


if(BUILD_WITH_EASY_PROFILER)
    target_link_libraries(sharc easy_profiler)
endif()