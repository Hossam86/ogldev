#include "application.h"

using namespace picking;
int
main()
{
	// app instance
	Picking3d* app = new Picking3d();
	app->Init();

	// TODO: need to move this block to render context
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glFrontFace(GL_CW);
	glCullFace(GL_BACK);
	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);

	app->Run();

	delete app;
	return 0;
}