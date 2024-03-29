#pragma once

#include <cstdio>
#include <glad/glad.h>

struct Pixel_Info
{
	unsigned int object_id = 0;
	unsigned int draw_id = 0;
	unsigned int prim_id = 0;
	void
	print()
	{
		printf(" object %d draw %d prim %d\n", object_id, draw_id, prim_id);
	}
};

class Picking_Texture
{
public:
	Picking_Texture(/* args */);
	~Picking_Texture();

	void
	init(unsigned int window_width, unsigned int window_height);

	void
	enable_writing();

	void
	disable_writing();

	Pixel_Info
	read_pixel(unsigned int x, unsigned int y);

private:
	GLuint m_fbo;			  // handle to frame buffer
	GLuint m_picking_texture; // handle to texture
	GLuint m_depth_texture;	  // handle to the depth buffer
};
