#include <glad/glad.h>

class Picking_Texture
{
private:
	GLuint m_fbo;			  // handle to frame buffer
	GLuint m_picking_texture; // handle to texture
	GLuint m_depth_texture;	  // handle to the depth buffer

public:
	Picking_Texture(/* args */);
	~Picking_Texture();

	void init(unsigned int window_width, unsigned int window_height);
	void enable_writing();
	void disaple_writing();
	struct Pixel_Info
	{
		uint object_id = 0;
		uint draw_id = 0;
		uint prim_id = 0;
		print()
		{
			printf(" object %d draw %d prim %d\n", object_id, draw_id, prim_id);
		}
	};
};

Picking_Texture::Picking_Texture(/* args */)
{
}

Picking_Texture::~Picking_Texture()
{
}
