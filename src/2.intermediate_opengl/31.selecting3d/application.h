#include "ogldev/math3d.h"

#include "simple_color_technique.h"
#include <GLFW/glfw3.h>
#include <ogldev/basic_mesh.h>
#include <ogldev/camera.h>
#include <ogldev/lighting2.h>

#include "picking_technique.h"
#include "picking_texture.h"

namespace picking
{
	// callbacks
	void
	key_callback();

	void
	cursor_pos_callback();

	void
	mouse_button_callback();

	struct
	{
		bool IsPressed = false;
		int x;
		int y;
	} m_leftMouseButton;

	class Picking3d
	{
	private:
		GLFWwindow* window = NULL;
		LightingTechnique m_lightingEffect;
		PickingTechnique m_pickingEffect;
		SimpleColorTechnique m_simpleColorEffect;
		ogl::BasicCamera* m_pGameCamera = NULL;
		DirectionalLight m_directionalLight;
		BasicMesh* pMesh = NULL;
		Picking_Texture m_pickingTexture;
		Vector3f m_worldPos[3];
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
		PassiveMouseCB();

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

}