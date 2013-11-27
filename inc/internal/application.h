#ifndef _APPLICATION_H_
#define _APPLICATION_H_

class Renderer;

class App
{
public:
	App();
	virtual ~App();

	void Init(int argc, char **argv);
	void Start(void);

private:
	Renderer *renderer;
};

#endif /* _APPLICATION_H_ */