#include "picking_texture.h"
#include <cstdint>

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
}