#include <iostream>
#include <fstream>

#include "global_include.hpp"
#include "compressor.hpp"
#include "statistics.hpp"

#include "boost/program_options.hpp"

namespace po = boost::program_options;


void usage(po::options_description options_desc){
    std::cerr << "Usage: " << std::endl;

    std::cerr << std::endl;
    std::cerr << options_desc << std::endl;
}



int main(int argc, const char **argv) {
    #ifdef BUILD_WITH_EASY_PROFILER
        EASY_PROFILER_ENABLE;
        profiler::startListen();
    #endif

    po::options_description options_desc("Allowed options (See readme for more detailed description)");
    options_desc.add_options()
        ("help", "produce help message")
        ("ifile,i", po::value<std::string>()->required(), "write report to FILE")
        ("block-size,b", po::value<int>()->required(), "block size, bytes")
        ("factor", po::value<int>()->required(), "factor")
        ("min-len", po::value<int>()->required(), "min key len")
        ("max-len", po::value<int>()->required(), "max key len")
        ("wei-fun", po::value<int>()->default_value(1), "weight function")
        ;


    po::variables_map vm;
    try { // check if all required parameters are passed
        po::store(po::command_line_parser(argc, argv).
                options(options_desc).run(), vm);
        po::notify(vm);
    } catch (...) { // if not - print usage
        usage(options_desc);
        exit(1);
    }


    std::string ifilename = vm["ifile"].as<std::string>();
    size_t block_size = vm["block_size"].as<size_t>();
    size_t factor = vm["factor"].as<size_t>();
    size_t min_len = vm["min_len"].as<size_t>();
    size_t max_len = vm["max_len"].as<size_t>();
    wei_fun_type wei_fun = WEIGHT_FUNCTIONS[vm["wei_fun"].as<std::string>()];;


    #ifdef DEBUG_OUTPUT
    {
        std::cout << "ifilename = <" << ifilename << ">" << std::endl;
        std::cout << "block_size = " << block_size << std::endl;
        std::cout << "factor = " << factor << std::endl;
        std::cout << "min_len = " << min_len << std::endl;
        std::cout << "max_len = " << max_len << std::endl;
        std::cout << "wei_fun = " << vm["wei_fun"].as<std::string>() << std::endl;
    }
    #endif


    std::cout << "Hello, World!" << std::endl;

    std::string text;
    std::ifstream ifile(ifilename);

    ifile.seekg(0, std::ios::end);
    text.reserve( (long) ifile.tellg());
    ifile.seekg(0, std::ios::beg);

    text.assign((std::istreambuf_iterator<char>(ifile)),
               std::istreambuf_iterator<char>());

    std::string s = compress_block(text, factor, min_len, max_len, wei_fun);

    #ifdef BUILD_WITH_EASY_PROFILER
        profiler::stopListen();
        profiler::dumpBlocksToFile("test_profile.prof");
    #endif

    return 0;
}