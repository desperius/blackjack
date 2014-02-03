#ifndef _RENDERER_H_
#define _RENDERER_H_

const int WINDOW_WIDTH  = 800;
const int WINDOW_HEIGHT = 600;

class Text;
class Image;

class Renderer
{
public:
	Renderer(void);
	~Renderer(void);

	void Init(int argc, char **argv);
	void Start(void);

private:
	static void CallbackReshape(int w, int h);
	static void CallbackDisplay(void);

	void Reshape(int w, int h);
	void Display(void);

private:
	static Renderer *instance;
    Text            *text;
    Image           *image;
};

#endif /* _RENDERER_H_ */