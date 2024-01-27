#include "application.h"

using namespace picking;

// app instance;
Picking3d* app = NULL;

static void
KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	app->KeyboardCB(key, action);
}

static void
CursorPosCallback(GLFWwindow* window, double x, double y)
{
	app->PassiveMouseCB((int)x, (int)y);
}

static void
MouseButtonCallback(GLFWwindow* window, int Button, int Action, int Mode)
{
	double x, y;

	glfwGetCursorPos(window, &x, &y);

	app->MouseCB(Button, Action, (int)x, (int)y);
}
int
main()
{
	// app instance
	app = new Picking3d();
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