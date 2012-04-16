#ifndef OCL_ERROR_HPP
#define OCL_ERROR_HPP

//#include <OpenCL/opencl.h>
#include <string>

std::string get_error_msg(int err)
{
    switch (err)
    {
        //createBuffer
        case CL_INVALID_CONTEXT:
            return "invalid context.";
        case CL_INVALID_VALUE:
            return "invalid flag value.";
        case CL_INVALID_BUFFER_SIZE:
            return "invalid buffer size.";
        case CL_INVALID_HOST_PTR:
            return "invalid host pointer.";
        case CL_MEM_OBJECT_ALLOCATION_FAILURE:
            return "failed allocating memory.";
        case CL_OUT_OF_HOST_MEMORY:
            return "out of host memory.";

        //buildProgram
        case CL_INVALID_PROGRAM:
            return "invalid program.";
        case CL_INVALID_DEVICE:
            return "invalid device.";
        case CL_INVALID_BINARY:
            return "invalid binary.";
        case CL_INVALID_BUILD_OPTIONS:
            return "invalid build options.";
        case CL_INVALID_OPERATION:
            return "invalid operation.";
        case CL_COMPILER_NOT_AVAILABLE:
            return "compiler not available.";
        case CL_BUILD_PROGRAM_FAILURE:
            return "build program failure.";

        //enqueueNDRangeKernel
        case CL_INVALID_PROGRAM_EXECUTABLE:
            return "invalid program executable.";
        case CL_INVALID_COMMAND_QUEUE:
            return "invalid command queue.";
        case CL_INVALID_KERNEL:
            return "invalid kernel.";
        case CL_INVALID_KERNEL_ARGS:
            return "invalid kernel arguments.";
        case CL_INVALID_WORK_DIMENSION:
            return "invalid work dimension.";
        case CL_INVALID_WORK_GROUP_SIZE:
            return "invalid work group size.";
        case CL_INVALID_WORK_ITEM_SIZE:
            return "invalid work item size, exceeds max number of work-items.";
        case CL_INVALID_GLOBAL_OFFSET:
            return "global_work_offset must be NULL.";
        case CL_OUT_OF_RESOURCES:
            return "out of resources.";
        case CL_INVALID_EVENT_WAIT_LIST:
            return "invalid event wait list.";

        //createKernel
        case CL_INVALID_KERNEL_NAME:
            return "invalid kernel name.";
        case CL_INVALID_KERNEL_DEFINITION:
            return "invalid kernel definition";

        default:
            return "unknown error.";
    }
}
#endif

