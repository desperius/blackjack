#include "main.h"
#include "font.h"
#include "renderer.h"
#include "application.h"

App::App(void) : renderer(nullptr)
{
	
}

App::~App(void)
{
	if (font)
	{
		delete font;
		font = nullptr;
	}

	if (renderer)
	{
		delete renderer;
		renderer = nullptr;
	}
}

void App::Init(int argc, char **argv)
{
	font = new (std::nothrow) Font();
	font->Init();

	renderer = new (std::nothrow) Renderer();
	renderer->Init(argc, argv);
}

void App::Start(void)
{
	renderer->Start();
}