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

#include <stdio.h>
#include "CL/cl.h"	/*	openCL	*/

/*	=============== Functions ===============	*/

int createBuffer(cl_context*, cl_mem_flags, size_t, void*, cl_mem*);
int createProgram(cl_context*,cl_uint, const char*,const size_t*, cl_program*);
int buildProgram(cl_program* program,cl_context*, cl_uint,cl_device_id*, const char*, void(*)(cl_program, void*),void*);
int createKernel(cl_program*, const char*, cl_kernel*);
int enqueueWriteBuffer(cl_command_queue*, cl_mem*,cl_bool,size_t,size_t, const void*, cl_uint, const cl_event*, cl_event*);
int setKernelArg(cl_kernel*, cl_uint, size_t, void*);
int enqueueNDRangeKernel(cl_command_queue*, cl_kernel*, cl_uint, const size_t*, const size_t*, const size_t*, cl_uint, const cl_event*, cl_event*);
int finish(cl_command_queue*);
int releaseKernel(cl_kernel*);
int releaseMemObject(cl_mem*);
int releaseProgram(cl_program*);
int getPlatformIDs(cl_uint, cl_platform_id*, cl_uint*);
int getDeviceIDs(cl_platform_id*,cl_device_type,cl_uint,cl_device_id*, cl_uint*);
int createContext(const cl_context_properties*, cl_uint,const cl_device_id*,void(*)(const char*, const void*, size_t, void*), void*, cl_context*);
int createCommandQueue(cl_context*, cl_device_id* , cl_command_queue_properties, cl_command_queue*);
unsigned int getMaxWorkItem(cl_device_id* );
size_t getMaxItemByGroup(cl_device_id*);
int releaseCommandQueue(cl_command_queue*);
int releaseContext(cl_context*);

#endif
