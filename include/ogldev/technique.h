

#ifndef TECHNIQUE_H
#define TECHNIQUE_H

#include <glad/glad.h>
#include <list>

namespace ogl
{

	class Technique
	{
	public:
		Technique();

		virtual ~Technique();

		virtual bool
		Init();

		void
		Enable();

		GLuint
		GetProgram() const
		{
			return m_shaderProg;
		}

	protected:
		bool
		AddShader(GLenum ShaderType, const char* pFilename);

		bool
		Finalize();

		GLint
		GetUniformLocation(const char* pUniformName);

		GLuint m_shaderProg = 0;

	private:
		typedef std::list<GLuint> ShaderObjList;
		ShaderObjList m_shaderObjList;
	};
}
#endif /* TECHNIQUE_H */
