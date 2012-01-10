#ifndef FOO_CL_HPP
#define FOO_CL_HPP

#include <boost/spirit/include/qi.hpp>

#include "cl.hpp"
#include "fcl_error.hpp"

#include <iostream>
#include <sstream>


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
        std::string             kernel_name;
        std::string             build_log;

        public:
        KernelFunc(Environment env, std::string kname) : env(env), kernel_name(kname)
        {
        }

        template <class T>
        friend std::stringstream& operator<< (KernelFunc& kern, const T inp);

        template <class os_type>
        friend os_type& operator<< (os_type &os, KernelFunc &kern);

        void build()
        {
            try
            {
                kernel_src = kernel_stream.str();
                source = cl::Program::Sources(1, std::make_pair(kernel_src.c_str(), kernel_src.size()));
                program = cl::Program(env.get_ctx(), source);
                program.build(env.get_dev());
                program.getBuildInfo(env.get_dev()[0], CL_PROGRAM_BUILD_LOG, &build_log);
                kernel = cl::Kernel(program, kernel_name.c_str());
            }
            catch (cl::Error err)
            {
                std::cerr
                    << "ERROR: "
                    << err.what()
                    << " [" << get_error_msg(err.err()) << "]" << std::endl
                    << " Build log (if any) follows:" << std::endl
                    << build_log << std::endl;
            }
        }
    };

    template <class T>
    std::stringstream& operator<< (KernelFunc &kern, const T inp)
    {
        kern.kernel_stream << inp;
        return kern.kernel_stream;
    }

    template <class os_type>
    os_type& operator<< (os_type &os, KernelFunc &kern)
    {
        os << kern.kernel_stream.str();
        return os;
    }
}

#endif
