#include "main.h"
#include "renderer.h"
#include "text.h"
#include "image.h"

extern "C"
{
	#include "glew.h"
	#include "freeglut.h"
}

Renderer *Renderer::instance = nullptr;

Renderer::Renderer(void) : text(nullptr), image(nullptr) { }

Renderer::~Renderer(void) 
{
    if (text)
    {
        delete text;
        text = nullptr;
    }

    if (image)
    {
        delete image;
        image = nullptr;
    }
}

void Renderer::CallbackReshape(int w, int h)
{
	instance->Reshape(w, h);
}

void Renderer::CallbackDisplay(void)
{
	instance->Display();
}

void Renderer::Init(int argc, char **argv)
{
	instance = this;

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA | GLUT_MULTISAMPLE);
	glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
	glutCreateWindow("Black Jack");
	glutCreateMenu(NULL);


	if (GLEW_OK != glewInit())
	{
		std::cout << "WTF? GLEW initialization problems!" << std::endl;
	}

	//> Set glut functions
	glutReshapeFunc(Renderer::CallbackReshape);
	glutDisplayFunc(Renderer::CallbackDisplay);

    text = new (std::nothrow) Text();
    text->Init();
    text->Load();

    image = new (std::nothrow) Image();
    image->Init();
    image->Load(IMAGE_DIR "box.png");
}
void Renderer::Reshape(int w, int h)
{
	if (0 == h)
	{
		h = 1;
	}

	glViewport(0, 0, w, h);
}

void Renderer::Display(void)
{
	//std::cout << "Display function was called!" << std::endl;
	glClearColor(1.0, 1.0, 1.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    if (image)
    {
        image->Draw();
    }

    if (text)
    {
        text->Draw();
    }

	glutSwapBuffers();
}

void Renderer::Start(void)
{
	glutMainLoop();
}