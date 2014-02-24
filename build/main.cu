// main.cu
#include <stdio.h>
#include <cuda_runtime.h>

__global__ void kernel(void)
{
    //
}

void launch(void)
{
   dim3 threads(2, 1);
   dim3 blocks(1, 1);

   kernel<<<blocks, threads>>>();

   int num_devices;
   int device;
   cudaGetDeviceCount(&num_devices);
   printf("Device count: %d!\n", num_devices);

   if (num_devices > 1) 
   {
       int max_multiprocessors = 0;
       int max_device = 0;
       
       for (device = 0; device < num_devices; ++device) 
       {
           cudaDeviceProp properties;
           cudaGetDeviceProperties(&properties, device);
           printf("Device name: %s!\n", properties.name);

           if (max_multiprocessors < properties.multiProcessorCount) 
           {
               max_multiprocessors = properties.multiProcessorCount;
               max_device = device;
           }
       }

       cudaSetDevice(max_device);
   }
}