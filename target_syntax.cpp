#include "FooCL.hpp"
int main (int argc, char const* argv[])
{
    fcl::Environment env;

    fcl::KernelFunc kernel(env);

    kernel
        << "__kernel void("                           << std::endl 
        << "    __global float4* in,"                 << std::endl 
        << "    __global float4* out,"                << std::endl 
        << "    __global float inout,"                << std::endl 
        << "    __const float param)"                 << std::endl 
        << "{"                                        << std::endl 
        << "    int tid = get_global_id(0);"          << std::endl
        << "    out[tid].x = inout[tid] * in[tid].x;" << std::endl
        << "    out[tid].y = inout[tid] * in[tid].y;" << std::endl
        << "    out[tid].z = inout[tid] * in[tid].z;" << std::endl
        << "    out[tid].w = inout[tid] * in[tid].w;" << std::endl
        << ""                                         << std::endl 
        << "    inout[tid] += param;"                 << std::endl 
        << "}"                                        << std::endl; 

    fcl::vector<float> in(1024, 1.0);
    fcl::vector<float> out(1024);
    fcl::vector<float> inout(256);

    for(int i=0; i<100; ++i)
    {
        kernel(in, out, inout, 0.1);
    }

    out.pull(); //oder out.read_back();

    std::cout << out[0] << " == " << 9.9 << " ?" << std::endl;
        
    return 0;
}

