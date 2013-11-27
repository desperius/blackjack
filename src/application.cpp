#include "main.h"
#include "renderer.h"
#include "application.h"

App::App() : renderer(nullptr)
{
	
}

App::~App()
{
	if (renderer)
	{
		delete renderer;
		renderer = NULL;
	}
}

void App::Init(int argc, char **argv)
{
	renderer = new (std::nothrow) Renderer();
	renderer->Init(argc, argv);
}

void App::Start(void)
{
	renderer->Start();
}