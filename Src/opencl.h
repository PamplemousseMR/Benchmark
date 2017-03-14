#ifndef __OPENCL_
#define __OPENCL__
/*!
 * \file opencl.h
 * \param Fichier surchargeant les fonctions de OpenCL permettant d'englober la gestion des erreurs.
 * \author Appert Kevin
 * \author Hun Tony
 * \author Lataix Maxime
 * \author Manciaux Romain
 * \author Peccard Remi
 */

/*	=============== Includes ===============	*/

#include "CL/cl.h"	/*	openCL	*/

/*	=============== Functions ===============	*/

static __inline int createBuffer(cl_context*, cl_mem_flags, size_t, void*, cl_mem*);
static __inline int createProgram(cl_context*,cl_uint, const char*,const size_t*, cl_program*);
static __inline int buildProgram(cl_program* program,cl_context*, cl_uint,cl_device_id*, const char*, void(*)(cl_program, void*),void*);
static __inline int createKernel(cl_program*, const char*, cl_kernel*);
static __inline int enqueueWriteBuffer(cl_command_queue*, cl_mem*,cl_bool,size_t,size_t, const void*, cl_uint, const cl_event*, cl_event*);
static __inline int setKernelArg(cl_kernel*, cl_uint, size_t, void*);
static __inline int enqueueNDRangeKernel(cl_command_queue*, cl_kernel*, cl_uint, const size_t*, const size_t*, const size_t*, cl_uint, const cl_event*, cl_event*);
static __inline int finish(cl_command_queue*);
static __inline int releaseKernel(cl_kernel*);
static __inline int releaseMemObject(cl_mem*);
static __inline int releaseProgram(cl_program*);
static __inline int getPlatformIDs(cl_uint, cl_platform_id*, cl_uint*);
static __inline int getDeviceIDs(cl_platform_id*,cl_device_type,cl_uint,cl_device_id*, cl_uint*);
static __inline int createContext(const cl_context_properties*, cl_uint,const cl_device_id*,void(*)(const char*, const void*, size_t, void*), void*, cl_context*);
static __inline int createCommandQueue(cl_context*, cl_device_id* , cl_command_queue_properties, cl_command_queue*);
static __inline unsigned int getMaxWorkItem(cl_device_id* );
static __inline size_t getMaxItemByGroup(cl_device_id*);

/*	=============== Inline definitions ===============	*/

static __inline int createBuffer(cl_context* context, cl_mem_flags flag, size_t size, void* ptr, cl_mem* res)
{
	cl_int err;
	*res = clCreateBuffer(*context, flag, size, ptr, &err);
	if(err != CL_SUCCESS)
	{
		switch(err)
		{
		case CL_INVALID_CONTEXT :
            fprintf(stderr,"[createBuffer] context is not a valid context\n");
			break;
		case CL_INVALID_VALUE :
            fprintf(stderr,"[createBuffer] values specified in flags are not valid\n");
			break;
		case CL_INVALID_BUFFER_SIZE :
            fprintf(stderr,"[createBuffer] invalide size\n");
			break;
		case CL_INVALID_HOST_PTR :
            fprintf(stderr,"[createBuffer] invalid host pointer\n");
			break;
		case CL_MEM_OBJECT_ALLOCATION_FAILURE :
            fprintf(stderr,"[createBuffer] there is a failure to allocate memory for buffer object\n");
			break;
		case CL_OUT_OF_HOST_MEMORY :
            fprintf(stderr,"[createBuffer] there is a failure to allocate resources required by the OpenCL implementation on the host\n");
			break;
		}
		return -1;
	}
	return 1;
}

static __inline int createProgram(cl_context* context,cl_uint i, const char* prog,const size_t* size, cl_program* program)
{
	cl_int err;
	*program = clCreateProgramWithSource(*context,i,&prog, size, &err);
	if(err != CL_SUCCESS)
	{
		switch(err)
		{
		case CL_INVALID_CONTEXT :
            fprintf(stderr,"[createProgram] context is not a valid context\n");
			break;
		case CL_INVALID_VALUE :
            fprintf(stderr,"[createProgram] values specified in flags are not valid\n");
			break;
		case CL_OUT_OF_HOST_MEMORY :
            fprintf(stderr,"[createProgram] there is a failure to allocate resources required by the OpenCL implementation on the host\n");
			break;
		}
		return -1;
	}
	return 1;
}

static __inline int buildProgram(cl_program* program, cl_context* context, cl_uint numDevices, cl_device_id* devices, const char* options, void(*notify)(cl_program, void*), void *data)
{
	if (clBuildProgram(*program, numDevices, devices, options, notify, data) != CL_SUCCESS)
	{
        fprintf(stderr,"[buildProgram] Error building program\n\n");
		size_t len = 0;
		cl_device_id deviceId;
		clGetContextInfo(*context,CL_CONTEXT_DEVICES,sizeof(cl_device_id), &deviceId, NULL);
		clGetProgramBuildInfo(*program, deviceId, CL_PROGRAM_BUILD_LOG, 0, NULL, &len);
		char *buffer = calloc(len, sizeof(char));
		clGetProgramBuildInfo(*program, deviceId, CL_PROGRAM_BUILD_LOG, len, buffer, NULL);
		printf("%s",buffer);
		free(buffer);
		return -1;
	}
	return 1;
}

static __inline int createKernel(cl_program* program, const char* name, cl_kernel* kernel)
{
	cl_int err;
	*kernel = clCreateKernel(*program, name, &err);
	if(err != CL_SUCCESS)
	{
		switch(err)
		{
		case CL_INVALID_PROGRAM :
            fprintf(stderr,"[createKernel] invalid program\n");
			break;
		case CL_INVALID_PROGRAM_EXECUTABLE :
            fprintf(stderr,"[createKernel] program executable was not successfully built\n");
			break;
		case CL_INVALID_KERNEL_NAME :
            fprintf(stderr,"[createKernel] invalid kernel name\n");
			break;
		case CL_INVALID_KERNEL_DEFINITION :
            fprintf(stderr,"[createKernel] invalid kernel definition\n");
			break;
		case CL_INVALID_VALUE :
            fprintf(stderr,"[createKernel] values specified in flags are not valid\n");
			break;
		case CL_OUT_OF_HOST_MEMORY :
            fprintf(stderr,"[createKernel] there is a failure to allocate resources required by the OpenCL implementation on the host\n");
			break;
		}
		return -1;
	}
	return 1;
}

static __inline int enqueueWriteBuffer(cl_command_queue* command, cl_mem* mem, cl_bool blocking, size_t offset, size_t cb, const void *ptr, cl_uint num_events, const cl_event* events, cl_event* event)
{
	cl_int err = clEnqueueWriteBuffer(*command, *mem, blocking, offset, cb, ptr, num_events, events, event);
	if(err != CL_SUCCESS)
	{
		switch(err)
		{
		case CL_INVALID_COMMAND_QUEUE :
            fprintf(stderr,"[enqueueWriteBuffer] invalid command queue\n");
			break;
		case CL_INVALID_CONTEXT :
            fprintf(stderr,"[enqueueWriteBuffer] context is not a valid context\n");
			break;
		case CL_INVALID_MEM_OBJECT :
            fprintf(stderr,"[enqueueWriteBuffer] invalid buffer\n");
			break;
		case CL_INVALID_VALUE :
            fprintf(stderr,"[enqueueWriteBuffer] values specified in flags are not valid\n");
			break;
		case CL_INVALID_EVENT_WAIT_LIST :
            fprintf(stderr,"[enqueueWriteBuffer] invalid events wait list\n");
			break;
		case CL_MEM_OBJECT_ALLOCATION_FAILURE :
            fprintf(stderr,"[enqueueWriteBuffer] there is a failure to allocate memory for buffer object\n");
			break;
		case CL_OUT_OF_HOST_MEMORY :
            fprintf(stderr,"[enqueueWriteBuffer] there is a failure to allocate resources required by the OpenCL implementation on the host\n");
			break;
		}
		return -1;
	}
	return 1;
}

static __inline int setKernelArg(cl_kernel* kernel, cl_uint num, size_t size, void *data)
{
	cl_int err = clSetKernelArg(*kernel,num,size,data);
	if(err != CL_SUCCESS)
	{
		switch(err)
		{
		case CL_INVALID_KERNEL :
            fprintf(stderr,"[setKernelArg] invalid kernel\n");
			break;
		case CL_INVALID_ARG_INDEX :
            fprintf(stderr,"[setKernelArg] invalid index\n");
			break;
		case CL_INVALID_ARG_VALUE :
            fprintf(stderr,"[setKernelArg] if data is NULL for not declared __local\n");
			break;
		case CL_INVALID_MEM_OBJECT :
            fprintf(stderr,"[setKernelArg] invalid mem object\n");
			break;
		case CL_INVALID_SAMPLER :
            fprintf(stderr,"[setKernelArg] argument declared to be of type sampler_t when the specified arg_value is not a valid sampler object\n");
			break;
		case CL_INVALID_ARG_SIZE :
            fprintf(stderr,"[setKernelArg] invalid arg size\n");
			break;
		}
		return -1;
	}
	return 1;
}

static __inline int enqueueNDRangeKernel(cl_command_queue* command, cl_kernel* kernel, cl_uint work_dim, const size_t* work_offset, const size_t* global_work_size, const size_t* local_work_size, cl_uint num_event, const cl_event* list, cl_event* event)
{
	cl_int err = clEnqueueNDRangeKernel(*command,*kernel,work_dim,work_offset,global_work_size,local_work_size,num_event,list,event);
	if(err != CL_SUCCESS)
	{
		switch(err)
		{
		case CL_INVALID_PROGRAM_EXECUTABLE :
            fprintf(stderr,"[enqueueNDRangeKernel] program executable was not successfully built\n");
			break;
		case CL_INVALID_COMMAND_QUEUE :
            fprintf(stderr,"[enqueueNDRangeKernel] invalid command queue\n");
			break;
		case CL_INVALID_KERNEL :
            fprintf(stderr,"[enqueueNDRangeKernel] invalid kernel\n");
			break;
		case CL_INVALID_CONTEXT :
            fprintf(stderr,"[enqueueNDRangeKernel] invalid context\n");
			break;
		case CL_INVALID_KERNEL_ARGS :
            fprintf(stderr,"[enqueueNDRangeKernel] kernel argument value is not specifie\n");
			break;
		case CL_INVALID_WORK_DIMENSION :
            fprintf(stderr,"[enqueueNDRangeKernel] invalid work dimension\n");
			break;
		case CL_INVALID_WORK_GROUP_SIZE :
            fprintf(stderr,"[enqueueNDRangeKernel] invalid group size\n");
			break;
		case CL_INVALID_WORK_ITEM_SIZE :
            fprintf(stderr,"[enqueueNDRangeKernel] invalid work item size\n");
			break;
		case CL_INVALID_GLOBAL_OFFSET :
            fprintf(stderr,"[enqueueNDRangeKernel] invalid global offset\n");
			break;
		case CL_OUT_OF_RESOURCES :
            fprintf(stderr,"[enqueueNDRangeKernel] out of resources\n");
			break;
		case CL_MEM_OBJECT_ALLOCATION_FAILURE :
            fprintf(stderr,"[enqueueNDRangeKernel] there is a failure to allocate memory for buffer object\n");
			break;
		case CL_INVALID_EVENT_WAIT_LIST :
            fprintf(stderr,"[enqueueNDRangeKernel] invalid events wait list\n");
			break;
		case CL_OUT_OF_HOST_MEMORY :
            fprintf(stderr,"[enqueueNDRangeKernel] there is a failure to allocate resources required by the OpenCL implementation on the host\n");
			break;
		}
		return -1;
	}
	return 1;
}

static __inline int finish(cl_command_queue* command)
{
	cl_int err = clFinish(*command);
	if(err != CL_SUCCESS)
	{
		switch(err)
		{
		case CL_INVALID_COMMAND_QUEUE :
            fprintf(stderr,"[finish] invalid command queue\n");
			break;
		case CL_OUT_OF_HOST_MEMORY :
            fprintf(stderr,"[finish] there is a failure to allocate resources required by the OpenCL implementation on the host\n");
			break;
		}
		return -1;
	}
	return 1;
}

static __inline int releaseKernel(cl_kernel* kernel)
{
	cl_int err = clReleaseKernel(*kernel);
	if(err != CL_SUCCESS)
	{
		switch(err)
		{
		case CL_INVALID_KERNEL :
            fprintf(stderr,"[releaseKernel] invalid kernel\n");
			break;
		}
		return -1;
	}
	return 1;
}

static __inline int releaseMemObject(cl_mem* mem)
{
	cl_int err = clReleaseMemObject(*mem);
	if(err != CL_SUCCESS)
	{
		switch(err)
		{
		case CL_INVALID_MEM_OBJECT :
            fprintf(stderr,"[releaseMemObject] invalid mem object\n");
			break;
		}
		return -1;
	}
	return 1;
}

static __inline int releaseProgram(cl_program* program)
{
	cl_int err = clReleaseProgram(*program);
	if(err != CL_SUCCESS)
	{
		switch(err)
		{
		case CL_INVALID_PROGRAM :
            fprintf(stderr,"[releaseProgram] invalid program\n");
			break;
		}
		return -1;
	}
	return 1;
}

static __inline int getPlatformIDs(cl_uint entries, cl_platform_id* platform, cl_uint* num_platforms)
{
    cl_int err = clGetPlatformIDs(entries,platform,num_platforms);
    if(err != CL_SUCCESS)
    {
        switch(err)
        {
        case CL_INVALID_VALUE  :
            fprintf(stderr,"[getPlatformIDs] invalid values\n");
            break;
        }
        return -1;
    }
    return 1;
}

static __inline int getDeviceIDs(cl_platform_id* platform,cl_device_type type,cl_uint entries,cl_device_id* devices, cl_uint* num_device)
{
    cl_int err = clGetDeviceIDs(*platform, type, entries, devices, num_device);
    if(err != CL_SUCCESS)
    {
        switch(err)
        {
        case CL_INVALID_PLATFORM  :
            fprintf(stderr,"[getDevicesIds] invalid platform\n");
            break;
        case CL_INVALID_DEVICE_TYPE  :
            fprintf(stderr,"[getDevicesIds] invalid device type\n");
            break;
        case CL_INVALID_VALUE :
            fprintf(stderr,"[getDevicesIds] invalid value\n");
            break;
        case CL_DEVICE_NOT_FOUND  :
            fprintf(stderr,"[getDevicesIds] device not found\n");
            break;
        }
        return -1;
    }
    return 1;
}

static __inline int createContext(const cl_context_properties* properties, cl_uint num_devices,const cl_device_id* devices,void(*pfn_notify)(const char*, const void*, size_t, void*), void* user_data, cl_context* context)
{
    cl_int err;
    *context = clCreateContext(properties,num_devices,devices,pfn_notify, user_data, &err);
    if(err != CL_SUCCESS)
    {
        switch(err)
        {
        case CL_INVALID_PLATFORM  :
            fprintf(stderr,"[createContext] invalid platform\n");
            break;
        case CL_INVALID_DEVICE  :
            fprintf(stderr,"[createContext] invalid device\n");
            break;
        case CL_INVALID_VALUE :
            fprintf(stderr,"[createContext] invalid value\n");
            break;
        case CL_DEVICE_NOT_AVAILABLE  :
            fprintf(stderr,"[createContext] device not available\n");
            break;
        case CL_OUT_OF_HOST_MEMORY  :
            fprintf(stderr,"[createContext] there is a failure to allocate resources required by the OpenCL implementation on the host\n");
            break;
        }
        return -1;
    }
    return 1;
}

static __inline int createCommandQueue(cl_context* context, cl_device_id* device,cl_command_queue_properties properties, cl_command_queue* command)
{
    cl_int err;
    #ifdef _WIN32
    *command = clCreateCommandQueue(*context, *device, properties, &err);
    #else
    *command = clCreateCommandQueueWithProperties(*context, *device, &properties, &err);
    #endif
    if(err != CL_SUCCESS)
    {
        switch(err)
        {
        case CL_INVALID_CONTEXT  :
            fprintf(stderr,"[createCommandQueue] invalid context\n");
            break;
        case CL_INVALID_DEVICE  :
            fprintf(stderr,"[createCommandQueue] invalid device\n");
            break;
        case CL_INVALID_VALUE :
            fprintf(stderr,"[createCommandQueue] invalid value\n");
            break;
        case CL_INVALID_QUEUE_PROPERTIES  :
            fprintf(stderr,"[createCommandQueue] invalid queue properties\n");
            break;
        case CL_OUT_OF_HOST_MEMORY  :
            fprintf(stderr,"[createCommandQueue] there is a failure to allocate resources required by the OpenCL implementation on the host\n");
            break;
        }
        return -1;
    }
    return 1;
}

static __inline unsigned int getMaxWorkItem(cl_device_id* device)
{
	unsigned int i;
	cl_uint dim = 0;
	clGetDeviceInfo(*device, CL_DEVICE_MAX_WORK_ITEM_DIMENSIONS,sizeof(cl_uint), &dim, NULL);

    size_t item[3];
    clGetDeviceInfo(*device, CL_DEVICE_MAX_WORK_ITEM_SIZES	,sizeof(size_t)*3, &item, NULL);

	for(i=1 ; i<dim ; ++i)
		item[0] *= item[i];

    dim = (cl_uint)item[0];
	return dim;
}

static __inline size_t getMaxItemByGroup(cl_device_id* device)
{
	size_t si = 0;
	clGetDeviceInfo(*device, CL_DEVICE_MAX_WORK_GROUP_SIZE,sizeof(size_t), &si, NULL);
	return si;
}

#endif
