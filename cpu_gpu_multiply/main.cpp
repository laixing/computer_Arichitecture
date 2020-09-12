#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <chrono>
#include <string>
#define CL_USE_DEPRECATED_OPENCL_1_2_APIS
#include <CL/cl.h>

#define MAX_SOURCE_SIZE (0x100000)
#define SIZE 512 // column/row size.
#define SIZE1 1024 // column/row size.
#define SIZE2 2048 // column/row size.
#define TILE_SIZE 8 // column/row size in a tile. It can only be 1,2,4,8,16 because of the limitation from the GPU 
#define TILE_SIZE1 16 // column/row size in a tile. It can only be 1,2,4,8,16 because of the limitation from the GPU 


// C version of matrix multiplcation. Use this function for result validation and execution time comaprison
void matrix_mul_sequence (int *A_mat,
                          int *B_mat,
                          int *C_mat)
{
	for (int j=0; j<SIZE; j++) {
		for (int i=0; i<SIZE; i++)
			for (int k=0; k<SIZE; k++)
				C_mat[j*SIZE + i] += A_mat[j*SIZE + k] * B_mat[k*SIZE + i];
	}
}

void matrix_mul_sequence1(int *A_mat1,
	int *B_mat1,
	int *C_mat1)
{
	for (int j = 0; j < SIZE1; j++) {
		for (int i = 0; i < SIZE1; i++)
			for (int k = 0; k < SIZE1; k++)
				C_mat1[j*SIZE1 + i] += A_mat1[j*SIZE1 + k] * B_mat1[k*SIZE1 + i];
	}
}

void matrix_mul_sequence2(int *A_mat2,
	int *B_mat2,
	int *C_mat2)
{
	for (int j = 0; j < SIZE2; j++) {
		for (int i = 0; i < SIZE2; i++)
			for (int k = 0; k < SIZE2; k++)
				C_mat2[j*SIZE2 + i] += A_mat2[j*SIZE2 + k] * B_mat2[k*SIZE2 + i];
	}
}

int main(void)
{
    // A, B are input matrix, C is the output matrix for OpenCL, C_seq is the output matrix for reference implementation.
    int *A = new int[SIZE*SIZE];
    int *B = new int[SIZE*SIZE];
    int *C = new int[SIZE*SIZE];
    int *C_seq = new int[SIZE*SIZE];

    // matrix when size is 1024
    int *A1 = new int[SIZE1*SIZE1];
    int *B1 = new int[SIZE1*SIZE1];
    int *C1 = new int[SIZE1*SIZE1];
    int *C_seq1 = new int[SIZE1*SIZE1];

    //matrix when size is 2048
    int *A2 = new int[SIZE2*SIZE2];
    int *B2 = new int[SIZE2*SIZE2];
    int *C2 = new int[SIZE2*SIZE2];
    int *C_seq2 = new int[SIZE2*SIZE2];
    

    //Initialize matrix
    for(int j=0; j<SIZE; j++) {
		for(int i=0; i<SIZE; i++) {
			A[j*SIZE + i] = rand()%41-20;
			B[j*SIZE + i] = rand()%41-20;
			C[j*SIZE + i] = 0;
			C_seq[j*SIZE + i] = 0;
			
		}
    }

	//Initialize matrix
	for (int j = 0; j < SIZE1; j++) {
		for (int i = 0; i < SIZE1; i++) {
			A1[j*SIZE1 + i] = rand() % 41 - 20;
			B1[j*SIZE1 + i] = rand() % 41 - 20;
			C1[j*SIZE1 + i] = 0;
			C_seq1[j*SIZE1 + i] = 0;

		}
	}

	//Initialize matrix
	for (int j = 0; j < SIZE2; j++) {
		for (int i = 0; i < SIZE2; i++) {
			A2[j*SIZE2 + i] = rand() % 41 - 20;
			B2[j*SIZE2 + i] = rand() % 41 - 20;
			C2[j*SIZE2 + i] = 0;
			C_seq2[j*SIZE2 + i] = 0;

		}
	}

	std::cout<<"----------Run reference C implementation----------"<<std::endl;
	std::cout<<"When matrix size is 512*512:"<<std::endl;
	std::chrono::high_resolution_clock::time_point t1, t2;
	t1 = std::chrono::high_resolution_clock::now();
        matrix_mul_sequence(A, B, C_seq);
        t2 = std::chrono::high_resolution_clock::now();
        std::cout << "Reference C matrix multiplication with size 512: "
		<< (float)(std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count())/1000000
		<< " sec"
		<< std::endl;

	std::cout<<"When matrix size is 1024*1024:"<<std::endl;
	std::chrono::high_resolution_clock::time_point t3, t4;
	t3 = std::chrono::high_resolution_clock::now();
	matrix_mul_sequence1(A1, B1, C_seq1);
	t4 = std::chrono::high_resolution_clock::now();
	std::cout << "Reference C matrix multiplication with size 1024: "
		<< (float)(std::chrono::duration_cast<std::chrono::microseconds>(t4 - t3).count()) / 1000000
		<< " sec"
		<< std::endl;

	std::cout<<"When matrix size is 2048*2048:"<<std::endl;
	std::chrono::high_resolution_clock::time_point t5, t6;
	t5 = std::chrono::high_resolution_clock::now();
	matrix_mul_sequence2(A2, B2, C_seq2);
	t6 = std::chrono::high_resolution_clock::now();
	std::cout << "Reference C matrix multiplication with size 2048: "
		<< (float)(std::chrono::duration_cast<std::chrono::microseconds>(t6 - t5).count()) / 1000000
		<< " sec"
		<< std::endl;
	std::cout<<"-------------------------------------------"<<std::endl;
    // Load the kernel source code into the array source_str
    FILE *fp;
    char *source_str;
    size_t source_size;

    fp = fopen("matrix_mul.cl", "r");
    if (!fp) {
        fprintf(stderr, "Failed to load kernel.\n");
        exit(1);
    }
    source_str = (char*)malloc(MAX_SOURCE_SIZE);
    source_size = fread( source_str, 1, MAX_SOURCE_SIZE, fp);
    fclose( fp );

    // Get platform and device information
    cl_platform_id platform_id = NULL;
    cl_device_id device_id = NULL;
    cl_uint ret_num_devices;
    cl_uint ret_num_platforms;
    cl_int err = CL_SUCCESS;
	cl_int ret=1;
	cl_int ret2=1;
    err = clGetPlatformIDs(0, 0, &ret_num_platforms);
    struct
    {
        cl_device_type type;
        const char* name;
        cl_uint count;
    }
    devices[] =
    {
        { CL_DEVICE_TYPE_CPU, "CL_DEVICE_TYPE_CPU", 0 },
        { CL_DEVICE_TYPE_GPU, "CL_DEVICE_TYPE_GPU", 0 },
        { CL_DEVICE_TYPE_ACCELERATOR, "CL_DEVICE_TYPE_ACCELERATOR", 0 }
    };

    const int NUM_OF_DEVICE_TYPES = sizeof(devices)/sizeof(devices[0]);
    
    std::cout << "Number of available platforms: " << ret_num_platforms << std::endl;
    cl_platform_id* platforms = new cl_platform_id[ret_num_platforms];
    // get IDs for all platforms:
    err = clGetPlatformIDs(ret_num_platforms, platforms, 0);
    cl_uint selected_platform_index = ret_num_platforms;
    for(cl_uint i1 = 0; i1 < ret_num_platforms; ++i1)
    {
        // Get the length for the i-th platform name
        size_t platform_name_length = 0;
        err = clGetPlatformInfo(
            platforms[i1],
            CL_PLATFORM_NAME,
            0,
            0,
            &platform_name_length
            );
        

        // Get the name itself for the i-th platform
        char* platform_name = new char[platform_name_length];
        err = clGetPlatformInfo(
            platforms[i1],
            CL_PLATFORM_NAME,
            platform_name_length,
            platform_name,
            0
            );
        

        std::cout << "    [" << i1 << "] " << platform_name<<std::endl;
		std::cout << "platform id: " << platforms[i1] << std::endl;

        // decide if this i-th platform is what we are looking for
        // we select the first one matched skipping the next one if any
       

        std::cout << std::endl;
        cl_platform_id platform=platforms[selected_platform_index-1];
        std::cout << "Number of devices available for each type:\n";

    // Now iterate over all device types picked above and
    // initialize num_of_devices
		for (int j1 = 0; j1 < NUM_OF_DEVICE_TYPES; ++j1)
		{
			err = clGetDeviceIDs(
				platforms[i1],
				devices[j1].type,
				0,
				0,
				&devices[j1].count
			);

			if (CL_DEVICE_NOT_FOUND == err)
			{
				// that's OK to fall here, because not all types of devices, which
				// you query for may be available for a particular system
				devices[j1].count = 0;
				err = CL_SUCCESS;
			}
			
			

			std::cout
				<< "    " << devices[j1].name << ": "
				<< devices[j1].count << std::endl;
		}
		if (devices[0].count == 1)
		{
			//ret = clGetPlatformIDs(1, &platform_id, &ret_num_platforms);
			//std::cout << "ret value: " << ret << std::endl;
			ret = clGetDeviceIDs(platforms[i1], CL_DEVICE_TYPE_CPU, 1, &device_id, &ret_num_devices);
			std::cout<<"----------Run matrix multiplication on CPU----------"<<std::endl;
			//std::cout << "ret value: " << ret << std::endl;
			std::cout<<"When matrix size is 512*512:"<<std::endl;
			// Create an OpenCL context size 512 
			cl_context context = clCreateContext(NULL, 1, &device_id, NULL, NULL, &ret);

			// Create a command queue with the capability of performance profiling for target device
			cl_command_queue command_queue = clCreateCommandQueue(context, device_id, CL_QUEUE_PROFILING_ENABLE, &ret);

			// Create memory buffers on the device for each matrix
			cl_mem a_mem_obj = clCreateBuffer(context, CL_MEM_READ_WRITE, SIZE*SIZE * sizeof(int), NULL, &ret);
			cl_mem b_mem_obj = clCreateBuffer(context, CL_MEM_READ_WRITE, SIZE*SIZE * sizeof(int), NULL, &ret);
			cl_mem c_mem_obj = clCreateBuffer(context, CL_MEM_READ_WRITE, SIZE*SIZE * sizeof(int), NULL, &ret);

			// Copy the matrix A, B and C to each device memory counterpart
			ret = clEnqueueWriteBuffer(command_queue, a_mem_obj, CL_TRUE, 0, SIZE*SIZE * sizeof(int), A, 0, NULL, NULL);
			ret = clEnqueueWriteBuffer(command_queue, b_mem_obj, CL_TRUE, 0, SIZE*SIZE * sizeof(int), B, 0, NULL, NULL);
			ret = clEnqueueWriteBuffer(command_queue, c_mem_obj, CL_TRUE, 0, SIZE*SIZE * sizeof(int), C, 0, NULL, NULL);

			// Create a program from the kernel source
			cl_program program = clCreateProgramWithSource(context, 1, (const char **)&source_str, (const size_t *)&source_size, &ret);

			// Build and compile the OpenCL kernel program
			std::string build_option = "-DTILE_SIZE=" + std::to_string(TILE_SIZE);
			ret = clBuildProgram(program, 1, &device_id, build_option.c_str(), NULL, NULL);
			if (ret == CL_BUILD_PROGRAM_FAILURE) { // If compile failed, print the error message
				// Determine the size of the log
				size_t log_size;
				clGetProgramBuildInfo(program, device_id, CL_PROGRAM_BUILD_LOG, 0, NULL, &log_size);
				char *log = (char *)malloc(log_size);

				// Get the log and print it
				clGetProgramBuildInfo(program, device_id, CL_PROGRAM_BUILD_LOG, log_size, log, NULL);
				printf("%s\n", log);
			}

			// Create the OpenCL kernel
			cl_kernel kernel;
			kernel = clCreateKernel(program, "matrix_mul", &ret);

			// Set the arguments of the kernel
			ret = clSetKernelArg(kernel, 0, sizeof(cl_mem), (void *)&a_mem_obj);
			ret = clSetKernelArg(kernel, 1, sizeof(cl_mem), (void *)&b_mem_obj);
			ret = clSetKernelArg(kernel, 2, sizeof(cl_mem), (void *)&c_mem_obj);

			int dimention = 2; // In this example, We will use 2 dimention index
			size_t global_item_size[] = { SIZE, SIZE, 1 };
			size_t local_item_size[] = { TILE_SIZE, TILE_SIZE, 1 };

			cl_event perf_event;
			cl_ulong start, end;

			// Execute the OpenCL kernel
			ret = clEnqueueNDRangeKernel(command_queue, kernel, dimention, NULL, global_item_size, local_item_size, 0, NULL, &perf_event);
			// Capture performance event from target device. In this case the event is to retrive the execution time.
			ret = clWaitForEvents(1, &perf_event);
			ret = clGetEventProfilingInfo(perf_event, CL_PROFILING_COMMAND_START, sizeof(start), &start, NULL);
			ret = clGetEventProfilingInfo(perf_event, CL_PROFILING_COMMAND_END, sizeof(end), &end, NULL);
			std::cout << "OpenCL matrix multiplication with normal kernel: " << (float)(end - start) / 1000000000 << " sec" << std::endl;

			// Read the memory buffer C from the device into the local variable C
			ret = clEnqueueReadBuffer(command_queue, c_mem_obj, CL_TRUE, 0, SIZE*SIZE * sizeof(int), C, 0, NULL, NULL);

			// Make sure all the command in the command queue has been executed
			ret = clFinish(command_queue);

			bool validate = true;
			for (int j = 0; j < SIZE; j++) {
				for (int i = 0; i < SIZE; i++) {
					if (C[j*SIZE + i] != C_seq[j*SIZE + i])
						validate = false;
				}
			}

			if (validate == false)
				std::cout << "The results are mismatched !!" << std::endl;

			// Clean up
			ret = clReleaseKernel(kernel);
			ret = clReleaseProgram(program);
			ret = clReleaseMemObject(a_mem_obj);
			ret = clReleaseMemObject(b_mem_obj);
			ret = clReleaseMemObject(c_mem_obj);
			ret = clReleaseCommandQueue(command_queue);
			ret = clReleaseContext(context);

			// Create an OpenCL context
			context = clCreateContext(NULL, 1, &device_id, NULL, NULL, &ret);

			// Create a command queue with the capability of performance profiling for target device
			command_queue = clCreateCommandQueue(context, device_id, CL_QUEUE_PROFILING_ENABLE, &ret);

			// Create memory buffers on the device for each matrix
			a_mem_obj = clCreateBuffer(context, CL_MEM_READ_WRITE, SIZE*SIZE * sizeof(int), NULL, &ret);
			b_mem_obj = clCreateBuffer(context, CL_MEM_READ_WRITE, SIZE*SIZE * sizeof(int), NULL, &ret);
			c_mem_obj = clCreateBuffer(context, CL_MEM_READ_WRITE, SIZE*SIZE * sizeof(int), NULL, &ret);

			// Copy the matrix A, B and C to each device memory counterpart
			ret = clEnqueueWriteBuffer(command_queue, a_mem_obj, CL_TRUE, 0, SIZE*SIZE * sizeof(int), A, 0, NULL, NULL);
			ret = clEnqueueWriteBuffer(command_queue, b_mem_obj, CL_TRUE, 0, SIZE*SIZE * sizeof(int), B, 0, NULL, NULL);
			ret = clEnqueueWriteBuffer(command_queue, c_mem_obj, CL_TRUE, 0, SIZE*SIZE * sizeof(int), C, 0, NULL, NULL);

			// Create a program from the kernel source
			program = clCreateProgramWithSource(context, 1, (const char **)&source_str, (const size_t *)&source_size, &ret);

			// Build and compile the OpenCL kernel program
			//std::string build_option = "-DTILE_SIZE=" + std::to_string(TILE_SIZE);
			ret = clBuildProgram(program, 1, &device_id, build_option.c_str(), NULL, NULL);
			if (ret == CL_BUILD_PROGRAM_FAILURE) { // If compile failed, print the error message
				// Determine the size of the log
				size_t log_size;
				clGetProgramBuildInfo(program, device_id, CL_PROGRAM_BUILD_LOG, 0, NULL, &log_size);
				char *log = (char *)malloc(log_size);

				// Get the log and print it
				clGetProgramBuildInfo(program, device_id, CL_PROGRAM_BUILD_LOG, log_size, log, NULL);
				printf("%s\n", log);
			}

			// Create the OpenCL kernel
			
			kernel = clCreateKernel(program, "matrix_mul_tile", &ret);

			// Set the arguments of the kernel
			ret = clSetKernelArg(kernel, 0, sizeof(cl_mem), (void *)&a_mem_obj);
			ret = clSetKernelArg(kernel, 1, sizeof(cl_mem), (void *)&b_mem_obj);
			ret = clSetKernelArg(kernel, 2, sizeof(cl_mem), (void *)&c_mem_obj);

			//int dimention = 2; // In this example, We will use 2 dimention index
			//size_t global_item_size[] = { SIZE, SIZE, 1 };
			//size_t local_item_size[] = { TILE_SIZE, TILE_SIZE, 1 };

			//cl_event perf_event;
			//cl_ulong start, end;

			// Execute the OpenCL kernel
			ret = clEnqueueNDRangeKernel(command_queue, kernel, dimention, NULL, global_item_size, local_item_size, 0, NULL, &perf_event);
			// Capture performance event from target device. In this case the event is to retrive the execution time.
			ret = clWaitForEvents(1, &perf_event);
			ret = clGetEventProfilingInfo(perf_event, CL_PROFILING_COMMAND_START, sizeof(start), &start, NULL);
			ret = clGetEventProfilingInfo(perf_event, CL_PROFILING_COMMAND_END, sizeof(end), &end, NULL);
			std::cout << "OpenCL Tiled kernel matrix multiplication with tile_size=8 : " << (float)(end - start) / 1000000000 << " sec" << std::endl;

			// Read the memory buffer C from the device into the local variable C
			ret = clEnqueueReadBuffer(command_queue, c_mem_obj, CL_TRUE, 0, SIZE*SIZE * sizeof(int), C, 0, NULL, NULL);

			// Make sure all the command in the command queue has been executed
			ret = clFinish(command_queue);

			validate = true;
			for (int j = 0; j < SIZE; j++) {
				for (int i = 0; i < SIZE; i++) {
					if (C[j*SIZE + i] != C_seq[j*SIZE + i])
						validate = false;
				}
			}

			if (validate == false)
				std::cout << "The results are mismatched !!" << std::endl;

			// Clean up
			ret = clReleaseKernel(kernel);
			ret = clReleaseProgram(program);
			ret = clReleaseMemObject(a_mem_obj);
			ret = clReleaseMemObject(b_mem_obj);
			ret = clReleaseMemObject(c_mem_obj);
			ret = clReleaseCommandQueue(command_queue);
			ret = clReleaseContext(context);

			// Create an OpenCL context
			context = clCreateContext(NULL, 1, &device_id, NULL, NULL, &ret);

			// Create a command queue with the capability of performance profiling for target device
			command_queue = clCreateCommandQueue(context, device_id, CL_QUEUE_PROFILING_ENABLE, &ret);

			// Create memory buffers on the device for each matrix
			a_mem_obj = clCreateBuffer(context, CL_MEM_READ_WRITE, SIZE*SIZE * sizeof(int), NULL, &ret);
			b_mem_obj = clCreateBuffer(context, CL_MEM_READ_WRITE, SIZE*SIZE * sizeof(int), NULL, &ret);
			c_mem_obj = clCreateBuffer(context, CL_MEM_READ_WRITE, SIZE*SIZE * sizeof(int), NULL, &ret);

			// Copy the matrix A, B and C to each device memory counterpart
			ret = clEnqueueWriteBuffer(command_queue, a_mem_obj, CL_TRUE, 0, SIZE*SIZE * sizeof(int), A, 0, NULL, NULL);
			ret = clEnqueueWriteBuffer(command_queue, b_mem_obj, CL_TRUE, 0, SIZE*SIZE * sizeof(int), B, 0, NULL, NULL);
			ret = clEnqueueWriteBuffer(command_queue, c_mem_obj, CL_TRUE, 0, SIZE*SIZE * sizeof(int), C, 0, NULL, NULL);

			// Create a program from the kernel source
			program = clCreateProgramWithSource(context, 1, (const char **)&source_str, (const size_t *)&source_size, &ret);

			// Build and compile the OpenCL kernel program
			build_option = "-DTILE_SIZE=" + std::to_string(TILE_SIZE1);
			ret = clBuildProgram(program, 1, &device_id, build_option.c_str(), NULL, NULL);
			if (ret == CL_BUILD_PROGRAM_FAILURE) { // If compile failed, print the error message
				// Determine the size of the log
				size_t log_size;
				clGetProgramBuildInfo(program, device_id, CL_PROGRAM_BUILD_LOG, 0, NULL, &log_size);
				char *log = (char *)malloc(log_size);

				// Get the log and print it
				clGetProgramBuildInfo(program, device_id, CL_PROGRAM_BUILD_LOG, log_size, log, NULL);
				printf("%s\n", log);
			}

			// Create the OpenCL kernel
			
			kernel = clCreateKernel(program, "matrix_mul_tile", &ret);

			// Set the arguments of the kernel
			ret = clSetKernelArg(kernel, 0, sizeof(cl_mem), (void *)&a_mem_obj);
			ret = clSetKernelArg(kernel, 1, sizeof(cl_mem), (void *)&b_mem_obj);
			ret = clSetKernelArg(kernel, 2, sizeof(cl_mem), (void *)&c_mem_obj);

			//int dimention = 2; // In this example, We will use 2 dimention index
			size_t global_item_size1[] = { SIZE, SIZE, 1 };
			size_t local_item_size1[] = { TILE_SIZE1, TILE_SIZE1, 1 };

			//cl_event perf_event;
			//cl_ulong start, end;

			// Execute the OpenCL kernel
			ret = clEnqueueNDRangeKernel(command_queue, kernel, dimention, NULL, global_item_size1, local_item_size1, 0, NULL, &perf_event);
			// Capture performance event from target device. In this case the event is to retrive the execution time.
			ret = clWaitForEvents(1, &perf_event);
			ret = clGetEventProfilingInfo(perf_event, CL_PROFILING_COMMAND_START, sizeof(start), &start, NULL);
			ret = clGetEventProfilingInfo(perf_event, CL_PROFILING_COMMAND_END, sizeof(end), &end, NULL);
			std::cout << "OpenCL Tiled kernel matrix multiplication with tile_size=16 : " << (float)(end - start) / 1000000000 << " sec" << std::endl;

			// Read the memory buffer C from the device into the local variable C
			ret = clEnqueueReadBuffer(command_queue, c_mem_obj, CL_TRUE, 0, SIZE*SIZE * sizeof(int), C, 0, NULL, NULL);

			// Make sure all the command in the command queue has been executed
			ret = clFinish(command_queue);

			validate = true;
			for (int j = 0; j < SIZE; j++) {
				for (int i = 0; i < SIZE; i++) {
					if (C[j*SIZE + i] != C_seq[j*SIZE + i])
						validate = false;
				}
			}

			if (validate == false)
				std::cout << "The results are mismatched !!" << std::endl;

			// Clean up
			ret = clReleaseKernel(kernel);
			ret = clReleaseProgram(program);
			ret = clReleaseMemObject(a_mem_obj);
			ret = clReleaseMemObject(b_mem_obj);
			ret = clReleaseMemObject(c_mem_obj);
			ret = clReleaseCommandQueue(command_queue);
			ret = clReleaseContext(context);

			std::cout<<"When matrix size is 1024*1024:"<<std::endl;
			// Create an OpenCL context  Matrix size 1024
			context = clCreateContext(NULL, 1, &device_id, NULL, NULL, &ret);

			// Create a command queue with the capability of performance profiling for target device
			command_queue = clCreateCommandQueue(context, device_id, CL_QUEUE_PROFILING_ENABLE, &ret);

			// Create memory buffers on the device for each matrix
			cl_mem a_mem_obj1 = clCreateBuffer(context, CL_MEM_READ_WRITE, SIZE1*SIZE1 * sizeof(int), NULL, &ret);
			cl_mem b_mem_obj1 = clCreateBuffer(context, CL_MEM_READ_WRITE, SIZE1*SIZE1 * sizeof(int), NULL, &ret);
			cl_mem c_mem_obj1 = clCreateBuffer(context, CL_MEM_READ_WRITE, SIZE1*SIZE1 * sizeof(int), NULL, &ret);

			// Copy the matrix A1, B1 and C1 to each device memory counterpart
			ret = clEnqueueWriteBuffer(command_queue, a_mem_obj1, CL_TRUE, 0, SIZE1*SIZE1 * sizeof(int), A1, 0, NULL, NULL);
			ret = clEnqueueWriteBuffer(command_queue, b_mem_obj1, CL_TRUE, 0, SIZE1*SIZE1 * sizeof(int), B1, 0, NULL, NULL);
			ret = clEnqueueWriteBuffer(command_queue, c_mem_obj1, CL_TRUE, 0, SIZE1*SIZE1 * sizeof(int), C1, 0, NULL, NULL);

			// Create a program from the kernel source
			program = clCreateProgramWithSource(context, 1, (const char **)&source_str, (const size_t *)&source_size, &ret);

			// Build and compile the OpenCL kernel program
			build_option = "-DTILE_SIZE=" + std::to_string(TILE_SIZE);
			ret = clBuildProgram(program, 1, &device_id, build_option.c_str(), NULL, NULL);
			if (ret == CL_BUILD_PROGRAM_FAILURE) { // If compile failed, print the error message
				// Determine the size of the log
				size_t log_size;
				clGetProgramBuildInfo(program, device_id, CL_PROGRAM_BUILD_LOG, 0, NULL, &log_size);
				char *log = (char *)malloc(log_size);

				// Get the log and print it
				clGetProgramBuildInfo(program, device_id, CL_PROGRAM_BUILD_LOG, log_size, log, NULL);
				printf("%s\n", log);
			}

			// Create the OpenCL kernel
			//cl_kernel kernel;
			kernel = clCreateKernel(program, "matrix_mul", &ret);

			// Set the arguments of the kernel
			ret = clSetKernelArg(kernel, 0, sizeof(cl_mem), (void *)&a_mem_obj1);
			ret = clSetKernelArg(kernel, 1, sizeof(cl_mem), (void *)&b_mem_obj1);
			ret = clSetKernelArg(kernel, 2, sizeof(cl_mem), (void *)&c_mem_obj1);

			//int dimention = 2; // In this example, We will use 2 dimention index
			size_t global_item_size2[] = { SIZE1, SIZE1, 1 };
			size_t local_item_size2[] = { TILE_SIZE, TILE_SIZE, 1 };

			//cl_event perf_event;
			//cl_ulong start, end;

			// Execute the OpenCL kernel
			ret = clEnqueueNDRangeKernel(command_queue, kernel, dimention, NULL, global_item_size2, local_item_size2, 0, NULL, &perf_event);
			// Capture performance event from target device. In this case the event is to retrive the execution time.
			ret = clWaitForEvents(1, &perf_event);
			ret = clGetEventProfilingInfo(perf_event, CL_PROFILING_COMMAND_START, sizeof(start), &start, NULL);
			ret = clGetEventProfilingInfo(perf_event, CL_PROFILING_COMMAND_END, sizeof(end), &end, NULL);
			std::cout << "OpenCL matrix multiplication with normal kernel: " << (float)(end - start) / 1000000000 << " sec" << std::endl;

			// Read the memory buffer C from the device into the local variable C
			ret = clEnqueueReadBuffer(command_queue, c_mem_obj1, CL_TRUE, 0, SIZE1*SIZE1 * sizeof(int), C1, 0, NULL, NULL);

			// Make sure all the command in the command queue has been executed
			ret = clFinish(command_queue);

			validate = true;
			for (int j = 0; j < SIZE1; j++) {
				for (int i = 0; i < SIZE1; i++) {
					if (C1[j*SIZE1 + i] != C_seq1[j*SIZE1 + i])
						validate = false;
				}
			}

			if (validate == false)
				std::cout << "The results are mismatched !!" << std::endl;

			// Clean up
			ret = clReleaseKernel(kernel);
			ret = clReleaseProgram(program);
			ret = clReleaseMemObject(a_mem_obj1);
			ret = clReleaseMemObject(b_mem_obj1);
			ret = clReleaseMemObject(c_mem_obj1);
			ret = clReleaseCommandQueue(command_queue);
			ret = clReleaseContext(context);

			// Create an OpenCL context  Matrix size 1024
			context = clCreateContext(NULL, 1, &device_id, NULL, NULL, &ret);

			// Create a command queue with the capability of performance profiling for target device
			command_queue = clCreateCommandQueue(context, device_id, CL_QUEUE_PROFILING_ENABLE, &ret);

			// Create memory buffers on the device for each matrix
			a_mem_obj1 = clCreateBuffer(context, CL_MEM_READ_WRITE, SIZE1*SIZE1 * sizeof(int), NULL, &ret);
			b_mem_obj1 = clCreateBuffer(context, CL_MEM_READ_WRITE, SIZE1*SIZE1 * sizeof(int), NULL, &ret);
			c_mem_obj1 = clCreateBuffer(context, CL_MEM_READ_WRITE, SIZE1*SIZE1 * sizeof(int), NULL, &ret);

			// Copy the matrix A, B and C to each device memory counterpart
			ret = clEnqueueWriteBuffer(command_queue, a_mem_obj1, CL_TRUE, 0, SIZE1*SIZE1 * sizeof(int), A1, 0, NULL, NULL);
			ret = clEnqueueWriteBuffer(command_queue, b_mem_obj1, CL_TRUE, 0, SIZE1*SIZE1 * sizeof(int), B1, 0, NULL, NULL);
			ret = clEnqueueWriteBuffer(command_queue, c_mem_obj1, CL_TRUE, 0, SIZE1*SIZE1 * sizeof(int), C1, 0, NULL, NULL);

			// Create a program from the kernel source
			program = clCreateProgramWithSource(context, 1, (const char **)&source_str, (const size_t *)&source_size, &ret);

			// Build and compile the OpenCL kernel program
			//build_option = "-DTILE_SIZE=" + std::to_string(TILE_SIZE);
			ret = clBuildProgram(program, 1, &device_id, build_option.c_str(), NULL, NULL);
			if (ret == CL_BUILD_PROGRAM_FAILURE) { // If compile failed, print the error message
				// Determine the size of the log
				size_t log_size;
				clGetProgramBuildInfo(program, device_id, CL_PROGRAM_BUILD_LOG, 0, NULL, &log_size);
				char *log = (char *)malloc(log_size);

				// Get the log and print it
				clGetProgramBuildInfo(program, device_id, CL_PROGRAM_BUILD_LOG, log_size, log, NULL);
				printf("%s\n", log);
			}

			// Create the OpenCL kernel
			
			kernel = clCreateKernel(program, "matrix_mul_tile", &ret);

			// Set the arguments of the kernel
			ret = clSetKernelArg(kernel, 0, sizeof(cl_mem), (void *)&a_mem_obj1);
			ret = clSetKernelArg(kernel, 1, sizeof(cl_mem), (void *)&b_mem_obj1);
			ret = clSetKernelArg(kernel, 2, sizeof(cl_mem), (void *)&c_mem_obj1);

			//int dimention = 2; // In this example, We will use 2 dimention index
			//size_t global_item_size2[] = { SIZE1, SIZE1, 1 };
			//size_t local_item_size2[] = { TILE_SIZE, TILE_SIZE, 1 };

			//cl_event perf_event;
			//cl_ulong start, end;

			// Execute the OpenCL kernel
			ret = clEnqueueNDRangeKernel(command_queue, kernel, dimention, NULL, global_item_size2, local_item_size2, 0, NULL, &perf_event);
			// Capture performance event from target device. In this case the event is to retrive the execution time.
			ret = clWaitForEvents(1, &perf_event);
			ret = clGetEventProfilingInfo(perf_event, CL_PROFILING_COMMAND_START, sizeof(start), &start, NULL);
			ret = clGetEventProfilingInfo(perf_event, CL_PROFILING_COMMAND_END, sizeof(end), &end, NULL);
			std::cout << "OpenCL Tiled kernel matrix multiplication with tile_size=8 : " << (float)(end - start) / 1000000000 << " sec" << std::endl;

			// Read the memory buffer C from the device into the local variable C
			ret = clEnqueueReadBuffer(command_queue, c_mem_obj1, CL_TRUE, 0, SIZE1*SIZE1 * sizeof(int), C1, 0, NULL, NULL);

			// Make sure all the command in the command queue has been executed
			ret = clFinish(command_queue);

			validate = true;
			for (int j = 0; j < SIZE1; j++) {
				for (int i = 0; i < SIZE1; i++) {
					if (C1[j*SIZE1 + i] != C_seq1[j*SIZE1 + i])
						validate = false;
				}
			}

			if (validate == false)
				std::cout << "The results are mismatched !!" << std::endl;

			// Clean up
			ret = clReleaseKernel(kernel);
			ret = clReleaseProgram(program);
			ret = clReleaseMemObject(a_mem_obj1);
			ret = clReleaseMemObject(b_mem_obj1);
			ret = clReleaseMemObject(c_mem_obj1);
			ret = clReleaseCommandQueue(command_queue);
			ret = clReleaseContext(context);

			// Create an OpenCL context
			context = clCreateContext(NULL, 1, &device_id, NULL, NULL, &ret);

			// Create a command queue with the capability of performance profiling for target device
			command_queue = clCreateCommandQueue(context, device_id, CL_QUEUE_PROFILING_ENABLE, &ret);

			// Create memory buffers on the device for each matrix
			a_mem_obj1 = clCreateBuffer(context, CL_MEM_READ_WRITE, SIZE1*SIZE1 * sizeof(int), NULL, &ret);
			b_mem_obj1 = clCreateBuffer(context, CL_MEM_READ_WRITE, SIZE1*SIZE1 * sizeof(int), NULL, &ret);
			c_mem_obj1 = clCreateBuffer(context, CL_MEM_READ_WRITE, SIZE1*SIZE1 * sizeof(int), NULL, &ret);

			// Copy the matrix A, B and C to each device memory counterpart
			ret = clEnqueueWriteBuffer(command_queue, a_mem_obj1, CL_TRUE, 0, SIZE1*SIZE1 * sizeof(int), A1, 0, NULL, NULL);
			ret = clEnqueueWriteBuffer(command_queue, b_mem_obj1, CL_TRUE, 0, SIZE1*SIZE1 * sizeof(int), B1, 0, NULL, NULL);
			ret = clEnqueueWriteBuffer(command_queue, c_mem_obj1, CL_TRUE, 0, SIZE1*SIZE1 * sizeof(int), C1, 0, NULL, NULL);

			// Create a program from the kernel source
			program = clCreateProgramWithSource(context, 1, (const char **)&source_str, (const size_t *)&source_size, &ret);

			// Build and compile the OpenCL kernel program
			build_option = "-DTILE_SIZE=" + std::to_string(TILE_SIZE1);
			ret = clBuildProgram(program, 1, &device_id, build_option.c_str(), NULL, NULL);
			if (ret == CL_BUILD_PROGRAM_FAILURE) { // If compile failed, print the error message
				// Determine the size of the log
				size_t log_size;
				clGetProgramBuildInfo(program, device_id, CL_PROGRAM_BUILD_LOG, 0, NULL, &log_size);
				char *log = (char *)malloc(log_size);

				// Get the log and print it
				clGetProgramBuildInfo(program, device_id, CL_PROGRAM_BUILD_LOG, log_size, log, NULL);
				printf("%s\n", log);
			}

			// Create the OpenCL kernel
			
			kernel = clCreateKernel(program, "matrix_mul_tile", &ret);

			// Set the arguments of the kernel
			ret = clSetKernelArg(kernel, 0, sizeof(cl_mem), (void *)&a_mem_obj1);
			ret = clSetKernelArg(kernel, 1, sizeof(cl_mem), (void *)&b_mem_obj1);
			ret = clSetKernelArg(kernel, 2, sizeof(cl_mem), (void *)&c_mem_obj1);

			//int dimention = 2; // In this example, We will use 2 dimention index
			size_t global_item_size3[] = { SIZE1, SIZE1, 1 };
			size_t local_item_size3[] = { TILE_SIZE1, TILE_SIZE1, 1 };

			//cl_event perf_event;
			//cl_ulong start, end;

			// Execute the OpenCL kernel
			ret = clEnqueueNDRangeKernel(command_queue, kernel, dimention, NULL, global_item_size3, local_item_size3, 0, NULL, &perf_event);
			// Capture performance event from target device. In this case the event is to retrive the execution time.
			ret = clWaitForEvents(1, &perf_event);
			ret = clGetEventProfilingInfo(perf_event, CL_PROFILING_COMMAND_START, sizeof(start), &start, NULL);
			ret = clGetEventProfilingInfo(perf_event, CL_PROFILING_COMMAND_END, sizeof(end), &end, NULL);
			std::cout << "OpenCL Tiled kernel matrix multiplication with tile_size=16 : " << (float)(end - start) / 1000000000 << " sec" << std::endl;

			// Read the memory buffer C from the device into the local variable C
			ret = clEnqueueReadBuffer(command_queue, c_mem_obj1, CL_TRUE, 0, SIZE1*SIZE1 * sizeof(int), C1, 0, NULL, NULL);

			// Make sure all the command in the command queue has been executed
			ret = clFinish(command_queue);

			validate = true;
			for (int j = 0; j < SIZE1; j++) {
				for (int i = 0; i < SIZE1; i++) {
					if (C1[j*SIZE1 + i] != C_seq1[j*SIZE1 + i])
						validate = false;
				}
			}

			if (validate == false)
				std::cout << "The results are mismatched !!" << std::endl;

			// Clean up
			ret = clReleaseKernel(kernel);
			ret = clReleaseProgram(program);
			ret = clReleaseMemObject(a_mem_obj1);
			ret = clReleaseMemObject(b_mem_obj1);
			ret = clReleaseMemObject(c_mem_obj1);
			ret = clReleaseCommandQueue(command_queue);
			ret = clReleaseContext(context);

			std::cout<<"When matrix size is 2048*2048:"<<std::endl;
			// Create an OpenCL context  Matrix size 2048
			context = clCreateContext(NULL, 1, &device_id, NULL, NULL, &ret);

			// Create a command queue with the capability of performance profiling for target device
			command_queue = clCreateCommandQueue(context, device_id, CL_QUEUE_PROFILING_ENABLE, &ret);

			// Create memory buffers on the device for each matrix
			cl_mem a_mem_obj2 = clCreateBuffer(context, CL_MEM_READ_WRITE, SIZE2*SIZE2 * sizeof(int), NULL, &ret);
			cl_mem b_mem_obj2 = clCreateBuffer(context, CL_MEM_READ_WRITE, SIZE2*SIZE2 * sizeof(int), NULL, &ret);
			cl_mem c_mem_obj2 = clCreateBuffer(context, CL_MEM_READ_WRITE, SIZE2*SIZE2 * sizeof(int), NULL, &ret);

			// Copy the matrix A1, B1 and C1 to each device memory counterpart
			ret = clEnqueueWriteBuffer(command_queue, a_mem_obj2, CL_TRUE, 0, SIZE2*SIZE2 * sizeof(int), A2, 0, NULL, NULL);
			ret = clEnqueueWriteBuffer(command_queue, b_mem_obj2, CL_TRUE, 0, SIZE2*SIZE2 * sizeof(int), B2, 0, NULL, NULL);
			ret = clEnqueueWriteBuffer(command_queue, c_mem_obj2, CL_TRUE, 0, SIZE2*SIZE2 * sizeof(int), C2, 0, NULL, NULL);

			// Create a program from the kernel source
			program = clCreateProgramWithSource(context, 1, (const char **)&source_str, (const size_t *)&source_size, &ret);

			// Build and compile the OpenCL kernel program
			build_option = "-DTILE_SIZE=" + std::to_string(TILE_SIZE);
			ret = clBuildProgram(program, 1, &device_id, build_option.c_str(), NULL, NULL);
			if (ret == CL_BUILD_PROGRAM_FAILURE) { // If compile failed, print the error message
				// Determine the size of the log
				size_t log_size;
				clGetProgramBuildInfo(program, device_id, CL_PROGRAM_BUILD_LOG, 0, NULL, &log_size);
				char *log = (char *)malloc(log_size);

				// Get the log and print it
				clGetProgramBuildInfo(program, device_id, CL_PROGRAM_BUILD_LOG, log_size, log, NULL);
				printf("%s\n", log);
			}

			// Create the OpenCL kernel
			//cl_kernel kernel;
			kernel = clCreateKernel(program, "matrix_mul", &ret);

			// Set the arguments of the kernel
			ret = clSetKernelArg(kernel, 0, sizeof(cl_mem), (void *)&a_mem_obj2);
			ret = clSetKernelArg(kernel, 1, sizeof(cl_mem), (void *)&b_mem_obj2);
			ret = clSetKernelArg(kernel, 2, sizeof(cl_mem), (void *)&c_mem_obj2);

			//int dimention = 2; // In this example, We will use 2 dimention index
			size_t global_item_size4[] = { SIZE2, SIZE2, 1 };
			size_t local_item_size4[] = { TILE_SIZE, TILE_SIZE, 1 };

			//cl_event perf_event;
			//cl_ulong start, end;

			// Execute the OpenCL kernel
			ret = clEnqueueNDRangeKernel(command_queue, kernel, dimention, NULL, global_item_size4, local_item_size4, 0, NULL, &perf_event);
			// Capture performance event from target device. In this case the event is to retrive the execution time.
			ret = clWaitForEvents(1, &perf_event);
			ret = clGetEventProfilingInfo(perf_event, CL_PROFILING_COMMAND_START, sizeof(start), &start, NULL);
			ret = clGetEventProfilingInfo(perf_event, CL_PROFILING_COMMAND_END, sizeof(end), &end, NULL);
			std::cout << "OpenCL matrix multiplication with normal kernel: " << (float)(end - start) / 1000000000 << " sec" << std::endl;

			// Read the memory buffer C from the device into the local variable C
			ret = clEnqueueReadBuffer(command_queue, c_mem_obj2, CL_TRUE, 0, SIZE2*SIZE2 * sizeof(int), C2, 0, NULL, NULL);

			// Make sure all the command in the command queue has been executed
			ret = clFinish(command_queue);

			validate = true;
			for (int j = 0; j < SIZE2; j++) {
				for (int i = 0; i < SIZE2; i++) {
					if (C2[j*SIZE2 + i] != C_seq2[j*SIZE2 + i])
						validate = false;
				}
			}

			if (validate == false)
				std::cout << "The results are mismatched !!" << std::endl;

			// Clean up
			ret = clReleaseKernel(kernel);
			ret = clReleaseProgram(program);
			ret = clReleaseMemObject(a_mem_obj2);
			ret = clReleaseMemObject(b_mem_obj2);
			ret = clReleaseMemObject(c_mem_obj2);
			ret = clReleaseCommandQueue(command_queue);
			ret = clReleaseContext(context);

			// Create an OpenCL context  Matrix size 2048
			context = clCreateContext(NULL, 1, &device_id, NULL, NULL, &ret);

			// Create a command queue with the capability of performance profiling for target device
			command_queue = clCreateCommandQueue(context, device_id, CL_QUEUE_PROFILING_ENABLE, &ret);

			// Create memory buffers on the device for each matrix
			a_mem_obj2 = clCreateBuffer(context, CL_MEM_READ_WRITE, SIZE2*SIZE2 * sizeof(int), NULL, &ret);
			b_mem_obj2 = clCreateBuffer(context, CL_MEM_READ_WRITE, SIZE2*SIZE2 * sizeof(int), NULL, &ret);
			c_mem_obj2 = clCreateBuffer(context, CL_MEM_READ_WRITE, SIZE2*SIZE2 * sizeof(int), NULL, &ret);

			// Copy the matrix A, B and C to each device memory counterpart
			ret = clEnqueueWriteBuffer(command_queue, a_mem_obj2, CL_TRUE, 0, SIZE2*SIZE2 * sizeof(int), A2, 0, NULL, NULL);
			ret = clEnqueueWriteBuffer(command_queue, b_mem_obj2, CL_TRUE, 0, SIZE2*SIZE2 * sizeof(int), B2, 0, NULL, NULL);
			ret = clEnqueueWriteBuffer(command_queue, c_mem_obj2, CL_TRUE, 0, SIZE2*SIZE2 * sizeof(int), C2, 0, NULL, NULL);

			// Create a program from the kernel source
			program = clCreateProgramWithSource(context, 1, (const char **)&source_str, (const size_t *)&source_size, &ret);

			// Build and compile the OpenCL kernel program
			//build_option = "-DTILE_SIZE=" + std::to_string(TILE_SIZE);
			ret = clBuildProgram(program, 1, &device_id, build_option.c_str(), NULL, NULL);
			if (ret == CL_BUILD_PROGRAM_FAILURE) { // If compile failed, print the error message
				// Determine the size of the log
				size_t log_size;
				clGetProgramBuildInfo(program, device_id, CL_PROGRAM_BUILD_LOG, 0, NULL, &log_size);
				char *log = (char *)malloc(log_size);

				// Get the log and print it
				clGetProgramBuildInfo(program, device_id, CL_PROGRAM_BUILD_LOG, log_size, log, NULL);
				printf("%s\n", log);
			}

			// Create the OpenCL kernel
			
			kernel = clCreateKernel(program, "matrix_mul_tile", &ret);

			// Set the arguments of the kernel
			ret = clSetKernelArg(kernel, 0, sizeof(cl_mem), (void *)&a_mem_obj2);
			ret = clSetKernelArg(kernel, 1, sizeof(cl_mem), (void *)&b_mem_obj2);
			ret = clSetKernelArg(kernel, 2, sizeof(cl_mem), (void *)&c_mem_obj2);

			//int dimention = 2; // In this example, We will use 2 dimention index
			//size_t global_item_size2[] = { SIZE1, SIZE1, 1 };
			//size_t local_item_size2[] = { TILE_SIZE, TILE_SIZE, 1 };

			//cl_event perf_event;
			//cl_ulong start, end;

			// Execute the OpenCL kernel
			ret = clEnqueueNDRangeKernel(command_queue, kernel, dimention, NULL, global_item_size4, local_item_size4, 0, NULL, &perf_event);
			// Capture performance event from target device. In this case the event is to retrive the execution time.
			ret = clWaitForEvents(1, &perf_event);
			ret = clGetEventProfilingInfo(perf_event, CL_PROFILING_COMMAND_START, sizeof(start), &start, NULL);
			ret = clGetEventProfilingInfo(perf_event, CL_PROFILING_COMMAND_END, sizeof(end), &end, NULL);
			std::cout << "OpenCL Tiled kernel matrix multiplication with tile_size=8 : " << (float)(end - start) / 1000000000 << " sec" << std::endl;

			// Read the memory buffer C from the device into the local variable C
			ret = clEnqueueReadBuffer(command_queue, c_mem_obj2, CL_TRUE, 0, SIZE2*SIZE2 * sizeof(int), C2, 0, NULL, NULL);

			// Make sure all the command in the command queue has been executed
			ret = clFinish(command_queue);

			validate = true;
			for (int j = 0; j < SIZE2; j++) {
				for (int i = 0; i < SIZE2; i++) {
					if (C2[j*SIZE2 + i] != C_seq2[j*SIZE2 + i])
						validate = false;
				}
			}

			if (validate == false)
				std::cout << "The results are mismatched !!" << std::endl;

			// Clean up
			ret = clReleaseKernel(kernel);
			ret = clReleaseProgram(program);
			ret = clReleaseMemObject(a_mem_obj2);
			ret = clReleaseMemObject(b_mem_obj2);
			ret = clReleaseMemObject(c_mem_obj2);
			ret = clReleaseCommandQueue(command_queue);
			ret = clReleaseContext(context);

			// Create an OpenCL context
			context = clCreateContext(NULL, 1, &device_id, NULL, NULL, &ret);

			// Create a command queue with the capability of performance profiling for target device
			command_queue = clCreateCommandQueue(context, device_id, CL_QUEUE_PROFILING_ENABLE, &ret);

			// Create memory buffers on the device for each matrix
			a_mem_obj2 = clCreateBuffer(context, CL_MEM_READ_WRITE, SIZE2*SIZE2 * sizeof(int), NULL, &ret);
			b_mem_obj2 = clCreateBuffer(context, CL_MEM_READ_WRITE, SIZE2*SIZE2 * sizeof(int), NULL, &ret);
			c_mem_obj2 = clCreateBuffer(context, CL_MEM_READ_WRITE, SIZE2*SIZE2 * sizeof(int), NULL, &ret);

			// Copy the matrix A, B and C to each device memory counterpart
			ret = clEnqueueWriteBuffer(command_queue, a_mem_obj2, CL_TRUE, 0, SIZE2*SIZE2 * sizeof(int), A2, 0, NULL, NULL);
			ret = clEnqueueWriteBuffer(command_queue, b_mem_obj2, CL_TRUE, 0, SIZE2*SIZE2 * sizeof(int), B2, 0, NULL, NULL);
			ret = clEnqueueWriteBuffer(command_queue, c_mem_obj2, CL_TRUE, 0, SIZE2*SIZE2 * sizeof(int), C2, 0, NULL, NULL);

			// Create a program from the kernel source
			program = clCreateProgramWithSource(context, 1, (const char **)&source_str, (const size_t *)&source_size, &ret);

			// Build and compile the OpenCL kernel program
			build_option = "-DTILE_SIZE=" + std::to_string(TILE_SIZE1);
			ret = clBuildProgram(program, 1, &device_id, build_option.c_str(), NULL, NULL);
			if (ret == CL_BUILD_PROGRAM_FAILURE) { // If compile failed, print the error message
				// Determine the size of the log
				size_t log_size;
				clGetProgramBuildInfo(program, device_id, CL_PROGRAM_BUILD_LOG, 0, NULL, &log_size);
				char *log = (char *)malloc(log_size);

				// Get the log and print it
				clGetProgramBuildInfo(program, device_id, CL_PROGRAM_BUILD_LOG, log_size, log, NULL);
				printf("%s\n", log);
			}

			// Create the OpenCL kernel
			
			kernel = clCreateKernel(program, "matrix_mul_tile", &ret);

			// Set the arguments of the kernel
			ret = clSetKernelArg(kernel, 0, sizeof(cl_mem), (void *)&a_mem_obj2);
			ret = clSetKernelArg(kernel, 1, sizeof(cl_mem), (void *)&b_mem_obj2);
			ret = clSetKernelArg(kernel, 2, sizeof(cl_mem), (void *)&c_mem_obj2);

			//int dimention = 2; // In this example, We will use 2 dimention index
			size_t global_item_size5[] = { SIZE2, SIZE2, 1 };
			size_t local_item_size5[] = { TILE_SIZE1, TILE_SIZE1, 1 };

			//cl_event perf_event;
			//cl_ulong start, end;

			// Execute the OpenCL kernel
			ret = clEnqueueNDRangeKernel(command_queue, kernel, dimention, NULL, global_item_size5, local_item_size5, 0, NULL, &perf_event);
			// Capture performance event from target device. In this case the event is to retrive the execution time.
			ret = clWaitForEvents(1, &perf_event);
			ret = clGetEventProfilingInfo(perf_event, CL_PROFILING_COMMAND_START, sizeof(start), &start, NULL);
			ret = clGetEventProfilingInfo(perf_event, CL_PROFILING_COMMAND_END, sizeof(end), &end, NULL);
			std::cout << "OpenCL Tiled kernel matrix multiplication with tile_size=16 : " << (float)(end - start) / 1000000000 << " sec" << std::endl;

			// Read the memory buffer C from the device into the local variable C
			ret = clEnqueueReadBuffer(command_queue, c_mem_obj2, CL_TRUE, 0, SIZE2*SIZE2 * sizeof(int), C2, 0, NULL, NULL);

			// Make sure all the command in the command queue has been executed
			ret = clFinish(command_queue);

			validate = true;
			for (int j = 0; j < SIZE2; j++) {
				for (int i = 0; i < SIZE2; i++) {
					if (C2[j*SIZE2 + i] != C_seq2[j*SIZE2 + i])
						validate = false;
				}
			}

			if (validate == false)
				std::cout << "The results are mismatched !!" << std::endl;

			// Clean up
			ret = clReleaseKernel(kernel);
			ret = clReleaseProgram(program);
			ret = clReleaseMemObject(a_mem_obj2);
			ret = clReleaseMemObject(b_mem_obj2);
			ret = clReleaseMemObject(c_mem_obj2);
			ret = clReleaseCommandQueue(command_queue);
			ret = clReleaseContext(context);
			std::cout<<"-------------------------------------------"<<std::endl;
			//}
		}
		if (devices[1].count == 1)
		{
			//ret2 = clGetPlatformIDs(1, &platform_id, &ret_num_platforms);
			ret2 = clGetDeviceIDs(platforms[i1], CL_DEVICE_TYPE_GPU, 1, &device_id, &ret_num_devices);
			std::cout<<"----------run matrix multiplication on GPU----------"<<std::endl;
			std::cout<<"When matrix size is 512*512:"<<std::endl;
			// Create an OpenCL context
			cl_context context = clCreateContext(NULL, 1, &device_id, NULL, NULL, &ret2);

			// Create a command queue with the capability of performance profiling for target device
			cl_command_queue command_queue = clCreateCommandQueue(context, device_id, CL_QUEUE_PROFILING_ENABLE, &ret2);

			// Create memory buffers on the device for each matrix
			cl_mem a_mem_obj = clCreateBuffer(context, CL_MEM_READ_WRITE, SIZE*SIZE * sizeof(int), NULL, &ret2);
			cl_mem b_mem_obj = clCreateBuffer(context, CL_MEM_READ_WRITE, SIZE*SIZE * sizeof(int), NULL, &ret2);
			cl_mem c_mem_obj = clCreateBuffer(context, CL_MEM_READ_WRITE, SIZE*SIZE * sizeof(int), NULL, &ret2);

			// Copy the matrix A, B and C to each device memory counterpart
			ret2 = clEnqueueWriteBuffer(command_queue, a_mem_obj, CL_TRUE, 0, SIZE*SIZE * sizeof(int), A, 0, NULL, NULL);
			ret2 = clEnqueueWriteBuffer(command_queue, b_mem_obj, CL_TRUE, 0, SIZE*SIZE * sizeof(int), B, 0, NULL, NULL);
			ret2 = clEnqueueWriteBuffer(command_queue, c_mem_obj, CL_TRUE, 0, SIZE*SIZE * sizeof(int), C, 0, NULL, NULL);

			// Create a program from the kernel source
			cl_program program = clCreateProgramWithSource(context, 1, (const char **)&source_str, (const size_t *)&source_size, &ret2);

			// Build and compile the OpenCL kernel program
			std::string build_option = "-DTILE_SIZE=" + std::to_string(TILE_SIZE);
			ret2 = clBuildProgram(program, 1, &device_id, build_option.c_str(), NULL, NULL);
			if (ret2 == CL_BUILD_PROGRAM_FAILURE) { // If compile failed, print the error message
				// Determine the size of the log
				size_t log_size;
				clGetProgramBuildInfo(program, device_id, CL_PROGRAM_BUILD_LOG, 0, NULL, &log_size);
				char *log = (char *)malloc(log_size);

				// Get the log and print it
				clGetProgramBuildInfo(program, device_id, CL_PROGRAM_BUILD_LOG, log_size, log, NULL);
				printf("%s\n", log);
			}

			// Create the OpenCL kernel
			cl_kernel kernel;
			kernel = clCreateKernel(program, "matrix_mul", &ret2);

			// Set the arguments of the kernel
			ret2 = clSetKernelArg(kernel, 0, sizeof(cl_mem), (void *)&a_mem_obj);
			ret2 = clSetKernelArg(kernel, 1, sizeof(cl_mem), (void *)&b_mem_obj);
			ret2 = clSetKernelArg(kernel, 2, sizeof(cl_mem), (void *)&c_mem_obj);

			int dimention = 2; // In this example, We will use 2 dimention index
			size_t global_item_size[] = { SIZE, SIZE, 1 };
			size_t local_item_size[] = { TILE_SIZE, TILE_SIZE, 1 };

			cl_event perf_event;
			cl_ulong start, end;

			// Execute the OpenCL kernel
			ret2 = clEnqueueNDRangeKernel(command_queue, kernel, dimention, NULL, global_item_size, local_item_size, 0, NULL, &perf_event);
			// Capture performance event from target device. In this case the event is to retrive the execution time.
			ret2 = clWaitForEvents(1, &perf_event);
			ret2 = clGetEventProfilingInfo(perf_event, CL_PROFILING_COMMAND_START, sizeof(start), &start, NULL);
			ret2 = clGetEventProfilingInfo(perf_event, CL_PROFILING_COMMAND_END, sizeof(end), &end, NULL);
			std::cout << "OpenCL matrix multiplication with normal kernel: " << (float)(end - start) / 1000000000 << " sec" << std::endl;

			// Read the memory buffer C from the device into the local variable C
			ret2 = clEnqueueReadBuffer(command_queue, c_mem_obj, CL_TRUE, 0, SIZE*SIZE * sizeof(int), C, 0, NULL, NULL);

			// Make sure all the command in the command queue has been executed
			ret2 = clFinish(command_queue);

			bool validate = true;
			for (int j = 0; j < SIZE; j++) {
				for (int i = 0; i < SIZE; i++) {
					if (C[j*SIZE + i] != C_seq[j*SIZE + i])
						validate = false;
				}
			}

			if (validate == false)
				std::cout << "The results are mismatched !!" << std::endl;

			// Clean up
			ret2 = clReleaseKernel(kernel);
			ret2 = clReleaseProgram(program);
			ret2 = clReleaseMemObject(a_mem_obj);
			ret2 = clReleaseMemObject(b_mem_obj);
			ret2 = clReleaseMemObject(c_mem_obj);
			ret2 = clReleaseCommandQueue(command_queue);
			ret2 = clReleaseContext(context);

                        // Create an OpenCL context
			context = clCreateContext(NULL, 1, &device_id, NULL, NULL, &ret2);

			// Create a command queue with the capability of performance profiling for target device
			command_queue = clCreateCommandQueue(context, device_id, CL_QUEUE_PROFILING_ENABLE, &ret2);

			// Create memory buffers on the device for each matrix
			a_mem_obj = clCreateBuffer(context, CL_MEM_READ_WRITE, SIZE*SIZE * sizeof(int), NULL, &ret2);
			b_mem_obj = clCreateBuffer(context, CL_MEM_READ_WRITE, SIZE*SIZE * sizeof(int), NULL, &ret2);
			c_mem_obj = clCreateBuffer(context, CL_MEM_READ_WRITE, SIZE*SIZE * sizeof(int), NULL, &ret2);

			// Copy the matrix A, B and C to each device memory counterpart
			ret2 = clEnqueueWriteBuffer(command_queue, a_mem_obj, CL_TRUE, 0, SIZE*SIZE * sizeof(int), A, 0, NULL, NULL);
			ret2 = clEnqueueWriteBuffer(command_queue, b_mem_obj, CL_TRUE, 0, SIZE*SIZE * sizeof(int), B, 0, NULL, NULL);
			ret2 = clEnqueueWriteBuffer(command_queue, c_mem_obj, CL_TRUE, 0, SIZE*SIZE * sizeof(int), C, 0, NULL, NULL);

			// Create a program from the kernel source
			program = clCreateProgramWithSource(context, 1, (const char **)&source_str, (const size_t *)&source_size, &ret2);

			// Build and compile the OpenCL kernel program
			//std::string build_option2 = "-DTILE_SIZE=" + std::to_string(TILE_SIZE);
			ret2 = clBuildProgram(program, 1, &device_id, build_option.c_str(), NULL, NULL);
			if (ret2 == CL_BUILD_PROGRAM_FAILURE) { // If compile failed, print the error message
				// Determine the size of the log
				size_t log_size;
				clGetProgramBuildInfo(program, device_id, CL_PROGRAM_BUILD_LOG, 0, NULL, &log_size);
				char *log = (char *)malloc(log_size);

				// Get the log and print it
				clGetProgramBuildInfo(program, device_id, CL_PROGRAM_BUILD_LOG, log_size, log, NULL);
				printf("%s\n", log);
			}

			// Create the OpenCL kernel
			//cl_kernel kernel;
			kernel = clCreateKernel(program, "matrix_mul_tile", &ret2);

			// Set the arguments of the kernel
			ret2 = clSetKernelArg(kernel, 0, sizeof(cl_mem), (void *)&a_mem_obj);
			ret2 = clSetKernelArg(kernel, 1, sizeof(cl_mem), (void *)&b_mem_obj);
			ret2 = clSetKernelArg(kernel, 2, sizeof(cl_mem), (void *)&c_mem_obj);

			//int dimention = 2; // In this example, We will use 2 dimention index
			//size_t global_item_size2[] = { SIZE, SIZE, 1 };
			//size_t local_item_size2[] = { TILE_SIZE, TILE_SIZE, 1 };

			//cl_event perf_event;
			//cl_ulong start, end;

			// Execute the OpenCL kernel
			ret2 = clEnqueueNDRangeKernel(command_queue, kernel, dimention, NULL, global_item_size, local_item_size, 0, NULL, &perf_event);
			// Capture performance event from target device. In this case the event is to retrive the execution time.
			ret2 = clWaitForEvents(1, &perf_event);
			ret2 = clGetEventProfilingInfo(perf_event, CL_PROFILING_COMMAND_START, sizeof(start), &start, NULL);
			ret2 = clGetEventProfilingInfo(perf_event, CL_PROFILING_COMMAND_END, sizeof(end), &end, NULL);
			std::cout << "OpenCL tiled matrix multiplication with tile_size=8 : " << (float)(end - start) / 1000000000 << " sec" << std::endl;

			// Read the memory buffer C from the device into the local variable C
			ret2 = clEnqueueReadBuffer(command_queue, c_mem_obj, CL_TRUE, 0, SIZE*SIZE * sizeof(int), C, 0, NULL, NULL);

			// Make sure all the command in the command queue has been executed
			ret2 = clFinish(command_queue);

			validate = true;
			for (int j = 0; j < SIZE; j++) {
				for (int i = 0; i < SIZE; i++) {
					if (C[j*SIZE + i] != C_seq[j*SIZE + i])
						validate = false;
				}
			}

			if (validate == false)
				std::cout << "The results are mismatched !!" << std::endl;

			// Clean up
			ret2 = clReleaseKernel(kernel);
			ret2 = clReleaseProgram(program);
			ret2 = clReleaseMemObject(a_mem_obj);
			ret2 = clReleaseMemObject(b_mem_obj);
			ret2 = clReleaseMemObject(c_mem_obj);
			ret2 = clReleaseCommandQueue(command_queue);
			ret2 = clReleaseContext(context);

			// Create an OpenCL context
			context = clCreateContext(NULL, 1, &device_id, NULL, NULL, &ret2);

			// Create a command queue with the capability of performance profiling for target device
			command_queue = clCreateCommandQueue(context, device_id, CL_QUEUE_PROFILING_ENABLE, &ret2);

			// Create memory buffers on the device for each matrix
			a_mem_obj = clCreateBuffer(context, CL_MEM_READ_WRITE, SIZE*SIZE * sizeof(int), NULL, &ret2);
			b_mem_obj = clCreateBuffer(context, CL_MEM_READ_WRITE, SIZE*SIZE * sizeof(int), NULL, &ret2);
			c_mem_obj = clCreateBuffer(context, CL_MEM_READ_WRITE, SIZE*SIZE * sizeof(int), NULL, &ret2);

			// Copy the matrix A, B and C to each device memory counterpart
			ret2 = clEnqueueWriteBuffer(command_queue, a_mem_obj, CL_TRUE, 0, SIZE*SIZE * sizeof(int), A, 0, NULL, NULL);
			ret2 = clEnqueueWriteBuffer(command_queue, b_mem_obj, CL_TRUE, 0, SIZE*SIZE * sizeof(int), B, 0, NULL, NULL);
			ret2 = clEnqueueWriteBuffer(command_queue, c_mem_obj, CL_TRUE, 0, SIZE*SIZE * sizeof(int), C, 0, NULL, NULL);

			// Create a program from the kernel source
			program = clCreateProgramWithSource(context, 1, (const char **)&source_str, (const size_t *)&source_size, &ret2);

			// Build and compile the OpenCL kernel program
			build_option = "-DTILE_SIZE=" + std::to_string(TILE_SIZE1);
			ret2 = clBuildProgram(program, 1, &device_id, build_option.c_str(), NULL, NULL);
			if (ret2 == CL_BUILD_PROGRAM_FAILURE) { // If compile failed, print the error message
				// Determine the size of the log
				size_t log_size;
				clGetProgramBuildInfo(program, device_id, CL_PROGRAM_BUILD_LOG, 0, NULL, &log_size);
				char *log = (char *)malloc(log_size);

				// Get the log and print it
				clGetProgramBuildInfo(program, device_id, CL_PROGRAM_BUILD_LOG, log_size, log, NULL);
				printf("%s\n", log);
			}

			// Create the OpenCL kernel
			//cl_kernel kernel;
			kernel = clCreateKernel(program, "matrix_mul_tile", &ret2);

			// Set the arguments of the kernel
			ret2 = clSetKernelArg(kernel, 0, sizeof(cl_mem), (void *)&a_mem_obj);
			ret2 = clSetKernelArg(kernel, 1, sizeof(cl_mem), (void *)&b_mem_obj);
			ret2 = clSetKernelArg(kernel, 2, sizeof(cl_mem), (void *)&c_mem_obj);

			//int dimention = 2; // In this example, We will use 2 dimention index
			size_t global_item_size1[] = { SIZE, SIZE, 1 };
			size_t local_item_size1[] = { TILE_SIZE1, TILE_SIZE1, 1 };

			//cl_event perf_event;
			//cl_ulong start, end;

			// Execute the OpenCL kernel
			ret2 = clEnqueueNDRangeKernel(command_queue, kernel, dimention, NULL, global_item_size1, local_item_size1, 0, NULL, &perf_event);
			// Capture performance event from target device. In this case the event is to retrive the execution time.
			ret2 = clWaitForEvents(1, &perf_event);
			ret2 = clGetEventProfilingInfo(perf_event, CL_PROFILING_COMMAND_START, sizeof(start), &start, NULL);
			ret2 = clGetEventProfilingInfo(perf_event, CL_PROFILING_COMMAND_END, sizeof(end), &end, NULL);
			std::cout << "OpenCL tiled matrix multiplication with tile_size=16 : " << (float)(end - start) / 1000000000 << " sec" << std::endl;

			// Read the memory buffer C from the device into the local variable C
			ret2 = clEnqueueReadBuffer(command_queue, c_mem_obj, CL_TRUE, 0, SIZE*SIZE * sizeof(int), C, 0, NULL, NULL);

			// Make sure all the command in the command queue has been executed
			ret2 = clFinish(command_queue);

			validate = true;
			for (int j = 0; j < SIZE; j++) {
				for (int i = 0; i < SIZE; i++) {
					if (C[j*SIZE + i] != C_seq[j*SIZE + i])
						validate = false;
				}
			}

			if (validate == false)
				std::cout << "The results are mismatched !!" << std::endl;

			// Clean up
			ret2 = clReleaseKernel(kernel);
			ret2 = clReleaseProgram(program);
			ret2 = clReleaseMemObject(a_mem_obj);
			ret2 = clReleaseMemObject(b_mem_obj);
			ret2 = clReleaseMemObject(c_mem_obj);
			ret2 = clReleaseCommandQueue(command_queue);
			ret2 = clReleaseContext(context);

			std::cout<<"When matrix size is 1024*1024:"<<std::endl;
			// Create an OpenCL context  Matrix size 1024
			//ret2 = clGetDeviceIDs(platforms[i1], CL_DEVICE_TYPE_GPU, 1, &device_id, &ret_num_devices);
			// Create an OpenCL context
			context = clCreateContext(NULL, 1, &device_id, NULL, NULL, &ret2);

			// Create a command queue with the capability of performance profiling for target device
			command_queue = clCreateCommandQueue(context, device_id, CL_QUEUE_PROFILING_ENABLE, &ret2);

			// Create memory buffers on the device for each matrix
			cl_mem a_mem_obj1 = clCreateBuffer(context, CL_MEM_READ_WRITE, SIZE1*SIZE1 * sizeof(int), NULL, &ret2);
			cl_mem b_mem_obj1 = clCreateBuffer(context, CL_MEM_READ_WRITE, SIZE1*SIZE1 * sizeof(int), NULL, &ret2);
			cl_mem c_mem_obj1 = clCreateBuffer(context, CL_MEM_READ_WRITE, SIZE1*SIZE1 * sizeof(int), NULL, &ret2);

			// Copy the matrix A, B and C to each device memory counterpart
			ret2 = clEnqueueWriteBuffer(command_queue, a_mem_obj1, CL_TRUE, 0, SIZE1*SIZE1 * sizeof(int), A1, 0, NULL, NULL);
			ret2 = clEnqueueWriteBuffer(command_queue, b_mem_obj1, CL_TRUE, 0, SIZE1*SIZE1 * sizeof(int), B1, 0, NULL, NULL);
			ret2 = clEnqueueWriteBuffer(command_queue, c_mem_obj1, CL_TRUE, 0, SIZE1*SIZE1 * sizeof(int), C1, 0, NULL, NULL);

			// Create a program from the kernel source
			program = clCreateProgramWithSource(context, 1, (const char **)&source_str, (const size_t *)&source_size, &ret2);

			// Build and compile the OpenCL kernel program
			build_option = "-DTILE_SIZE=" + std::to_string(TILE_SIZE);
			ret2 = clBuildProgram(program, 1, &device_id, build_option.c_str(), NULL, NULL);
			if (ret2 == CL_BUILD_PROGRAM_FAILURE) { // If compile failed, print the error message
				// Determine the size of the log
				size_t log_size;
				clGetProgramBuildInfo(program, device_id, CL_PROGRAM_BUILD_LOG, 0, NULL, &log_size);
				char *log = (char *)malloc(log_size);

				// Get the log and print it
				clGetProgramBuildInfo(program, device_id, CL_PROGRAM_BUILD_LOG, log_size, log, NULL);
				printf("%s\n", log);
			}

			// Create the OpenCL kernel
			//cl_kernel kernel;
			kernel = clCreateKernel(program, "matrix_mul", &ret2);

			// Set the arguments of the kernel
			ret2 = clSetKernelArg(kernel, 0, sizeof(cl_mem), (void *)&a_mem_obj1);
			ret2 = clSetKernelArg(kernel, 1, sizeof(cl_mem), (void *)&b_mem_obj1);
			ret2 = clSetKernelArg(kernel, 2, sizeof(cl_mem), (void *)&c_mem_obj1);

			//int dimention = 2; // In this example, We will use 2 dimention index
			size_t global_item_size2[] = { SIZE1, SIZE1, 1 };
			size_t local_item_size2[] = { TILE_SIZE, TILE_SIZE, 1 };

			//cl_event perf_event;
			//cl_ulong start, end;

			// Execute the OpenCL kernel
			ret2 = clEnqueueNDRangeKernel(command_queue, kernel, dimention, NULL, global_item_size2, local_item_size2, 0, NULL, &perf_event);
			// Capture performance event from target device. In this case the event is to retrive the execution time.
			ret2 = clWaitForEvents(1, &perf_event);
			ret2 = clGetEventProfilingInfo(perf_event, CL_PROFILING_COMMAND_START, sizeof(start), &start, NULL);
			ret2 = clGetEventProfilingInfo(perf_event, CL_PROFILING_COMMAND_END, sizeof(end), &end, NULL);
			std::cout << "OpenCL matrix multiplication with normal kernel: " << (float)(end - start) / 1000000000 << " sec" << std::endl;

			// Read the memory buffer C from the device into the local variable C
			ret2 = clEnqueueReadBuffer(command_queue, c_mem_obj1, CL_TRUE, 0, SIZE1*SIZE1 * sizeof(int), C1, 0, NULL, NULL);

			// Make sure all the command in the command queue has been executed
			ret2 = clFinish(command_queue);

			validate = true;
			for (int j = 0; j < SIZE1; j++) {
				for (int i = 0; i < SIZE1; i++) {
					if (C1[j*SIZE1 + i] != C_seq1[j*SIZE1 + i])
						validate = false;
				}
			}

			if (validate == false)
				std::cout << "The results are mismatched !!" << std::endl;

			// Clean up
			ret2 = clReleaseKernel(kernel);
			ret2 = clReleaseProgram(program);
			ret2 = clReleaseMemObject(a_mem_obj1);
			ret2 = clReleaseMemObject(b_mem_obj1);
			ret2 = clReleaseMemObject(c_mem_obj1);
			ret2 = clReleaseCommandQueue(command_queue);
			ret2 = clReleaseContext(context);


			// Create an OpenCL context  Matrix size 1024
			context = clCreateContext(NULL, 1, &device_id, NULL, NULL, &ret2);

			// Create a command queue with the capability of performance profiling for target device
			command_queue = clCreateCommandQueue(context, device_id, CL_QUEUE_PROFILING_ENABLE, &ret2);

			// Create memory buffers on the device for each matrix
			a_mem_obj1 = clCreateBuffer(context, CL_MEM_READ_WRITE, SIZE1*SIZE1 * sizeof(int), NULL, &ret2);
			b_mem_obj1 = clCreateBuffer(context, CL_MEM_READ_WRITE, SIZE1*SIZE1 * sizeof(int), NULL, &ret2);
			c_mem_obj1 = clCreateBuffer(context, CL_MEM_READ_WRITE, SIZE1*SIZE1 * sizeof(int), NULL, &ret2);

			// Copy the matrix A, B and C to each device memory counterpart
			ret2 = clEnqueueWriteBuffer(command_queue, a_mem_obj1, CL_TRUE, 0, SIZE1*SIZE1 * sizeof(int), A1, 0, NULL, NULL);
			ret2 = clEnqueueWriteBuffer(command_queue, b_mem_obj1, CL_TRUE, 0, SIZE1*SIZE1 * sizeof(int), B1, 0, NULL, NULL);
			ret2 = clEnqueueWriteBuffer(command_queue, c_mem_obj1, CL_TRUE, 0, SIZE1*SIZE1 * sizeof(int), C1, 0, NULL, NULL);

			// Create a program from the kernel source
			program = clCreateProgramWithSource(context, 1, (const char **)&source_str, (const size_t *)&source_size, &ret2);

			// Build and compile the OpenCL kernel program
			build_option = "-DTILE_SIZE=" + std::to_string(TILE_SIZE);
			ret2 = clBuildProgram(program, 1, &device_id, build_option.c_str(), NULL, NULL);
			if (ret2 == CL_BUILD_PROGRAM_FAILURE) { // If compile failed, print the error message
				// Determine the size of the log
				size_t log_size;
				clGetProgramBuildInfo(program, device_id, CL_PROGRAM_BUILD_LOG, 0, NULL, &log_size);
				char *log = (char *)malloc(log_size);

				// Get the log and print it
				clGetProgramBuildInfo(program, device_id, CL_PROGRAM_BUILD_LOG, log_size, log, NULL);
				printf("%s\n", log);
			}

			// Create the OpenCL kernel
			
			kernel = clCreateKernel(program, "matrix_mul_tile", &ret2);

			// Set the arguments of the kernel
			ret2 = clSetKernelArg(kernel, 0, sizeof(cl_mem), (void *)&a_mem_obj1);
			ret2 = clSetKernelArg(kernel, 1, sizeof(cl_mem), (void *)&b_mem_obj1);
			ret2 = clSetKernelArg(kernel, 2, sizeof(cl_mem), (void *)&c_mem_obj1);

			//int dimention = 2; // In this example, We will use 2 dimention index
			//size_t global_item_size2[] = { SIZE1, SIZE1, 1 };
			//size_t local_item_size2[] = { TILE_SIZE, TILE_SIZE, 1 };

			//cl_event perf_event;
			//cl_ulong start, end;

			// Execute the OpenCL kernel
			ret2 = clEnqueueNDRangeKernel(command_queue, kernel, dimention, NULL, global_item_size2, local_item_size2, 0, NULL, &perf_event);
			// Capture performance event from target device. In this case the event is to retrive the execution time.
			ret2 = clWaitForEvents(1, &perf_event);
			ret2 = clGetEventProfilingInfo(perf_event, CL_PROFILING_COMMAND_START, sizeof(start), &start, NULL);
			ret2 = clGetEventProfilingInfo(perf_event, CL_PROFILING_COMMAND_END, sizeof(end), &end, NULL);
			std::cout << "OpenCL Tiled kernel matrix multiplication with tile_size=8 : " << (float)(end - start) / 1000000000 << " sec" << std::endl;

			// Read the memory buffer C from the device into the local variable C
			ret2 = clEnqueueReadBuffer(command_queue, c_mem_obj1, CL_TRUE, 0, SIZE1*SIZE1 * sizeof(int), C1, 0, NULL, NULL);

			// Make sure all the command in the command queue has been executed
			ret2 = clFinish(command_queue);

			validate = true;
			for (int j = 0; j < SIZE1; j++) {
				for (int i = 0; i < SIZE1; i++) {
					if (C1[j*SIZE1 + i] != C_seq1[j*SIZE1 + i])
						validate = false;
				}
			}

			if (validate == false)
				std::cout << "The results are mismatched !!" << std::endl;

			// Clean up
			ret2 = clReleaseKernel(kernel);
			ret2 = clReleaseProgram(program);
			ret2 = clReleaseMemObject(a_mem_obj1);
			ret2 = clReleaseMemObject(b_mem_obj1);
			ret2 = clReleaseMemObject(c_mem_obj1);
			ret2 = clReleaseCommandQueue(command_queue);
			ret2 = clReleaseContext(context);

			// Create an OpenCL context
			context = clCreateContext(NULL, 1, &device_id, NULL, NULL, &ret2);

			// Create a command queue with the capability of performance profiling for target device
			command_queue = clCreateCommandQueue(context, device_id, CL_QUEUE_PROFILING_ENABLE, &ret2);

			// Create memory buffers on the device for each matrix
			a_mem_obj1 = clCreateBuffer(context, CL_MEM_READ_WRITE, SIZE1*SIZE1 * sizeof(int), NULL, &ret2);
			b_mem_obj1 = clCreateBuffer(context, CL_MEM_READ_WRITE, SIZE1*SIZE1 * sizeof(int), NULL, &ret2);
			c_mem_obj1 = clCreateBuffer(context, CL_MEM_READ_WRITE, SIZE1*SIZE1 * sizeof(int), NULL, &ret2);

			// Copy the matrix A, B and C to each device memory counterpart
			ret2 = clEnqueueWriteBuffer(command_queue, a_mem_obj1, CL_TRUE, 0, SIZE1*SIZE1 * sizeof(int), A1, 0, NULL, NULL);
			ret2 = clEnqueueWriteBuffer(command_queue, b_mem_obj1, CL_TRUE, 0, SIZE1*SIZE1 * sizeof(int), B1, 0, NULL, NULL);
			ret2 = clEnqueueWriteBuffer(command_queue, c_mem_obj1, CL_TRUE, 0, SIZE1*SIZE1 * sizeof(int), C1, 0, NULL, NULL);

			// Create a program from the kernel source
			program = clCreateProgramWithSource(context, 1, (const char **)&source_str, (const size_t *)&source_size, &ret2);

			// Build and compile the OpenCL kernel program
			build_option = "-DTILE_SIZE=" + std::to_string(TILE_SIZE1);
			ret2 = clBuildProgram(program, 1, &device_id, build_option.c_str(), NULL, NULL);
			if (ret2 == CL_BUILD_PROGRAM_FAILURE) { // If compile failed, print the error message
				// Determine the size of the log
				size_t log_size;
				clGetProgramBuildInfo(program, device_id, CL_PROGRAM_BUILD_LOG, 0, NULL, &log_size);
				char *log = (char *)malloc(log_size);

				// Get the log and print it
				clGetProgramBuildInfo(program, device_id, CL_PROGRAM_BUILD_LOG, log_size, log, NULL);
				printf("%s\n", log);
			}

			// Create the OpenCL kernel
			
			kernel = clCreateKernel(program, "matrix_mul_tile", &ret2);

			// Set the arguments of the kernel
			ret2 = clSetKernelArg(kernel, 0, sizeof(cl_mem), (void *)&a_mem_obj1);
			ret2 = clSetKernelArg(kernel, 1, sizeof(cl_mem), (void *)&b_mem_obj1);
			ret2 = clSetKernelArg(kernel, 2, sizeof(cl_mem), (void *)&c_mem_obj1);

			//int dimention = 2; // In this example, We will use 2 dimention index
			size_t global_item_size3[] = { SIZE1, SIZE1, 1 };
			size_t local_item_size3[] = { TILE_SIZE1, TILE_SIZE1, 1 };

			//cl_event perf_event;
			//cl_ulong start, end;

			// Execute the OpenCL kernel
			ret2 = clEnqueueNDRangeKernel(command_queue, kernel, dimention, NULL, global_item_size3, local_item_size3, 0, NULL, &perf_event);
			// Capture performance event from target device. In this case the event is to retrive the execution time.
			ret2 = clWaitForEvents(1, &perf_event);
			ret2 = clGetEventProfilingInfo(perf_event, CL_PROFILING_COMMAND_START, sizeof(start), &start, NULL);
			ret2 = clGetEventProfilingInfo(perf_event, CL_PROFILING_COMMAND_END, sizeof(end), &end, NULL);
			std::cout << "OpenCL Tiled kernel matrix multiplication with tile_size=16 : " << (float)(end - start) / 1000000000 << " sec" << std::endl;

			// Read the memory buffer C from the device into the local variable C
			ret2 = clEnqueueReadBuffer(command_queue, c_mem_obj1, CL_TRUE, 0, SIZE1*SIZE1 * sizeof(int), C1, 0, NULL, NULL);

			// Make sure all the command in the command queue has been executed
			ret2 = clFinish(command_queue);

			validate = true;
			for (int j = 0; j < SIZE1; j++) {
				for (int i = 0; i < SIZE1; i++) {
					if (C1[j*SIZE1 + i] != C_seq1[j*SIZE1 + i])
						validate = false;
				}
			}

			if (validate == false)
				std::cout << "The results are mismatched !!" << std::endl;

			// Clean up
			ret2 = clReleaseKernel(kernel);
			ret2 = clReleaseProgram(program);
			ret2 = clReleaseMemObject(a_mem_obj1);
			ret2 = clReleaseMemObject(b_mem_obj1);
			ret2 = clReleaseMemObject(c_mem_obj1);
			ret2 = clReleaseCommandQueue(command_queue);
			ret2 = clReleaseContext(context);

			std::cout<<"When matrix size is 2048*2048:"<<std::endl;
			// Create an OpenCL context  Matrix size 2048
			context = clCreateContext(NULL, 1, &device_id, NULL, NULL, &ret2);

			// Create a command queue with the capability of performance profiling for target device
			command_queue = clCreateCommandQueue(context, device_id, CL_QUEUE_PROFILING_ENABLE, &ret2);

			// Create memory buffers on the device for each matrix
			cl_mem a_mem_obj2 = clCreateBuffer(context, CL_MEM_READ_WRITE, SIZE2*SIZE2 * sizeof(int), NULL, &ret2);
			cl_mem b_mem_obj2 = clCreateBuffer(context, CL_MEM_READ_WRITE, SIZE2*SIZE2 * sizeof(int), NULL, &ret2);
			cl_mem c_mem_obj2 = clCreateBuffer(context, CL_MEM_READ_WRITE, SIZE2*SIZE2 * sizeof(int), NULL, &ret2);

			// Copy the matrix A1, B1 and C1 to each device memory counterpart
			ret2 = clEnqueueWriteBuffer(command_queue, a_mem_obj2, CL_TRUE, 0, SIZE2*SIZE2 * sizeof(int), A2, 0, NULL, NULL);
			ret2 = clEnqueueWriteBuffer(command_queue, b_mem_obj2, CL_TRUE, 0, SIZE2*SIZE2 * sizeof(int), B2, 0, NULL, NULL);
			ret2 = clEnqueueWriteBuffer(command_queue, c_mem_obj2, CL_TRUE, 0, SIZE2*SIZE2 * sizeof(int), C2, 0, NULL, NULL);

			// Create a program from the kernel source
			program = clCreateProgramWithSource(context, 1, (const char **)&source_str, (const size_t *)&source_size, &ret);

			// Build and compile the OpenCL kernel program
			build_option = "-DTILE_SIZE=" + std::to_string(TILE_SIZE);
			ret2 = clBuildProgram(program, 1, &device_id, build_option.c_str(), NULL, NULL);
			if (ret2 == CL_BUILD_PROGRAM_FAILURE) { // If compile failed, print the error message
				// Determine the size of the log
				size_t log_size;
				clGetProgramBuildInfo(program, device_id, CL_PROGRAM_BUILD_LOG, 0, NULL, &log_size);
				char *log = (char *)malloc(log_size);

				// Get the log and print it
				clGetProgramBuildInfo(program, device_id, CL_PROGRAM_BUILD_LOG, log_size, log, NULL);
				printf("%s\n", log);
			}

			// Create the OpenCL kernel
			//cl_kernel kernel;
			kernel = clCreateKernel(program, "matrix_mul", &ret2);

			// Set the arguments of the kernel
			ret2 = clSetKernelArg(kernel, 0, sizeof(cl_mem), (void *)&a_mem_obj2);
			ret2 = clSetKernelArg(kernel, 1, sizeof(cl_mem), (void *)&b_mem_obj2);
			ret2 = clSetKernelArg(kernel, 2, sizeof(cl_mem), (void *)&c_mem_obj2);

			//int dimention = 2; // In this example, We will use 2 dimention index
			size_t global_item_size4[] = { SIZE2, SIZE2, 1 };
			size_t local_item_size4[] = { TILE_SIZE, TILE_SIZE, 1 };

			//cl_event perf_event;
			//cl_ulong start, end;

			// Execute the OpenCL kernel
			ret2 = clEnqueueNDRangeKernel(command_queue, kernel, dimention, NULL, global_item_size4, local_item_size4, 0, NULL, &perf_event);
			// Capture performance event from target device. In this case the event is to retrive the execution time.
			ret2 = clWaitForEvents(1, &perf_event);
			ret2 = clGetEventProfilingInfo(perf_event, CL_PROFILING_COMMAND_START, sizeof(start), &start, NULL);
			ret2 = clGetEventProfilingInfo(perf_event, CL_PROFILING_COMMAND_END, sizeof(end), &end, NULL);
			std::cout << "OpenCL matrix multiplication with normal kernel: " << (float)(end - start) / 1000000000 << " sec" << std::endl;

			// Read the memory buffer C from the device into the local variable C
			ret2 = clEnqueueReadBuffer(command_queue, c_mem_obj2, CL_TRUE, 0, SIZE2*SIZE2 * sizeof(int), C2, 0, NULL, NULL);

			// Make sure all the command in the command queue has been executed
			ret2 = clFinish(command_queue);

			validate = true;
			for (int j = 0; j < SIZE2; j++) {
				for (int i = 0; i < SIZE2; i++) {
					if (C2[j*SIZE2 + i] != C_seq2[j*SIZE2 + i])
						validate = false;
				}
			}

			if (validate == false)
				std::cout << "The results are mismatched !!" << std::endl;

			// Clean up
			ret2 = clReleaseKernel(kernel);
			ret2 = clReleaseProgram(program);
			ret2 = clReleaseMemObject(a_mem_obj2);
			ret2 = clReleaseMemObject(b_mem_obj2);
			ret2 = clReleaseMemObject(c_mem_obj2);
			ret2 = clReleaseCommandQueue(command_queue);
			ret2 = clReleaseContext(context);

			// Create an OpenCL context  Matrix size 2048
			context = clCreateContext(NULL, 1, &device_id, NULL, NULL, &ret2);

			// Create a command queue with the capability of performance profiling for target device
			command_queue = clCreateCommandQueue(context, device_id, CL_QUEUE_PROFILING_ENABLE, &ret2);

			// Create memory buffers on the device for each matrix
			a_mem_obj2 = clCreateBuffer(context, CL_MEM_READ_WRITE, SIZE2*SIZE2 * sizeof(int), NULL, &ret2);
			b_mem_obj2 = clCreateBuffer(context, CL_MEM_READ_WRITE, SIZE2*SIZE2 * sizeof(int), NULL, &ret2);
			c_mem_obj2 = clCreateBuffer(context, CL_MEM_READ_WRITE, SIZE2*SIZE2 * sizeof(int), NULL, &ret2);

			// Copy the matrix A, B and C to each device memory counterpart
			ret2 = clEnqueueWriteBuffer(command_queue, a_mem_obj2, CL_TRUE, 0, SIZE2*SIZE2 * sizeof(int), A2, 0, NULL, NULL);
			ret2 = clEnqueueWriteBuffer(command_queue, b_mem_obj2, CL_TRUE, 0, SIZE2*SIZE2 * sizeof(int), B2, 0, NULL, NULL);
			ret2 = clEnqueueWriteBuffer(command_queue, c_mem_obj2, CL_TRUE, 0, SIZE2*SIZE2 * sizeof(int), C2, 0, NULL, NULL);

			// Create a program from the kernel source
			program = clCreateProgramWithSource(context, 1, (const char **)&source_str, (const size_t *)&source_size, &ret2);

			// Build and compile the OpenCL kernel program
			//build_option = "-DTILE_SIZE=" + std::to_string(TILE_SIZE);
			ret2 = clBuildProgram(program, 1, &device_id, build_option.c_str(), NULL, NULL);
			if (ret2 == CL_BUILD_PROGRAM_FAILURE) { // If compile failed, print the error message
				// Determine the size of the log
				size_t log_size;
				clGetProgramBuildInfo(program, device_id, CL_PROGRAM_BUILD_LOG, 0, NULL, &log_size);
				char *log = (char *)malloc(log_size);

				// Get the log and print it
				clGetProgramBuildInfo(program, device_id, CL_PROGRAM_BUILD_LOG, log_size, log, NULL);
				printf("%s\n", log);
			}

			// Create the OpenCL kernel
			
			kernel = clCreateKernel(program, "matrix_mul_tile", &ret2);

			// Set the arguments of the kernel
			ret2 = clSetKernelArg(kernel, 0, sizeof(cl_mem), (void *)&a_mem_obj2);
			ret2 = clSetKernelArg(kernel, 1, sizeof(cl_mem), (void *)&b_mem_obj2);
			ret2 = clSetKernelArg(kernel, 2, sizeof(cl_mem), (void *)&c_mem_obj2);

			//int dimention = 2; // In this example, We will use 2 dimention index
			//size_t global_item_size2[] = { SIZE1, SIZE1, 1 };
			//size_t local_item_size2[] = { TILE_SIZE, TILE_SIZE, 1 };

			//cl_event perf_event;
			//cl_ulong start, end;

			// Execute the OpenCL kernel
			ret2 = clEnqueueNDRangeKernel(command_queue, kernel, dimention, NULL, global_item_size4, local_item_size4, 0, NULL, &perf_event);
			// Capture performance event from target device. In this case the event is to retrive the execution time.
			ret2 = clWaitForEvents(1, &perf_event);
			ret2 = clGetEventProfilingInfo(perf_event, CL_PROFILING_COMMAND_START, sizeof(start), &start, NULL);
			ret2 = clGetEventProfilingInfo(perf_event, CL_PROFILING_COMMAND_END, sizeof(end), &end, NULL);
			std::cout << "OpenCL Tiled kernel matrix multiplication with tile_size=8 : " << (float)(end - start) / 1000000000 << " sec" << std::endl;

			// Read the memory buffer C from the device into the local variable C
			ret2 = clEnqueueReadBuffer(command_queue, c_mem_obj2, CL_TRUE, 0, SIZE2*SIZE2 * sizeof(int), C2, 0, NULL, NULL);

			// Make sure all the command in the command queue has been executed
			ret2 = clFinish(command_queue);

			validate = true;
			for (int j = 0; j < SIZE2; j++) {
				for (int i = 0; i < SIZE2; i++) {
					if (C2[j*SIZE2 + i] != C_seq2[j*SIZE2 + i])
						validate = false;
				}
			}

			if (validate == false)
				std::cout << "The results are mismatched !!" << std::endl;

			// Clean up
			ret2 = clReleaseKernel(kernel);
			ret2 = clReleaseProgram(program);
			ret2 = clReleaseMemObject(a_mem_obj2);
			ret2 = clReleaseMemObject(b_mem_obj2);
			ret2 = clReleaseMemObject(c_mem_obj2);
			ret2 = clReleaseCommandQueue(command_queue);
			ret2 = clReleaseContext(context);

			// Create an OpenCL context
			context = clCreateContext(NULL, 1, &device_id, NULL, NULL, &ret2);

			// Create a command queue with the capability of performance profiling for target device
			command_queue = clCreateCommandQueue(context, device_id, CL_QUEUE_PROFILING_ENABLE, &ret2);

			// Create memory buffers on the device for each matrix
			a_mem_obj2 = clCreateBuffer(context, CL_MEM_READ_WRITE, SIZE2*SIZE2 * sizeof(int), NULL, &ret2);
			b_mem_obj2 = clCreateBuffer(context, CL_MEM_READ_WRITE, SIZE2*SIZE2 * sizeof(int), NULL, &ret2);
			c_mem_obj2 = clCreateBuffer(context, CL_MEM_READ_WRITE, SIZE2*SIZE2 * sizeof(int), NULL, &ret2);

			// Copy the matrix A, B and C to each device memory counterpart
			ret2 = clEnqueueWriteBuffer(command_queue, a_mem_obj2, CL_TRUE, 0, SIZE2*SIZE2 * sizeof(int), A2, 0, NULL, NULL);
			ret2 = clEnqueueWriteBuffer(command_queue, b_mem_obj2, CL_TRUE, 0, SIZE2*SIZE2 * sizeof(int), B2, 0, NULL, NULL);
			ret2 = clEnqueueWriteBuffer(command_queue, c_mem_obj2, CL_TRUE, 0, SIZE2*SIZE2 * sizeof(int), C2, 0, NULL, NULL);

			// Create a program from the kernel source
			program = clCreateProgramWithSource(context, 1, (const char **)&source_str, (const size_t *)&source_size, &ret2);

			// Build and compile the OpenCL kernel program
			build_option = "-DTILE_SIZE=" + std::to_string(TILE_SIZE1);
			ret2 = clBuildProgram(program, 1, &device_id, build_option.c_str(), NULL, NULL);
			if (ret2 == CL_BUILD_PROGRAM_FAILURE) { // If compile failed, print the error message
				// Determine the size of the log
				size_t log_size;
				clGetProgramBuildInfo(program, device_id, CL_PROGRAM_BUILD_LOG, 0, NULL, &log_size);
				char *log = (char *)malloc(log_size);

				// Get the log and print it
				clGetProgramBuildInfo(program, device_id, CL_PROGRAM_BUILD_LOG, log_size, log, NULL);
				printf("%s\n", log);
			}

			// Create the OpenCL kernel
			
			kernel = clCreateKernel(program, "matrix_mul_tile", &ret2);

			// Set the arguments of the kernel
			ret2 = clSetKernelArg(kernel, 0, sizeof(cl_mem), (void *)&a_mem_obj2);
			ret2 = clSetKernelArg(kernel, 1, sizeof(cl_mem), (void *)&b_mem_obj2);
			ret2 = clSetKernelArg(kernel, 2, sizeof(cl_mem), (void *)&c_mem_obj2);

			//int dimention = 2; // In this example, We will use 2 dimention index
			size_t global_item_size5[] = { SIZE2, SIZE2, 1 };
			size_t local_item_size5[] = { TILE_SIZE1, TILE_SIZE1, 1 };

			//cl_event perf_event;
			//cl_ulong start, end;

			// Execute the OpenCL kernel
			ret2 = clEnqueueNDRangeKernel(command_queue, kernel, dimention, NULL, global_item_size5, local_item_size5, 0, NULL, &perf_event);
			// Capture performance event from target device. In this case the event is to retrive the execution time.
			ret2 = clWaitForEvents(1, &perf_event);
			ret2 = clGetEventProfilingInfo(perf_event, CL_PROFILING_COMMAND_START, sizeof(start), &start, NULL);
			ret2 = clGetEventProfilingInfo(perf_event, CL_PROFILING_COMMAND_END, sizeof(end), &end, NULL);
			std::cout << "OpenCL Tiled kernel matrix multiplication with tile_size=16 : " << (float)(end - start) / 1000000000 << " sec" << std::endl;

			// Read the memory buffer C from the device into the local variable C
			ret2 = clEnqueueReadBuffer(command_queue, c_mem_obj2, CL_TRUE, 0, SIZE2*SIZE2 * sizeof(int), C2, 0, NULL, NULL);

			// Make sure all the command in the command queue has been executed
			ret2 = clFinish(command_queue);

			validate = true;
			for (int j = 0; j < SIZE2; j++) {
				for (int i = 0; i < SIZE2; i++) {
					if (C2[j*SIZE2 + i] != C_seq2[j*SIZE2 + i])
						validate = false;
				}
			}

			if (validate == false)
				std::cout << "The results are mismatched !!" << std::endl;

			// Clean up
			ret2 = clReleaseKernel(kernel);
			ret2 = clReleaseProgram(program);
			ret2 = clReleaseMemObject(a_mem_obj2);
			ret2 = clReleaseMemObject(b_mem_obj2);
			ret2 = clReleaseMemObject(c_mem_obj2);
			ret2 = clReleaseCommandQueue(command_queue);
			ret2 = clReleaseContext(context);
			std::cout<<"-------------------------------------------"<<std::endl;
			//}
		}
        //delete [] platform_name;
    }
    

	

	std::cout << "Press Enter to finish..." << std::endl;
	getchar();
    return 0;
}
