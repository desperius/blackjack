#include "main.h"
#include "application.h"

int main(int argc, char **argv)
{
	std::cout << argv[argc - 1] << std::endl;
	App app;
	app.Init(argc, argv);
	app.Start();

	system("PAUSE");
	return 0;
}