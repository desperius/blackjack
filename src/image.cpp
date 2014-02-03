#include "main.h"
#include "image.h"

extern "C"
{
	#include "glew.h"
}

#include "il.h"
#include "ilu.h"
#include "ilut.h"

Image::Image(void) : 
	image_id(0),
	data_size(0),
	image_data(nullptr),
	width(0),
	height(0) { }

Image::~Image(void) 
{
	if (image_data)
	{
		delete image_data;
		image_data = nullptr;
	}
}

void Image::Init(void)
{
    // Initialize DevIL
    ilInit();
    iluInit();
    ilutInit();

    // Initialize the renderer
    ilutRenderer(ILUT_OPENGL);

    // Generate an image
    ilGenImages(1, &image_id);

    // Select the current image
    ilBindImage(image_id);

    // Enable an origin state
    ilEnable(IL_ORIGIN_SET);

    // Set default image origin
    ilOriginFunc(IL_ORIGIN_LOWER_LEFT);
}

void Image::Load(const char *path)
{
    // Load an image into memory
    ILboolean error = ilLoadImage(path);

    if (!error)
    {
        std::cout << "DevIL image loading: WTF?" << std::endl;
    }

    // Convert image into specific format
    error = ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE);

    if (!error)
    {
        std::cout << "DevIL convert image: WTF?" << std::endl;
    }

    data_size = ilGetInteger(IL_IMAGE_SIZE_OF_DATA);
    image_data = new (std::nothrow) GLubyte[data_size];
    std::memset(image_data, 0, data_size);
    std::memcpy(image_data, ilGetData(), data_size);
	width  = ilGetInteger(IL_IMAGE_WIDTH);
	height = ilGetInteger(IL_IMAGE_HEIGHT);

    // Delete an image
    ilDeleteImages(1, &image_id);
}