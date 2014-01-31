#include "main.h"
#include "renderer.h"
#include "application.h"

App::App(void) : renderer(nullptr) { }

App::~App(void)
{
	if (renderer)
	{
		delete renderer;
		renderer = nullptr;
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