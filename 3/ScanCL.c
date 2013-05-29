//
//  helloworld.c
//  ScanCL
//
//  Created by JIANNAN GUO on 27.05.13.
//  Copyright (c) 2013 Jiannan Guo. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <time.h>
#include <sys/time.h>
//#include <util.h>

#include <CL/opencl.h>

#define LENGTH (16)

const char *source =
/*
""
"\n__kernel void vec_add(__global int *c, __global int *a, __global int *b)"
"\n{"
"\n	int x = get_global_id(0);"
"\n	c[x] = a[x] + b[x];"
"\n}"
;*/
""
"\n__kernel void scan(__global int *output, __global int *input, __global int *gap)"
"\n{"
"\n int x = get_global_id(0);"
"\n int i = *gap;"
"\n if(x > (i-1)){"
"\n     output[x] = input[x] + input[x-i];"
"\n } else {"
"\n     output[x] = input[x];"
"\n }"
"\n}"
;

int main(int argc, const char * argv[])
{
    int i;
    cl_int status;
    cl_device_id mydevice;
    
    const int max_devices = 8;
    cl_platform_id platform;
    cl_device_id device[max_devices];
    cl_uint num_devices;
    
    /* configuration */
    clGetPlatformIDs(1, &platform, NULL);
    printf(" [OpenCL]\tPlatform ID is: %ld\n", (long)platform);
    clGetDeviceIDs(platform, CL_DEVICE_TYPE_GPU, max_devices, device, &num_devices);
    printf(" [OpenCL]\t%d device(s) found, with ID:\n", (uint8_t)num_devices);
    for (i=0; i<num_devices; i++) {
        printf(" [OpenCL]\t\t%d: %hd\n", i, (uint16_t)device[i]);
    }
    
    if (num_devices > max_devices) {
        num_devices = max_devices;
    }
    
    
    /* randomly pick up a device 
     pick the first device for test */
    mydevice = device[0];
    
    
    /* Setup context + command queue with selected device */
	cl_int errcode;
	cl_context ctx;
	cl_context_properties *ctx_prop;
	ctx_prop = (cl_context_properties *)calloc(3, sizeof(*ctx_prop));
	ctx_prop[0] = CL_CONTEXT_PLATFORM;
	ctx_prop[1] = (cl_context_properties)platform;
	ctx = clCreateContext(ctx_prop, 1, &mydevice, NULL, NULL, &errcode);
	assert(ctx != NULL);
    
    cl_command_queue cq;
    cq = clCreateCommandQueue(ctx, mydevice, 0, NULL);
    assert(cq != NULL);
    
    /* Compile and create kernal */
    cl_program program;
    program = clCreateProgramWithSource(ctx, 1, &source, NULL, NULL);
    
    const char *build_opts = NULL;
	status = clBuildProgram(program, 1, &mydevice, build_opts, NULL, NULL);
    if (status != CL_SUCCESS) {
        printf(" [OpenCL]\tBuild failed\n");
        //printBuildError(program, mydevice);
    }
	assert(status == CL_SUCCESS);
    printf(" [OpenCL]\tBuild successfully!\n");
    
	cl_kernel kernel = clCreateKernel(program, "scan", NULL);
	assert(kernel != NULL);
    
    /* Create memory objects */
	cl_mem input, output, gap;
	input = clCreateBuffer(ctx, CL_MEM_READ_ONLY, sizeof(cl_int) * LENGTH, NULL, NULL);
    output = clCreateBuffer(ctx, CL_MEM_WRITE_ONLY, sizeof(cl_int) * LENGTH, NULL, NULL);
    gap = clCreateBuffer(ctx, CL_MEM_READ_ONLY, sizeof(cl_int) * 1, NULL, NULL);
	assert(input != NULL);
    assert(output != NULL);
    assert(gap != NULL);
    
    /* Prepare data on host */
	cl_int *data = (cl_int *)malloc(sizeof(*data) * LENGTH);
    cl_int *data_back_up = (cl_int *)malloc(sizeof(*data_back_up) * LENGTH);
    //printf(" [OpenCL]\tData:\n");
    //printf(" [OpenCL]\t");
	for (i = 0; i < LENGTH; i++) {
		data[i] = rand()%64+1;
        data_back_up[i] = data[i];
        //printf("%d\t", data[i]);
	}
    printf("\n");
    
    /* get logn */
    int power;
    power = 2;
    int log = 1;
    while (power < LENGTH) {
        power = power * 2;
        log++;
    }
    printf(" [OpenCL]\tLog(LENGTH) = %d steps\n", log);
    
    /* Do SCAN in logn steps */
    struct timeval start, end, start1, end1;
    gettimeofday(&start, NULL);
    power = 1;
    int j;
    for (i = 0; i < log; i++) {
        /* write buffer */
        cl_int *temp = &power;
        clEnqueueWriteBuffer(cq, input, CL_TRUE, 0, LENGTH * sizeof(*data), data, 0, NULL, NULL);
        clEnqueueWriteBuffer(cq, gap, CL_TRUE, 0, sizeof(cl_int), temp, 0, NULL, NULL);
        /* Bind kernel arguments + execute kernel */
        clSetKernelArg(kernel, 0, sizeof(cl_mem), &output);
        clSetKernelArg(kernel, 1, sizeof(cl_mem), &input);
        clSetKernelArg(kernel, 2, sizeof(cl_mem), &gap);
        size_t global_work_sizes[] = { LENGTH };
        const size_t local_work_sizes[] = { 2 };
        status = clEnqueueNDRangeKernel(cq, kernel, 1, NULL, global_work_sizes, local_work_sizes, 0, NULL, NULL);
        assert(status == CL_SUCCESS);
        clFinish(cq);
        power = power << 1;
        printf(" [OpenCL]\tPower is %d", power);
        /* Read back */
        clEnqueueReadBuffer(cq, output, CL_TRUE, 0, LENGTH * sizeof(*data), data, 0, NULL, NULL);
        printf(" [OpenCL]\tAfter step %d, result is:\n", i);
        printf(" [OpenCL]\t");
        for (j = 0; j < LENGTH; j++) {
            printf("%d\t", data[j]);
        }
        printf("\n");
    }
    
    gettimeofday(&end, NULL);
    long exectime = (end.tv_sec - start.tv_sec)*1000000 + end.tv_usec - start.tv_usec;
    printf(" [HOST]\t\tTime taken by the kernel to execute is %ld usec\n", exectime);
    
    gettimeofday(&start1, NULL);
    for (i = 1; i < LENGTH; i++) {
        data_back_up[i] += data_back_up[i-1];
        if (data_back_up[i] != data[i]) {
            printf(" [HOST]\t\tVerification failed!\n");
            break;
        }
    }

    if (i == LENGTH)
        printf(" [HOST]\t\tVerification successful!\n");

    gettimeofday(&end1, NULL);
    exectime = (end1.tv_sec - start1.tv_sec)*1000000 + end1.tv_usec - start1.tv_usec;
    printf(" [HOST]\t\tTime taken for sequential execution is %ld usec\n", exectime);

    /* Free everything */
    clReleaseMemObject(input);
    clReleaseMemObject(output);
    clReleaseMemObject(gap);
    clReleaseKernel(kernel);
    clReleaseProgram(program);
    clReleaseCommandQueue(cq);
    clReleaseContext(ctx);

    return 0;
}
