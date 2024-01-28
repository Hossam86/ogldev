#pragma once

#include <ogldev/math3d.h>
#include <ogldev/technique.h>

class SimpleColorTechnique : public Technique
{
public:
	SimpleColorTechnique();

	virtual bool
	Init();

	void
	SetWVP(const Matrix4f& WVP);

private:
	GLuint m_WVPLocation;
};