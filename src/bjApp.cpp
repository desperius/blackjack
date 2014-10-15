#include "main.h"
#include "bjRender.h"
#include "bjApp.h"

bjApp::bjApp(void) : render(nullptr) { }

bjApp::~bjApp(void)
{
	if (render)
	{
		delete render;
		render = nullptr;
	}
}

void bjApp::Init(int argc, char **argv)
{
    render = new (std::nothrow) bjRender();
	render->Init(argc, argv);
}

void bjApp::Start(void)
{
	render->Start();
}