#include "main.h"
#include "font.h"
#include "ft2.h"
#include FT_FREETYPE_H

Font::Font(void) : library(nullptr), face(nullptr) { }

Font::~Font(void) 
{
	FT_Done_FreeType(library);
	library = nullptr;
}

void Font::Init(void)
{
	FT_Error error = FT_Init_FreeType(&library);
	
	if (error)
	{
		std::cout << "FT library init WTF?" << std::endl;
	}
}