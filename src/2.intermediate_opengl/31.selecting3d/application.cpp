#include "application.h"
#include "GLFW/glfw3.h"
#include "ogldev/math3d.h"
#include "ogldev/utility.h"
#include "ogldev/world_transform.h"
#include <OpenGL/OpenGL.h>
#include <ogldev/glfw_window.h>

namespace picking
{

	Picking3d::Picking3d()
	{
		m_directionalLight.Color = Vector3f(1.0f, 1.0f, 1.0f);
		m_directionalLight.AmbientIntensity = 3.0f;
		m_directionalLight.DiffuseIntensity = 0.1f;
		m_directionalLight.WorldDirection = Vector3f(-1.0f, 0.0, 0.0);

		// the same mesh will rendered  all the following location;
		m_worldPos[0] = Vector3f(-10.0f, 0.0f, 5.0f);
		m_worldPos[1] = Vector3f(10.0f, 0.0f, 5.0f);
		m_worldPos[2] = Vector3f(0.0f, 2.0f, 20.0f);

		// window size
		width = 1920;
		height = 1080;
	}

	Picking3d::~Picking3d()
	{
		SAFE_DELETE(m_pGameCamera);
		SAFE_DELETE(pMesh);
	}

	void
	Picking3d::Init()
	{
		CreateWindow();
		InitCallBacks();
		InitCamera();
		InitMesh();
		InitShaders();
	}

	void
	Picking3d::CreateWindow()
	{
		window = ogl::glfw_init(0, 0, width, height, false, "31.Picking3d");
	}

	void
	Picking3d::InitCallBacks()
	{
	}

	void
	Picking3d::RenderSceneCB()
	{
		m_pGameCamera->OnRender();

		if (m_leftMouseButton.IsPressed)
			PickingPhase();

		RenderPhase();
	}

	void
	Picking3d::PickingPhase()
	{
		m_pickingTexture.enable_writing();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		m_pickingEffect.Enable();

		ogl::WorldTrans& world_transform = pMesh->GetWorldTransform();
		Matrix4f view = m_pGameCamera->GetViewMatrix();
		Matrix4f Projection = m_pGameCamera->GetViewProjMatrix();
		for (int i = 0; i < ARRAY_SIZE_IN_ELEMENTS(m_worldPos); ++i)
		{
			world_transform.SetPosition(m_worldPos[i]);

			// Background is zero the real objects  start 1
			m_pickingEffect.SetObjectIndex(i + 1);
			Matrix4f world = world_transform.GetMatrix();
			Matrix4f WVP = Projection * view * world;
			m_pickingEffect.SetWVP(WVP);
			pMesh->Render(&m_pickingEffect);
		}
	}

}
