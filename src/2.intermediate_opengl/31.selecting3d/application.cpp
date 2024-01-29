#include "picking_texture.h"

#include "application.h"
#include <ogldev/basic_mesh.h>
#include <ogldev/camera.h>
#include <ogldev/engine_common.h>
#include <ogldev/glfw_window.h>
#include <ogldev/math3d.h>
#include <ogldev/utility.h>
#include <ogldev/world_transform.h>

using namespace ogl;

static void
glfw_lib_init()
{
	if (!glfwInit())
	{
		OGLDEV_ERROR0("Error initialization GLFW");
	}

	int Major, Minor, Rev;
	glfwGetVersion(&Major, &Minor, &Rev);
	printf("GLFW: %d.%d.%d initalized\n", Major, Minor, Rev);
}

static void
enable_debug_output()
{
	glEnable(GL_DEBUG_OUTPUT);
	glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
	glDebugMessageCallback(glDebugOutput, nullptr);
	glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_HIGH, 0, nullptr, GL_TRUE);
}

// gLad:should be initialized before calling any OpenGL function.
// hint: Must be done after glfw is initialized!
bool
init_glad()
{

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return false;
	}
	return true;
}

// API
int
GetGLMajorVersion()
{
	return glMajorVersion;
}

int
GetGLMinorVersion()
{
	return glMinorVersion;
}

int
IsGLVersionHigher(int MajorVer, int MinorVer)
{
	return ((glMajorVersion >= MajorVer) && (glMinorVersion >= MinorVer));
}

GLFWwindow*
glfw_init(int major_ver, int minor_ver, int width, int height, bool is_full_screen, const char* title)
{
	glfw_lib_init();

	GLFWmonitor* monitor = is_full_screen ? glfwGetPrimaryMonitor() : NULL;

	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	if (major_ver > 0)
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, major_ver);
	if (minor_ver > 0)
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, minor_ver);

	GLFWwindow* window = glfwCreateWindow(width, height, title, monitor, NULL);

	if (!window)
	{
		const char* pDesc = NULL;
		int error_code = glfwGetError(&pDesc);
		OGLDEV_ERROR("glfw: failed to create window! %s", pDesc);
		exit(1);

		glfwMakeContextCurrent(window);

		// The following functions must be called after the context is made current
		glGetIntegerv(GL_MAJOR_VERSION, &glMajorVersion);
		glGetIntegerv(GL_MINOR_VERSION, &glMinorVersion);

		if (major_ver > 0)
		{
			if (major_ver != glMajorVersion)
			{
				OGLDEV_ERROR(
					"Requested major version %d is not the same as created version %d",
					major_ver,
					glMajorVersion);
				exit(0);
			}
		}

		if (minor_ver > 0)
		{
			if (minor_ver != glMinorVersion)
			{
				OGLDEV_ERROR(
					"Requested minor version %d is not the same as created version %d",
					minor_ver,
					glMinorVersion);
				exit(0);
			}
		}

		// glfw should be initialized before glad !
		init_glad();

		enable_debug_output();

		glfwSwapInterval(1);
	}
	return window;
}

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
	window = glfw_init(0, 0, width, height, false, "31.Picking3d");
}

void
Picking3d::InitCallBacks()
{
	glfwSetKeyCallback(window, KeyCallback);
	glfwSetMouseButtonCallback(window, MouseButtonCallback);
	glfwSetCursorPosCallback(window, CursorPosCallback);
}

void
Picking3d::InitMesh()
{
	pMesh = new BasicMesh();
	pMesh->LoadMesh("../Resources/spider.obj");
	ogl::WorldTrans& world_transform = pMesh->GetWorldTransform();
	world_transform.SetScale(0.1f);
	world_transform.SetRotation(0.0f, 90.f, 0.0f);
}

void
Picking3d::InitCamera()
{
	Vector3f pos(0.0f, 5.0f, -22.0f);
	Vector3f Target(0.0f, -0.2f, 1.0f);
	Vector3f Up(0.0f, 1.0f, 0.0f);

	float FOV = 45.0f;
	float zNear = 0.1f;
	float zFar = 100.f;
	PersProjInfo persProjInfo = {FOV, (float)width, (float)height, zNear, zFar};
	m_pGameCamera = new ogl::BasicCamera(persProjInfo, pos, Target, Up);

	if (!m_lightingEffect.Init())
	{
		printf("Error Initializing The Lighting Technique ");
		exit(1);
	}
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

void
Picking3d::InitShaders()
{
	m_lightingEffect.Enable();
	m_lightingEffect.SetTextureUnit(COLOR_TEXTURE_UNIT_INDEX);
	m_lightingEffect.SetSpecularExponentTextureUnit(SPECULAR_EXPONENT_UNIT_INDEX);
	m_lightingEffect.SetMaterial(pMesh->GetMaterial());
	m_pickingTexture.init(width, height);

	if (!m_pickingEffect.Init())
		exit(1);
	if (!m_simpleColorEffect.Init())
		exit(1);
}

void
Picking3d::KeyboardCB(uint key, int state)
{
	switch (key)
	{
	case 'a':
		m_directionalLight.AmbientIntensity += 0.05f;
		break;
	case 's':
		m_directionalLight.AmbientIntensity -= 0.05f;
		break;
	case 'z':
		m_directionalLight.DiffuseIntensity += 0.05f;
		break;
	case 'x':
		m_directionalLight.DiffuseIntensity -= 0.05f;
		break;
	default:
		m_pGameCamera->OnKeyboard(key);
	}
}

void
Picking3d::PassiveMouseCB(int x, int y)
{
	m_pGameCamera->OnMouse(x, y);
}

void
Picking3d ::MouseCB(int button, int action, int x, int y)
{
	if (button == GLFW_MOUSE_BUTTON_LEFT)
	{
		m_leftMouseButton.IsPressed = (action == GLFW_PRESS);
		m_leftMouseButton.x = x;
		m_leftMouseButton.y = y;
	}
}

void
Picking3d::RenderPhase()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	ogl::WorldTrans& wt = pMesh->GetWorldTransform();
	Matrix4f view = m_pGameCamera->GetViewMatrix();
	Matrix4f projection = m_pGameCamera->GetProjectionMat();

	// If the left mouse button is clicked check if it hit triangle and color it red
	int clicked_object_id = -1;
	if (m_leftMouseButton.IsPressed)
	{
		Pixel_Info px = m_pickingTexture.read_pixel(m_leftMouseButton.x, height - m_leftMouseButton.y - 1);
		if (px.object_id != 0)
		{
			// Compensate for the SetObjectindex call in the picking phase
			clicked_object_id = px.object_id - 1;
			assert(clicked_object_id < ARRAY_SIZE_IN_ELEMENTS(m_worldPos));
			m_simpleColorEffect.Enable();
			wt.SetPosition(m_worldPos[clicked_object_id]);
			Matrix4f world = wt.GetMatrix();
			Matrix4f WVP = projection * view * world;
			m_simpleColorEffect.SetWVP(WVP);
			pMesh->Render(px.draw_id, px.prim_id);
		}
	}

	// Render the objects as usual
	m_lightingEffect.Enable();
	for (unsigned int i = 0; i < ARRAY_SIZE_IN_ELEMENTS(m_worldPos); i++)
	{
		wt.SetPosition(m_worldPos[i]);
		Matrix4f World = wt.GetMatrix();
		Matrix4f WVP = projection * view * World;
		m_lightingEffect.SetWVP(WVP);
		Vector3f CameraLocalPos3f = wt.WorldPosToLocalPos(m_pGameCamera->GetPos());
		m_lightingEffect.SetCameraLocalPos(CameraLocalPos3f);
		m_directionalLight.CalcLocalDirection(wt);
		m_lightingEffect.SetDirectionalLight(m_directionalLight);

		if (i == clicked_object_id)
		{
			m_lightingEffect.SetColorMod(ogl::Vector4f(0.0f, 1.0, 0.0, 1.0f));
		}
		else
		{
			m_lightingEffect.SetColorMod(ogl::Vector4f(1.0f, 1.0, 1.0, 1.0f));
		}

		pMesh->Render(NULL);
	}
}

void
Picking3d::Run()
{
	while (!glfwWindowShouldClose(window))
	{
		RenderSceneCB();
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
}