#ifndef _RENDERER_H_
#define _RENDERER_H_

const int WINDOW_WIDTH  = 800;
const int WINDOW_HEIGHT = 600;

class Font;

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
    void SetFont(Font *pointer) { font = pointer; }

private:
	static Renderer *instance;
    Font *font;
};

#endif /* _RENDERER_H_ */