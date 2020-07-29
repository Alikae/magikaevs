#ifndef MAGIKAE
# define MAGIKAE

# include "gllib.h"
# include <math.h>

# define CHUNK_SIZE	512

typedef enum	e_matter
{
	M_AIR,
	M_SAND,
	M_WATER
}				t_matter;

void	fchunk_center_bordered(uint32_t* data, int size_in_pixels);
void    update_chunk(t_texrect* chunk);
void    psychedelic_update_chunk(t_texrect* chunk);
inline void set_pixel(t_texrect* chunk, int i, uint32_t color);

uint32_t	crand();

#endif