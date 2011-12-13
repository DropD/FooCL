#include "FooCL.hpp"
#include "parser.hpp"

#include <iostream>
#include <sstream>
#include <string>

int main (int argc, char const* argv[])
{
    fcl::Environment env;

    fcl::KernelFunc kernel(env, "test_kernel");

    kernel
        << "__kernel void test_kernel(void"     << std::endl
        << ")"                                  << std::endl
        << "{"                                  << std::endl
        << "    int tid = get_global_id(0);"    << std::endl
        << "}"                                  << std::endl;

    kernel.build();

    std::string str("1, 2, 3, 4, 5");
    if(fcl::parse_numbers(str.begin(), str.end()))
    {
        std::cout << "Parsing succeeded" << std::endl;
        std::cout << str << " Parses OK" << std::endl;
    }
    else
    {
        std::cout << "Parsing failed" << std::endl;
    }
    
    return 0;
}
