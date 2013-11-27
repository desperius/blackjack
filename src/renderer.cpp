#include "main.h"
#include "renderer.h"

extern "C"
{
	#include "glew.h"
	#include "freeglut.h"
}

Renderer *Renderer::instance = nullptr;

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
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.5, 1.0, 0.0, 1.0);
	glutSwapBuffers();
}

void Renderer::Start(void)
{
	glutMainLoop();
}