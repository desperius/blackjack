#ifndef _BJ_APP_H_
#define _BJ_APP_H_

class bjRender;

class bjApp
{
public:
	bjApp(void);
	~bjApp(void);

	void Init(int argc, char **argv);
	void Start(void);

private:
    bjRender *render;
};

#endif /* _BJ_APP_H_ */