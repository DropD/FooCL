#ifndef FOO_CL_HPP
#define FOO_CL_HPP

#include "cl.hpp";

namespace fcl
{
    class Environment
    {
        private:
        std::vector<cl::Platform>   platforms;
        std::vector<cl::Device>     devices;
        cl::Context                 context;
        cl::CommandQueue            queue;

        public:
        Environment()
        {
            cl::Platform::get(&platforms);
            platforms[0].getDevices(CL_DEVICE_TYPE_GPU, &devices);
            context = cl::Context(devices);
            queue = cl::CommandQueue(context, devices[0]);
        }
    };

    class KernelFunc
    {
        private:
        Environment env;
        std::stringstream       kernel_stream;
        std::string             kernel_src;
        cl::Program::Sources    source;
        cl::Program             program;
        cl::Kernel              kernel;

        public:
        KernelFunc(Environment env) : env(env)
        {
        }
    };
}

#endif
