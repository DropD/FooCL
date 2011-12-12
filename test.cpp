#include "FooCL.hpp"

#include <iostream>
#include <sstream>

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
    
    return 0;
}
