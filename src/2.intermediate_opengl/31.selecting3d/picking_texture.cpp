#include "picking_texture.h"
#include <stdlib.h>

Picking_Texture::Picking_Texture(/* args */)
{
}

Picking_Texture::~Picking_Texture()
{
}

void Picking_Texture::init(unsigned int width, unsigned int height)
{
	// create FBO;
	glGenFramebuffers(1, &m_fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);

	// create the texture object for the primitive information buffer
	glGenTextures(1, &m_picking_texture);
	glBindTexture(GL_TEXTURE_2D, m_picking_texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32UI, width, height, 0, GL_RGB_INTEGER, GL_UNSIGNED_INT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_picking_texture, 0);

	// create the texture object for the depth buffer
	glGenTextures(1, &m_depth_texture);
	glBindBuffer(GL_TEXTURE_2D, m_depth_texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_depth_texture, 0);

	// verify that fbo is correct
	GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);

	if (status != GL_FRAMEBUFFER_COMPLETE)
	{
		printf("FB error, status: 0x%x\n", status);
		exit(1);
	}
	// restore the default frame buffer
	glBindTexture(GL_TEXTURE_2D, 0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Picking_Texture::enable_writing()
{
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_fbo);
}
void Picking_Texture::disable_writing()
{
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
}

Pixel_Info Picking_Texture::read_pixel(unsigned int x, unsigned int y)
{
	glBindFramebuffer(GL_READ_FRAMEBUFFER, m_fbo);
	glReadBuffer(GL_COLOR_ATTACHMENT0);

	Pixel_Info pixel;
	glReadPixels(x, y, 1, 1, GL_RGB_INTEGER, GL_UNSIGNED_INT, &pixel);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);
	return pixel;
}