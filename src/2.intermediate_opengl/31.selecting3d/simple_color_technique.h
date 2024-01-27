#ifndef SIMPLE_COLOR_TECHNIQUE_H
#define SIMPLE_COLOR_TECHNIQUE_H

#include "ogldev/technique.h"
#include <ogldev/math3d.h>

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

#endif /* SIMPLE_COLOR_TECHNIQUE_H */