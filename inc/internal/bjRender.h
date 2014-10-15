#ifndef _BJ_RENDER_H_
#define _BJ_RENDER_H_

//const int WINDOW_WIDTH  = 800;
//const int WINDOW_HEIGHT = 600;

class bjText;
class bjImage;

class bjRender
{
public:
	bjRender(void);
	~bjRender(void);

	void Init(int argc, char **argv);
	void Start(void);

private:
	static void CallbackReshape(int w, int h);
	static void CallbackDisplay(void);
	static void CallbackMouse(int button, int state, int x, int y);

	void Reshape(int w, int h);
	void Display(void);
	void Mouse(int button, int state, int x, int y);

private:
	static bjRender   *instance;
    bjText            *text;
    bjImage           *image;
};

#endif /* _BJ_RENDER_H_ */