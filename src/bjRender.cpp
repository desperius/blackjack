#include "main.h"
#include "bjRender.h"
#include "bjText.h"
#include "bjImage.h"

extern "C"
{
	#include "glew.h"
	#include "freeglut.h"
}

bjRender *bjRender::instance = nullptr;

bjRender::bjRender(void) : text(nullptr), image(nullptr) { }

bjRender::~bjRender(void) 
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

void bjRender::CallbackReshape(int w, int h)
{
	instance->Reshape(w, h);
}

void bjRender::CallbackDisplay(void)
{
	instance->Display();
}

void bjRender::CallbackMouse(int button, int state, int x, int y)
{
	instance->Mouse(button, state, x, y);
}

void bjRender::Init(int argc, char **argv)
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

    if (glewIsSupported("GL_VERSION_3_3"))
    {
        std::cout << "OpenGL 3.3 supported!" << std::endl;
    }

	//> Set glut functions
	glutReshapeFunc(bjRender::CallbackReshape);
	glutDisplayFunc(bjRender::CallbackDisplay);
	glutMouseFunc(bjRender::CallbackMouse);

    text = new (std::nothrow) bjText();
    text->Init();
    text->Load();

    image = new (std::nothrow) bjImage();
    image->Init();
    image->Load(IMAGE_DIR "background.png");

    const GLubyte *gpu = glGetString(GL_RENDERER);
    std::cout << gpu << std::endl;
}
void bjRender::Reshape(int w, int h)
{
	if (0 == h)
	{
		h = 1;
	}

	glViewport(0, 0, w, h);
}

void bjRender::Display(void)
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

void bjRender::Mouse(int button, int state, int x, int y)
{
	if (button == GLUT_RIGHT_BUTTON && state == GLUT_UP)
	{
		std::cout << "Mouse position: (" << x << ", " << y << ")" << std::endl;
	}
}

void bjRender::Start(void)
{
	glutMainLoop();
}