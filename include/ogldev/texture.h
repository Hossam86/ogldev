#pragma once

#include <math.h>
#include <string>


#include <glad/glad.h>

#include <ogldev/texture.h>
#include <ogldev/utility.h>

#include <stb_image/stb_image.h>
#include <stb_image/stb_image_write.h>

class Texture
{
public:
	Texture(GLenum TextureTarget, const std::string& FileName)
	{
		m_textureTarget = TextureTarget;
		m_fileName = FileName;
	}

	Texture(GLenum TextureTarget) { m_textureTarget = TextureTarget; }

	// Should be called once to load the texture
	bool
	Load()
	{
		stbi_set_flip_vertically_on_load(1);

		unsigned char* pImageData = stbi_load(m_fileName.c_str(), &m_imageWidth, &m_imageHeight, &m_imageBPP, 0);

		if (!pImageData)
		{
			printf("Can't load texture from '%s' - %s\n", m_fileName.c_str(), stbi_failure_reason());
			exit(0);
		}

		printf("Width %d, height %d, bpp %d\n", m_imageWidth, m_imageHeight, m_imageBPP);

		LoadInternal(pImageData);

		return true;
	}

	void
	Load(unsigned int BufferSize, void* pData)
	{
		void* pImageData =
			stbi_load_from_memory((const stbi_uc*)pData, BufferSize, &m_imageWidth, &m_imageHeight, &m_imageBPP, 0);

		LoadInternal(pImageData);

		stbi_image_free(pImageData);
	}

	void
	Load(const std::string& Filename)
	{
		m_fileName = Filename;

		if (!Load())
		{
			exit(0);
		}
	}

	void
	LoadRaw(int Width, int Height, int BPP, unsigned char* pImageData)
	{
		m_imageWidth = Width;
		m_imageHeight = Height;
		m_imageBPP = BPP;

		LoadInternal(pImageData);
	}

	// Must be called at least once for the specific texture unit
	void
	Bind(GLenum TextureUnit)
	{
		// if (IsGLVersionHigher(4, 5)) {
		//   BindInternalDSA(TextureUnit);
		// } else {
		//   BindInternalNonDSA(TextureUnit);
		// }

		// TODO: fix IsGLVersionHigher
		BindInternalNonDSA(TextureUnit);
	}

	void
	GetImageSize(int& ImageWidth, int& ImageHeight)
	{
		ImageWidth = m_imageWidth;
		ImageHeight = m_imageHeight;
	}

	GLuint
	GetTexture() const
	{
		return m_textureObj;
	}

private:
	void
	LoadInternal(void* pImageData)
	{
		// if (IsGLVersionHigher(4, 5)) {
		//   LoadInternalDSA(pImageData);
		// } else {
		//   LoadInternalNonDSA(pImageData);
		// }

		// TODO: fix IsGLVersionHigher
		LoadInternalNonDSA(pImageData);
	}

	void
	LoadInternalNonDSA(void* pImageData)
	{
		glGenTextures(1, &m_textureObj);
		glBindTexture(m_textureTarget, m_textureObj);

		if (m_textureTarget == GL_TEXTURE_2D)
		{
			switch (m_imageBPP)
			{
			case 1:
				glTexImage2D(
					m_textureTarget,
					0,
					GL_RED,
					m_imageWidth,
					m_imageHeight,
					0,
					GL_RED,
					GL_UNSIGNED_BYTE,
					pImageData);
				break;

			case 2:
				glTexImage2D(
					m_textureTarget,
					0,
					GL_RG,
					m_imageWidth,
					m_imageHeight,
					0,
					GL_RG,
					GL_UNSIGNED_BYTE,
					pImageData);
				break;

			case 3:
				glTexImage2D(
					m_textureTarget,
					0,
					GL_RGB,
					m_imageWidth,
					m_imageHeight,
					0,
					GL_RGB,
					GL_UNSIGNED_BYTE,
					pImageData);
				break;

			case 4:
				glTexImage2D(
					m_textureTarget,
					0,
					GL_RGBA,
					m_imageWidth,
					m_imageHeight,
					0,
					GL_RGBA,
					GL_UNSIGNED_BYTE,
					pImageData);
				break;

			default:
				NOT_IMPLEMENTED;
			}
		}
		else
		{
			printf("Support for texture target %x is not implemented\n", m_textureTarget);
			exit(1);
		}

		glTexParameteri(m_textureTarget, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(m_textureTarget, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(m_textureTarget, GL_TEXTURE_BASE_LEVEL, 0);
		glTexParameteri(m_textureTarget, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(m_textureTarget, GL_TEXTURE_WRAP_T, GL_REPEAT);

		glGenerateMipmap(m_textureTarget);

		glBindTexture(m_textureTarget, 0);
	}

	void
	LoadInternalDSA(void* pImageData)
	{
		glCreateTextures(m_textureTarget, 1, &m_textureObj);

		int Levels = std::min(5, (int)log2f((float)std::max(m_imageWidth, m_imageHeight)));

		if (m_textureTarget == GL_TEXTURE_2D)
		{
			switch (m_imageBPP)
			{
			case 1:
				glTextureStorage2D(m_textureObj, Levels, GL_R8, m_imageWidth, m_imageHeight);
				glTextureSubImage2D(
					m_textureObj,
					0,
					0,
					0,
					m_imageWidth,
					m_imageHeight,
					GL_RED,
					GL_UNSIGNED_BYTE,
					pImageData);
				break;

			case 2:
				glTextureStorage2D(m_textureObj, Levels, GL_RG8, m_imageWidth, m_imageHeight);
				glTextureSubImage2D(
					m_textureObj,
					0,
					0,
					0,
					m_imageWidth,
					m_imageHeight,
					GL_RG,
					GL_UNSIGNED_BYTE,
					pImageData);
				break;

			case 3:
				glTextureStorage2D(m_textureObj, Levels, GL_RGB8, m_imageWidth, m_imageHeight);
				glTextureSubImage2D(
					m_textureObj,
					0,
					0,
					0,
					m_imageWidth,
					m_imageHeight,
					GL_RGB,
					GL_UNSIGNED_BYTE,
					pImageData);
				break;

			case 4:
				glTextureStorage2D(m_textureObj, Levels, GL_RGBA8, m_imageWidth, m_imageHeight);
				glTextureSubImage2D(
					m_textureObj,
					0,
					0,
					0,
					m_imageWidth,
					m_imageHeight,
					GL_RGBA,
					GL_UNSIGNED_BYTE,
					pImageData);
				break;

			default:
				NOT_IMPLEMENTED;
			}
		}
		else
		{
			printf("Support for texture target %x is not implemented\n", m_textureTarget);
			exit(1);
		}

		glTextureParameteri(m_textureObj, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTextureParameteri(m_textureObj, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTextureParameterf(m_textureObj, GL_TEXTURE_BASE_LEVEL, 0);
		glTextureParameteri(m_textureObj, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTextureParameteri(m_textureObj, GL_TEXTURE_WRAP_T, GL_REPEAT);

		glGenerateTextureMipmap(m_textureObj);
	}

	void
	BindInternalNonDSA(GLenum TextureUnit)
	{
		glActiveTexture(TextureUnit);
		glBindTexture(m_textureTarget, m_textureObj);
	}

	void
	BindInternalDSA(GLenum TextureUnit)
	{
		glBindTextureUnit(TextureUnit - GL_TEXTURE0, m_textureObj);
	}

	std::string m_fileName;
	GLenum m_textureTarget;
	GLuint m_textureObj;
	int m_imageWidth = 0;
	int m_imageHeight = 0;
	int m_imageBPP = 0;
};