//
//  main.c
//  CLCommon
//
//  Created by NATURE on 11/9/2016.
//  Copyright © 2016 NATURE. All rights reserved.
//
#ifdef __linux__
#include <string.h>
#endif
#ifdef _WIN32
#define _CRT_SECURE_NO_WARNINGS
#endif
#ifdef __APPLE__
#include <OpenCL/opencl.h>
#else
#include<CL/cl.h>
#endif
#include <stdio.h>
#include<iostream>
#include <stdlib.h>
#ifdef _WIN32
#include <conio.h>
#include <direct.h>
#endif

#ifdef _WIN32//check pwd for windows
void PrintFullPath(char * partialPath)
{
    char full[_MAX_PATH];
    if (_fullpath(full, partialPath, _MAX_PATH) != NULL)
        printf("Full path is: %s\n", full);
    else
        printf("Invalid path\n");
}
#endif
//check error
void ckE(int status,int line){
    if(status!=CL_SUCCESS){
        printf("Line[%d] Error happens with code %d\n",line,status);
        exit(1);
    }
}
//file to char*
char* readFile(char* filePath){
    char* content=NULL;
    FILE* f = fopen(filePath, "r");
    if(f==NULL){
        printf("File %s does not exist\n",filePath);
        return content;
    }
    content=new char[10000];
    memset(content,0,10000);
    int lenSize=256;
    char* tmpC = new char[lenSize];
    while(fgets(tmpC,lenSize,f)){
        strcat(content, tmpC);
    }
    return content;
}

void iniArray(int size, float* array){
    for(int i=0;i<size;i++){
        array[i]=rand()%(i+1);
    }
}

int chosenPlatform=0;
int chosenDevice=0;


int main(int argc, const char * argv[]) {
#ifdef _WIN32
    printf("Current work path is \n");
    PrintFullPath(".\\");
#endif
    if(argc==5){
        if(!strcmp(argv[1],"-P")){
            chosenPlatform=atoi(argv[2]);
        }else{
            printf("args not set, value is default for platforms\n");
        }
        if(!strcmp(argv[3],"-D")){
            chosenDevice=atoi(argv[4]);
        }else{
            printf("args not set, value is default for devices\n");
        }
    }else{
        printf("args not in proper form, using default value\n");
    }
    
    cl_uint numPlatforms;
    cl_uint numDevices;
    cl_int status;
    cl_platform_id* platforms;
    cl_device_id* devices;
    cl_context context;
    
    
    
    
    status=clGetPlatformIDs(0, NULL, &numPlatforms);
    ckE(status,__LINE__);
    platforms=(cl_platform_id*)malloc(sizeof(cl_platform_id)*numPlatforms);
    status =clGetPlatformIDs(numPlatforms,platforms,NULL);
    ckE(status, __LINE__);
    printf("There are %d platforms and platform %d is chosen\n",numPlatforms,chosenPlatform);
    cl_platform_id this_platform = platforms[chosenPlatform];
    
    status=clGetDeviceIDs(this_platform, CL_DEVICE_TYPE_ALL, 0, NULL, &numDevices);
    ckE(status, __LINE__);
    devices = (cl_device_id*)malloc(sizeof(cl_device_id)*numDevices);
    status=clGetDeviceIDs(this_platform, CL_DEVICE_TYPE_ALL, numDevices, devices, NULL);
    ckE(status, __LINE__);
    printf("Thera are %d devices on chosen paltform and divice id is %d\n",numDevices,chosenDevice);
    cl_device_id this_device = devices[chosenDevice];
    
    context=clCreateContext(NULL, 1, &this_device, NULL, NULL, &status);
    ckE(status, __LINE__);
    const char* programSource = readFile((char*)"Kernel1.cl");
    cl_program program=clCreateProgramWithSource(context, 1, &programSource, NULL, &status);
    ckE(status, __LINE__);
    status = clBuildProgram(program, 1, &this_device, NULL, NULL, NULL);
    if(status!=CL_SUCCESS){
        size_t size;
        status=clGetProgramBuildInfo(program, this_device, CL_PROGRAM_BUILD_LOG, 0, NULL, &size);
        ckE(status, __LINE__);
        char* info=new char[size+1];
        status=clGetProgramBuildInfo(program, this_device, CL_PROGRAM_BUILD_LOG, size, info, NULL);
        printf("build program info %s\n",info);
        delete info;
        exit(0);
    }
    
    cl_command_queue cmdQueueA=clCreateCommandQueue(context, this_device, CL_QUEUE_PROFILING_ENABLE, &status);
    ckE(status, __LINE__);
    
    cl_kernel simpleKernel = clCreateKernel(program, (char*)"VectorAdd", &status);
    ckE(status, __LINE__);
    
    
    
    /*data & buffer*/
    int numThread=256;
    float* A=new float[numThread];
    float* B=new float[numThread];
    iniArray(numThread, A);
    cl_mem memA=clCreateBuffer(context, CL_MEM_READ_ONLY, numThread*sizeof(cl_float), NULL, &status);
    ckE(status, __LINE__);
    cl_mem memB=clCreateBuffer(context,CL_MEM_WRITE_ONLY,numThread*sizeof(cl_float),NULL,&status);
    ckE(status, __LINE__);
    
    
    status=clEnqueueWriteBuffer(cmdQueueA, memA, CL_FALSE, 0, numThread*sizeof(cl_float), A, 0, NULL, NULL);
    ckE(status, __LINE__);
    
    
    status=clSetKernelArg(simpleKernel, 0, sizeof(cl_mem), &memA);
    ckE(status, __LINE__);
    status = clSetKernelArg(simpleKernel, 1, sizeof(cl_mem), &memB);
    ckE(status, __LINE__);
    status = clSetKernelArg(simpleKernel, 2, sizeof(cl_uint), &numThread);
    ckE(status, __LINE__);
    size_t globalsize[1]={(size_t)numThread};
    size_t localsize[1]={64};
    status=clEnqueueNDRangeKernel(cmdQueueA, simpleKernel, 1, NULL, globalsize, localsize, 0, NULL, NULL);
    ckE(status, __LINE__);
    
    status = clEnqueueReadBuffer(cmdQueueA, memB, CL_TRUE, 0, numThread*sizeof(cl_float), B, 0, NULL, NULL);
    ckE(status, __LINE__);
    
    
    for(int i=0;i<numThread;i++){
        if((B[i]-A[i])!=1.0f){
            printf("Result is not correct\n");
            exit(1);
        }
    }
    
    printf("Result is right and your GPU is working\n");

    
    ckE(clReleaseMemObject(memA), __LINE__);
    ckE(clReleaseMemObject(memB),__LINE__);
    ckE(clReleaseKernel(simpleKernel), __LINE__);
    ckE(clReleaseCommandQueue(cmdQueueA), __LINE__);
    ckE(clReleaseProgram(program), __LINE__);
    ckE(clReleaseContext(context), __LINE__);
    ckE(clReleaseDevice(this_device), __LINE__);
    
    
    return 0;
}