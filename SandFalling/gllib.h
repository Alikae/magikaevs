#ifndef GLLIB_H
# define GLLIB_H

# include <stdio.h>
# include <stdlib.h>
# include <malloc.h>
# include <string.h>
# include <glad/glad.h>
# include <GLFW\glfw3.h>
# include "utils.h"

typedef struct	s_camera
{
	float	x;
	float	y;
	float	zoom;
}				t_camera;

typedef struct	s_gllib
{
	GLFWwindow* window;
	t_camera	camera;
}				t_gllib;

typedef struct	s_texrect
{
	unsigned int	VAO;
	unsigned int	VBO;
	unsigned int	texture;
	unsigned int	size;
	uint32_t*		data;
}				t_texrect;

extern t_gllib	GLLIB;

void			init_gllib();
int				create_shader(const char* vertex_path, const char* fragment_path);
unsigned int    create_rectangle_example();
unsigned int    create_texture_example();
void			handle_fps();
void			destroy_gllib();
void			draw_texrect(t_texrect* texrect, GLint shader_location);
t_texrect*		create_texrect(int xi, int yi, int size_in_pixels, void (*texture_function)(uint32_t*, int));

#endif