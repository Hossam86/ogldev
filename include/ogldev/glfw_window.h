#include "GLFW/glfw3.h"
// #include <cstddef>
// #include <cstdlib>
#include <ogldev/utility.h>

static int glMajorVersion = 0;
static int glMinorVersion = 0;
namespace ogl
{

	static void
	glfw_lib_init()
	{
		if (!glfwInit())
		{
			OGLDEV_ERROR("Error initialization GLFW");
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

			return window;
		}
	}

} // namespace ogl
