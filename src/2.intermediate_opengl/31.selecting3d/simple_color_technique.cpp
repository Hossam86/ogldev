#include "simple_color_technique.h"
#include <ogldev/utility.h>

SimpleColorTechnique::SimpleColorTechnique() {}

bool
SimpleColorTechnique::Init()
{
	if (!Technique::Init())
	{
		return false;
	}

	if (!AddShader(GL_VERTEX_SHADER, "simple_color.vs"))
	{
		return false;
	}

	if (!AddShader(GL_FRAGMENT_SHADER, "simple_color.fs"))
	{
		return false;
	}

	if (!Finalize())
	{
		return false;
	}

	m_WVPLocation = GetUniformLocation("gWVP");

	if (m_WVPLocation == INVALID_UNIFORM_LOCATION)
	{
		return false;
	}

	return true;
}

void
SimpleColorTechnique::SetWVP(const Matrix4f& WVP)
{
	glUniformMatrix4fv(m_WVPLocation, 1, GL_TRUE, (const GLfloat*)WVP.m);
}