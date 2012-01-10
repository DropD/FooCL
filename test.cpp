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
        << "__kernel void test_kernel("         << "\n"
        << "    void"                           << "\n"
        << ")"                                  << "\n"
        << "{"                                  << "\n"
        << "    int tid = get_global_id(0);"    << "\n"
        << "}"                                  << "\n";

    kernel.build();

    std::cout << "kernel: " << std::endl << kernel <<  std::endl;

    {
        fcl::ExpKernel kernel2(env, "exp_kernel");
        kernel2
            [ "__kernel void exp_kernel(" ]
            [ ")" ]
            [ "{" ]
            [ "    int bla = 5;"]
            [ "}" ];
        std::cout << "kernel2: " << std::endl << kernel2 << std::endl;
    }

    std::complex<double> c;
    std::vector<double> v;
    unsigned result;

    fcl::roman<std::string::const_iterator> roman_parser;

    std::string str("1, 2, 3, 4, 5");
    std::string str2("(1.5, 6.4)");
    std::string str3("MCMLXXXVIII");

    //bool r = parse(str3.begin(), str3.end(), roman_parser, result)

    if(fcl::parse_numbers(str.begin(), str.end()) &&
       fcl::parse_complex(str2.begin(), str2.end(), c) &&
       fcl::parse_list(str.begin(), str.end(), v) &&
       fcl::parse_roman(str3.begin(), str3.end(), result))
    {
        std::cout << "Parsing succeeded" << std::endl;
        std::cout << str << " Parses OK" << std::endl;
        std::cout << str2 << " Parses OK --> c = " << c << std::endl;
        std::cout << "List parser OK" << std::endl;
        std::cout << str3 << " Parses OK --> result = " << result << std::endl;
    }
    else
    {
        std::cout << "Parsing failed" << std::endl;
    }
    
    return 0;
}
