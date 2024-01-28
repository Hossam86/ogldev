#pragma once

#include <fstream>
#include <iostream>
#ifdef _WIN32
#include <Windows.h>
#else
#include <sys/time.h>
#endif

#ifndef VULKAN
#include <glad/glad.h>
#endif
#include <errno.h>
#include <fcntl.h>
#include <stdarg.h>
#include <sys/stat.h>
#include <sys/types.h>

#ifndef _WIN64
#include <unistd.h>
#endif
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <time.h>
#include <vector>

#include "types.h"

using namespace std;

bool
ReadFile(const char* fileName, string& outFile);
char*
ReadBinaryFile(const char* pFileName, int& size);

void
WriteBinaryFile(const char* pFilename, const void* pData, int size);

void
OgldevError(const char* pFileName, uint line, const char* msg, ...);
void
OgldevFileError(const char* pFileName, uint line, const char* pFileError);

#define OGLDEV_ERROR0(msg) OgldevError(__FILE__, __LINE__, msg)
#define OGLDEV_ERROR(msg, ...) OgldevError(__FILE__, __LINE__, msg, __VA_ARGS__)
#define OGLDEV_FILE_ERROR(FileError) OgldevFileError(__FILE__, __LINE__, FileError);

#define ZERO_MEM(a) memset(a, 0, sizeof(a))
#define ZERO_MEM_VAR(var) memset(&var, 0, sizeof(var))
#define ARRAY_SIZE_IN_ELEMENTS(a) (sizeof(a) / sizeof(a[0]))

#ifndef MAX
#define MAX(a, b) ((a) > (b) ? (a) : (b))
#endif

#ifdef _WIN64
#define SNPRINTF _snprintf_s
#define VSNPRINTF vsnprintf_s
#define RANDOM rand
#define SRANDOM srand((unsigned)time(NULL))
#pragma warning(disable : 4566)
#else
#define SNPRINTF snprintf
#define VSNPRINTF vsnprintf
#define RANDOM random
#define SRANDOM srandom(getpid())
#endif

#define INVALID_UNIFORM_LOCATION 0xffffffff
#define INVALID_OGL_VALUE 0xffffffff

#define NUM_CUBE_MAP_FACES 6

#define SAFE_DELETE(p) \
	if (p)             \
	{                  \
		delete p;      \
		p = NULL;      \
	}

#define GLExitIfError                                                           \
	{                                                                           \
		GLenum Error = glGetError();                                            \
                                                                                \
		if (Error != GL_NO_ERROR)                                               \
		{                                                                       \
			printf("OpenGL error in %s:%d: 0x%x\n", __FILE__, __LINE__, Error); \
			exit(0);                                                            \
		}                                                                       \
	}

#define GLCheckError() (glGetError() == GL_NO_ERROR)

long long
GetCurrentTimeMillis();

#define ASSIMP_LOAD_FLAGS (aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_JoinIdenticalVertices)

#define NOT_IMPLEMENTED                                            \
	printf("Not implemented case in %s:%d\n", __FILE__, __LINE__); \
	exit(0);

void
gl_check_error(const char* function, const char* file, int line);

#define CHECK_GL_ERRORS

#ifdef CHECK_GL_ERRORS
#define GCE gl_check_error(__FUNCTION__, __FILE__, __LINE__);
#else
#define GCE
#endif

void
glDebugOutput(
	GLenum source,
	GLenum type,
	unsigned int id,
	GLenum severity,
	GLsizei length,
	const char* message,
	const void* userParam);

string
GetDirFromFilename(const string& Filename);

#define MAX_BONES (200)

#define CLAMP(Val, Start, End) Val = std::min(std::max((Val), (Start)), (End));

int
GetGLMajorVersion();
int
GetGLMinorVersion();

// int IsGLVersionHigher(int MajorVer, int MinorVer);

// API Imp

bool
ReadFile(const char* pFileName, string& outFile)
{
	ifstream f(pFileName);

	bool ret = false;

	if (f.is_open())
	{
		string line;
		while (getline(f, line))
		{
			outFile.append(line);
			outFile.append("\n");
		}

		f.close();

		ret = true;
	}
	else
	{
		OGLDEV_FILE_ERROR(pFileName);
	}

	return ret;
}

#ifdef _WIN32
char*
ReadBinaryFile(const char* pFilename, int& size)
{
	FILE* f = NULL;

	errno_t err = fopen_s(&f, pFilename, "rb");

	if (!f)
	{
		char buf[256] = {0};
		strerror_s(buf, sizeof(buf), err);
		OGLDEV_ERROR("Error opening '%s': %s\n", pFilename, buf);
		exit(0);
	}

	struct stat stat_buf;
	int error = stat(pFilename, &stat_buf);

	if (error)
	{
		char buf[256] = {0};
		strerror_s(buf, sizeof(buf), err);
		OGLDEV_ERROR("Error getting file stats: %s\n", buf);
		return NULL;
	}

	size = stat_buf.st_size;

	char* p = (char*)malloc(size);
	assert(p);

	size_t bytes_read = fread(p, 1, size, f);

	if (bytes_read != size)
	{
		char buf[256] = {0};
		strerror_s(buf, sizeof(buf), err);
		OGLDEV_ERROR("Read file error file: %s\n", buf);
		exit(0);
	}

	fclose(f);

	return p;
}
#else
char*
ReadBinaryFile(const char* pFilename, int& size)
{
	FILE* f = fopen(pFilename, "rb");

	if (!f)
	{
		OGLDEV_ERROR("Error opening '%s': %s\n", pFilename, strerror(errno));
		exit(0);
	}

	struct stat stat_buf;
	int error = stat(pFilename, &stat_buf);

	if (error)
	{
		OGLDEV_ERROR("Error getting file stats: %s\n", strerror(errno));
		return NULL;
	}

	size = stat_buf.st_size;

	char* p = (char*)malloc(size);
	assert(p);

	size_t bytes_read = fread(p, 1, size, f);

	if (bytes_read != size)
	{
		OGLDEV_ERROR("Read file error file: %s\n", strerror(errno));
		exit(0);
	}

	fclose(f);

	return p;
}

void
WriteBinaryFile(const char* pFilename, const void* pData, int size)
{
	FILE* f = fopen(pFilename, "wb");

	if (!f)
	{
		OGLDEV_ERROR("Error opening '%s': %s\n", pFilename, strerror(errno));
		exit(0);
	}

	int bytes_written = fwrite(pData, 1, size, f);

	if (bytes_written != size)
	{
		OGLDEV_ERROR("Error write file: %s\n", strerror(errno));
		exit(0);
	}

	fclose(f);

	/*    int f = open(pFilename, O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);

	if (f == -1) {
		OGLDEV_ERROR("Error opening '%s': %s\n", pFilename, strerror(errno));
		exit(0);
	}

	int write_len = write(f, pData, size);
	printf("%d\n", write_len);
	if (write_len != size) {
		OGLDEV_ERROR("Error write file: %s\n", strerror(errno));
		exit(0);
	}

	close(f);*/
}

#endif

void
OgldevError(const char* pFileName, uint line, const char* format, ...)
{
	char msg[1000];
	va_list args;
	va_start(args, format);
	VSNPRINTF(msg, sizeof(msg), format, args);
	va_end(args);

#ifdef _WIN32
	char msg2[1000];
	_snprintf_s(msg2, sizeof(msg2), "%s:%d: %s", pFileName, line, msg);
	MessageBoxA(NULL, msg2, NULL, 0);
#else
	fprintf(stderr, "%s:%d - %s", pFileName, line, msg);
#endif
}

void
OgldevFileError(const char* pFileName, uint line, const char* pFileError)
{
#ifdef _WIN32
	char msg[1000];
	_snprintf_s(msg, sizeof(msg), "%s:%d: unable to open file `%s`", pFileName, line, pFileError);
	MessageBoxA(NULL, msg, NULL, 0);
#else
	fprintf(stderr, "%s:%d: unable to open file `%s`\n", pFileName, line, pFileError);
#endif
}

long long
GetCurrentTimeMillis()
{
#ifdef _WIN32
	return GetTickCount();
#else
	timeval t;
	gettimeofday(&t, NULL);

	long long ret = t.tv_sec * 1000 + t.tv_usec / 1000;
	return ret;
#endif
}

/*
#ifndef VULKAN

#define EXIT_ON_GL_ERROR
void gl_check_error(const char *function, const char *file, int line) {
  GLenum error = 0;
  bool is_error = false;
  while ((error = glGetError()) != GL_NO_ERROR) {
	is_error = true;
	switch (error) {
	case GL_INVALID_ENUM:
	  printf("GL_INVALID_ENUM");
	  break;
	case GL_INVALID_VALUE:
	  printf("GL_INVALID_VALUE");
	  break;
	case GL_INVALID_OPERATION:
	  printf("INVALID_OPERATION");
	  break;
	case GL_STACK_OVERFLOW:
	  printf("STACK_OVERFLOW");
	  break;
	case GL_STACK_UNDERFLOW:
	  printf("STACK_UNDERFLOW");
	  break;
	case GL_OUT_OF_MEMORY:
	  printf("OUT_OF_MEMORY");
	  break;
	case GL_INVALID_FRAMEBUFFER_OPERATION:
	  printf("INVALID_FRAMEBUFFER_OPERATION");
	  break;
	case GL_CONTEXT_LOST:
	  printf("GL_CONTEXT_LOST");
	  break;
	case GL_TABLE_TOO_LARGE:
	  printf("GL_TABLE_TOO_LARGE");
	  break;
	default:
	  printf("Unknown error code %d", error);
	}
	printf(" encountered at function '%s' (%s:%d)\n", function, file, line);
  }
#ifdef EXIT_ON_GL_ERROR
  if (is_error) {
	exit(1);
  }
#endif
}

void glDebugOutput(GLenum source, GLenum type, unsigned int id, GLenum severity,
				   GLsizei length, const char *message, const void *userParam) {
  GLuint ignore_ids[1] = {131185}; // "will use video memory..."

  for (int i = 0; i < ARRAY_SIZE_IN_ELEMENTS(ignore_ids); i++) {
	if (id == ignore_ids[i]) {
	  return;
	}
  }

  printf("!!! Debug callback !!!\n");
  printf("Debug message: id %d, %s\n", id, message);

  printf("Message source: ");
  switch (source) {
  case GL_DEBUG_SOURCE_API:
	printf("API\n");
	break;
  case GL_DEBUG_SOURCE_WINDOW_SYSTEM:
	printf("Window System\n");
	break;
  case GL_DEBUG_SOURCE_SHADER_COMPILER:
	printf("Shader Compiler\n");
	break;
  case GL_DEBUG_SOURCE_THIRD_PARTY:
	printf("Third Party\n");
	break;
  case GL_DEBUG_SOURCE_APPLICATION:
	printf("Application\n");
	break;
  case GL_DEBUG_SOURCE_OTHER:
	printf("Other\n");
	break;
  }

  printf("Error type: ");
  switch (type) {
  case GL_DEBUG_TYPE_ERROR:
	printf("Error\n");
	break;
  case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
	printf("Deprecated Behaviour\n");
	break;
  case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
	printf("Undefined Behaviour\n");
	break;
  case GL_DEBUG_TYPE_PORTABILITY:
	printf("Portability\n");
	break;
  case GL_DEBUG_TYPE_PERFORMANCE:
	printf("Performance\n");
	break;
  case GL_DEBUG_TYPE_MARKER:
	printf("Marker\n");
	break;
  case GL_DEBUG_TYPE_PUSH_GROUP:
	printf("Push Group\n");
	break;
  case GL_DEBUG_TYPE_POP_GROUP:
	printf("Pop Group\n");
	break;
  case GL_DEBUG_TYPE_OTHER:
	printf("Other\n");
	break;
  }

  printf("Severity: ");
  switch (severity) {
  case GL_DEBUG_SEVERITY_HIGH:
	printf("High\n");
	break;
  case GL_DEBUG_SEVERITY_MEDIUM:
	printf("Medium\n");
	break;
  case GL_DEBUG_SEVERITY_LOW:
	printf("Low\n");
	break;
  case GL_DEBUG_SEVERITY_NOTIFICATION:
	printf("Notification\n");
	break;
  }
}
#endif
*/
string
GetDirFromFilename(const string& Filename)
{
	// Extract the directory part from the file name
	string::size_type SlashIndex;

#ifdef _WIN64
	SlashIndex = Filename.find_last_of("\\");

	if (SlashIndex == -1)
	{
		SlashIndex = Filename.find_last_of("/");
	}
#else
	SlashIndex = Filename.find_last_of("/");
#endif

	string Dir;

	if (SlashIndex == string::npos)
	{
		Dir = ".";
	}
	else if (SlashIndex == 0)
	{
		Dir = "/";
	}
	else
	{
		Dir = Filename.substr(0, SlashIndex);
	}

	return Dir;
}

// powf wrapper for integer params to avoid crazy casting
#define powi(base, exp) (int)powf((float)(base), (float)(exp))

#define ToRadian(x) (float)(((x)*M_PI / 180.0f))
#define ToDegree(x) (float)(((x)*180.0f / M_PI))

float
RandomFloat()
{
	float Max = RAND_MAX;
	return ((float)RANDOM() / Max);
}

float
RandomFloatRange(float Start, float End)
{
	if (End == Start)
	{
		printf("Invalid random range: (%f, %f)\n", Start, End);
		exit(0);
	}

	float Delta = End - Start;

	float RandomValue = RandomFloat() * Delta + Start;

	return RandomValue;
}

struct PersProjInfo
{
	float FOV;
	float Width;
	float Height;
	float zNear;
	float zFar;
};

int
CalcNextPowerOfTwo(int x)
{
	int ret = 1;

	if (x == 1)
	{
		return 2;
	}

	while (ret < x)
	{
		ret = ret * 2;
	}

	return ret;
}