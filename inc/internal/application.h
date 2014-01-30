#ifndef _APPLICATION_H_
#define _APPLICATION_H_

class Font;
class Renderer;

class App
{
public:
	App(void);
	~App(void);

	void Init(int argc, char **argv);
	void Start(void);

private:
	Font     *font;
	Renderer *renderer;
};

#endif /* _APPLICATION_H_ */