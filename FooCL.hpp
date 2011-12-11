#ifndef FOO_CL_HPP
#define FOO_CL_HPP

#include "cl.hpp"
#include "fcl_error.hpp"


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
        
        cl::Context get_ctx()
        {
            return context;
        }

        std::vector<cl::Device> get_dev()
        {
            return devices;
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

        void build()
        {
            try
            {
                kernel_src = kernel_stream.str();
                source = cl::Program::Sources(1, std::make_pair(kernel_src.c_str(), kernel_src.size()));
                program = cl::Program(env.get_ctx(), source);
                program.build(ctx.get_dev());
                kernel = cl::Kernel(program, kernel_name);
            }
            catch (cl::Error err)
            {
                std::cerr
                    << "ERROR: "
                    << err.what()
                    << " [" << get_error_msg(err.err()) << "]" << std::endl;
            }
        }
    };
}

#endif
