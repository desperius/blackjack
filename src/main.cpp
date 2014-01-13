#include "main.h"
#include "application.h"
#include "unzip.h"
#include "zip.h"

//> DevIL library
#include "il.h"
#include "ilu.h"
#include "ilut.h"

#define TEXTURE_PATH "../res/textures/"

uLong size_buf;
void *buf;

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

int main(int argc, char **argv)
{
	std::cout << argv[argc - 1] << std::endl;
	read_zip();

	App app;
	app.Init(argc, argv);
	app.Start();

	system("PAUSE");
	return 0;
}