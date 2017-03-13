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
			fprintf(stderr,"[createBuffer] context is not a valid context");
			break;
		case CL_INVALID_VALUE :
			fprintf(stderr,"[createBuffer] values specified in flags are not valid");
			break;
		case CL_INVALID_BUFFER_SIZE :
			fprintf(stderr,"[createBuffer] invalide size");
			break;
		case CL_INVALID_HOST_PTR :
			fprintf(stderr,"[createBuffer] invalid host pointer");
			break;
		case CL_MEM_OBJECT_ALLOCATION_FAILURE :
			fprintf(stderr,"[createBuffer] there is a failure to allocate memory for buffer object");
			break;
		case CL_OUT_OF_HOST_MEMORY :
			fprintf(stderr,"[createBuffer] there is a failure to allocate resources required by the OpenCL implementation on the host");
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
			fprintf(stderr,"[createProgram] context is not a valid context");
			break;
		case CL_INVALID_VALUE :
			fprintf(stderr,"[createProgram] values specified in flags are not valid");
			break;
		case CL_OUT_OF_HOST_MEMORY :
			fprintf(stderr,"[createProgram] there is a failure to allocate resources required by the OpenCL implementation on the host");
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
		fprintf(stderr,"[buildProgram] Error building program\n");
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
			fprintf(stderr,"[createKernel] invalid program");
			break;
		case CL_INVALID_PROGRAM_EXECUTABLE :
			fprintf(stderr,"[createKernel] program executable was not successfully built");
			break;
		case CL_INVALID_KERNEL_NAME :
			fprintf(stderr,"[createKernel] invalid kernel name");
			break;
		case CL_INVALID_KERNEL_DEFINITION :
			fprintf(stderr,"[createKernel] invalid kernel definition");
			break;
		case CL_INVALID_VALUE :
			fprintf(stderr,"[createKernel] values specified in flags are not valid");
			break;
		case CL_OUT_OF_HOST_MEMORY :
			fprintf(stderr,"[createKernel] there is a failure to allocate resources required by the OpenCL implementation on the host");
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
			fprintf(stderr,"[enqueueWriteBuffer] invalid command queue");
			break;
		case CL_INVALID_CONTEXT :
			fprintf(stderr,"[enqueueWriteBuffer] context is not a valid context");
			break;
		case CL_INVALID_MEM_OBJECT :
			fprintf(stderr,"[enqueueWriteBuffer] invalid buffer");
			break;
		case CL_INVALID_VALUE :
			fprintf(stderr,"[enqueueWriteBuffer] values specified in flags are not valid");
			break;
		case CL_INVALID_EVENT_WAIT_LIST :
			fprintf(stderr,"[enqueueWriteBuffer] invalid events wait list");
			break;
		case CL_MEM_OBJECT_ALLOCATION_FAILURE :
			fprintf(stderr,"[enqueueWriteBuffer] there is a failure to allocate memory for buffer object");
			break;
		case CL_OUT_OF_HOST_MEMORY :
			fprintf(stderr,"[enqueueWriteBuffer] there is a failure to allocate resources required by the OpenCL implementation on the host");
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
			fprintf(stderr,"[setKernelArg] invalid kernel");
			break;
		case CL_INVALID_ARG_INDEX :
			fprintf(stderr,"[setKernelArg] invalid index");
			break;
		case CL_INVALID_ARG_VALUE :
			fprintf(stderr,"[setKernelArg] if data is NULL for not declared __local");
			break;
		case CL_INVALID_MEM_OBJECT :
			fprintf(stderr,"[setKernelArg] invalid mem object");
			break;
		case CL_INVALID_SAMPLER :
			fprintf(stderr,"[setKernelArg] argument declared to be of type sampler_t when the specified arg_value is not a valid sampler object");
			break;
		case CL_INVALID_ARG_SIZE :
			fprintf(stderr,"[setKernelArg] invalid arg size");
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
			fprintf(stderr,"[enqueueNDRangeKernel] program executable was not successfully built");
			break;
		case CL_INVALID_COMMAND_QUEUE :
			fprintf(stderr,"[enqueueNDRangeKernel] invalid command queue");
			break;
		case CL_INVALID_KERNEL :
			fprintf(stderr,"[enqueueNDRangeKernel] invalid kernel");
			break;
		case CL_INVALID_CONTEXT :
			fprintf(stderr,"[enqueueNDRangeKernel] invalid context");
			break;
		case CL_INVALID_KERNEL_ARGS :
			fprintf(stderr,"[enqueueNDRangeKernel] kernel argument value is not specifie");
			break;
		case CL_INVALID_WORK_DIMENSION :
			fprintf(stderr,"[enqueueNDRangeKernel] invalid work dimension");
			break;
		case CL_INVALID_WORK_GROUP_SIZE :
			fprintf(stderr,"[enqueueNDRangeKernel] invalid group size");
			break;
		case CL_INVALID_WORK_ITEM_SIZE :
			fprintf(stderr,"[enqueueNDRangeKernel] invalid work item size");
			break;
		case CL_INVALID_GLOBAL_OFFSET :
			fprintf(stderr,"[enqueueNDRangeKernel] invalid global offset");
			break;
		case CL_OUT_OF_RESOURCES :
			fprintf(stderr,"[enqueueNDRangeKernel] out of resources");
			break;
		case CL_MEM_OBJECT_ALLOCATION_FAILURE :
			fprintf(stderr,"[enqueueNDRangeKernel] there is a failure to allocate memory for buffer object");
			break;
		case CL_INVALID_EVENT_WAIT_LIST :
			fprintf(stderr,"[enqueueNDRangeKernel] invalid events wait list");
			break;
		case CL_OUT_OF_HOST_MEMORY :
			fprintf(stderr,"[enqueueNDRangeKernel] there is a failure to allocate resources required by the OpenCL implementation on the host");
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
			fprintf(stderr,"[finish] invalid command queue");
			break;
		case CL_OUT_OF_HOST_MEMORY :
			fprintf(stderr,"[finish] there is a failure to allocate resources required by the OpenCL implementation on the host");
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
			fprintf(stderr,"[releaseKernel] invalid kernel");
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
			fprintf(stderr,"[releaseMemObject] invalid mem object");
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
			fprintf(stderr,"[releaseProgram] invalid program");
			break;
		}
		return -1;
	}
	return 1;
}

#endif
