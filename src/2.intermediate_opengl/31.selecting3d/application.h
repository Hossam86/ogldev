#pragma once

#include <ogldev/camera.h>
#include <ogldev/glfw_window.h>
#include <ogldev/lighting2.h>

#include "picking_technique.h"
#include "picking_texture.h"
#include "simple_color_technique.h"

// Callbacks
void
KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);

void
CursorPosCallback(GLFWwindow* window, double x, double y);

void
MouseButtonCallback(GLFWwindow* window, int Button, int Action, int Mode);

struct MouseButton
{
	bool IsPressed = false;
	int x;
	int y;
};

class Picking3d
{
private:
	GLFWwindow* window = NULL;
	ogl::LightingTechnique * m_lightingEffect;
	PickingTechnique m_pickingEffect;
	SimpleColorTechnique m_simpleColorEffect;
	ogl::BasicCamera* m_pGameCamera = NULL;
	ogl::DirectionalLight m_directionalLight;
	ogl::BasicMesh* pMesh = NULL;
	Picking_Texture m_pickingTexture;
	ogl::Vector3f m_worldPos[3];
	MouseButton m_leftMouseButton;
	uint width;
	uint height;

public:
	Picking3d();
	virtual ~Picking3d();

	void
	Init();

	void
	Run();

	void
	RenderSceneCB();

	void
	PickingPhase();

	void
	RenderPhase();

	void
	KeyboardCB(uint key, int state);

	void
	PassiveMouseCB(int x, int y);

	void
	MouseCB(int button, int action, int x, int y);

private:
	void
	CreateWindow();

	void
	InitCallBacks();

	void
	InitCamera();

	void
	InitShaders();

	void
	InitMesh();
};
