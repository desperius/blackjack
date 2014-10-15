#include "main.h"
#include "bjApp.h"
#include "unzip.h"
#include "zip.h"

//> DevIL library
#include "il.h"
#include "ilu.h"
#include "ilut.h"

// OpenCL
#define __CL_ENABLE_EXCEPTIONS
#include "cl.hpp"

#define TEXTURE_PATH "../res/textures/"

static const std::string source =
    "#if defined(cl_khr_fp64)\n"
    "#pragma OPENCL EXTENSION cl_khr_fp64: enable\n"
    "#elif defined(cl_amd_fp64)\n"
    "#pragma OPENCL EXTENSION cl_amd_fp64: enable\n"
    "#else\n"
    "#error double precision is not supported\n"
    "#endif\n"
    "__kernel void add(\n"
    "       ulong w,\n"
    "       ulong h,\n"
    "       __global double *c\n"
    "       )\n"
    "{\n"
    "    size_t i = get_global_id(0);\n"
    "    size_t j = get_global_id(1);\n"
    "    if (i < h && j < w)\n" 
    "    {\n"
    "       c[i * h + j] = i * h + j;\n"
    "    }\n"
    "}\n";

uLong size_buf;
void *buf;

void read_zip(void);
void init_ocl(void);

int main(int argc, char **argv)
{
	std::cout << argv[argc - 1] << std::endl;
	read_zip();
    //init_ocl();

	bjApp app;
	app.Init(argc, argv);
	app.Start();

	system("PAUSE");
	return 0;
}

void read_zip(void)
{
	int error = UNZ_OK;
	std::string path(TEXTURE_PATH "box.zip");
	unz_file_info file_info;
	char filename[256] = {0};

	unzFile unzfile = unzOpen(path.c_str());

	if (NULL == unzfile)
	{
		std::cout << "WTF? Cannot open file!" << std::endl;
	}

	error = unzLocateFile(unzfile, "box.png", 1);

	if (error != UNZ_OK)
	{
		std::cout << "WTF? File not found in!" << std::endl;
	}

	error = unzGetCurrentFileInfo(unzfile, &file_info, filename, sizeof(filename), NULL, 0, NULL, 0);

	if (error != UNZ_OK)
	{
		std::cout << "WTF? Cannot get file info!" << std::endl;
	}

	size_buf = file_info.uncompressed_size;
	buf = new (std::nothrow) char[size_buf + 1];

	if (nullptr == buf)
	{
		std::cout << "WTF? Cannot allocate the memory!" << std::endl;
	}

	error = unzOpenCurrentFile(unzfile);

	if (error != UNZ_OK)
	{
		std::cout << "WTF? Cannot open current file!" << std::endl;
	}

	error = unzReadCurrentFile(unzfile, buf, size_buf);

	if (error <= 0)
	{
		std::cout << "WTF? Cannot read current file!" << std::endl;
	}
	else
	{
		std::cout << "Size of current file: " << error << std::endl;
	}

	//> Initialize DevIL
	ilInit();
	iluInit();
	ilutInit();

	ILuint image_id = 0;

	//> Initialize renderer which used by OpenILUT
	ilutRenderer(ILUT_OPENGL);

	//> Generate an image id
	image_id = ilGenImage();
}

void init_ocl(void)
{
    const std::size_t w = 4;
    const std::size_t h = 4;

    try
    {
       std::vector<cl::Platform> platforms;
       cl::Platform::get(&platforms);

       if (platforms.empty())
       {
          std::cerr << "OCL platforms not found!" << std::endl;

          return;
       }
       else
       {
          std::cout << "Platform name: " << platforms[0].getInfo<CL_PLATFORM_NAME>() << std::endl;
          std::cout << "Platform name: " << platforms[1].getInfo<CL_PLATFORM_NAME>() << std::endl;
       }

       std::vector<cl::Device> devices;
       //platforms[0].getDevices(CL_DEVICE_TYPE_GPU, &devices);
       platforms[1].getDevices(CL_DEVICE_TYPE_CPU, &devices);

       if (devices.empty())
       {
          std::cerr << "OCL devices not found!" << std::endl;
       }
       else
       {
          std::cout << devices[0].getInfo<CL_DEVICE_NAME>() << std::endl;
       }

       cl::Context context(devices);

       cl::CommandQueue queue(context, devices[0]);
       cl::Program::Sources sources(1, std::make_pair(source.c_str(), source.length()));

       cl::Program program(context, source);
       program.build(devices);

       cl::Kernel kernel(program, "add");

       std::vector<double> c(w * h);

       cl::Buffer C(context, CL_MEM_WRITE_ONLY | CL_MEM_COPY_HOST_PTR, c.size() * sizeof(double), c.data());

       kernel.setArg(0, static_cast<cl_ulong>(w));
       kernel.setArg(1, static_cast<cl_ulong>(h));
       kernel.setArg(2, C);

       cl::NDRange global(h, w);
       queue.enqueueNDRangeKernel(kernel, cl::NullRange, global, cl::NullRange);
       queue.enqueueReadBuffer(C, CL_TRUE, 0, c.size() * sizeof(double), c.data());

       for (std::size_t i = 0; i < h; ++i)
       {
          for (std::size_t j = 0; j < w; ++j)
          {
             std::cout << c[i * h + j] << " ";
          }
          std::cout << std::endl;
       }
       c.clear();

       for (std::size_t i = 0; i < h; ++i)
       {
          for (std::size_t j = 0; j < w; ++j)
          {
             c[i * h + j] = i * h + j;
          }
       }

       for (std::size_t i = 0; i < h; ++i)
       {
          for (std::size_t j = 0; j < w; ++j)
          {
             std::cout << c[i * h + j] << " ";
          }
          std::cout << std::endl;
       }
    }
    catch (...)
    {
       std::cout << "Exception throw!" << std::endl;
    }
}