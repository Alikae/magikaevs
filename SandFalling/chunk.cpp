#include "magikae.h"

void	fchunk_center_bordered(uint32_t* data, int size_in_pixels)
{
    int nb_pixels = size_in_pixels * size_in_pixels;
    for (int i = 0; i < nb_pixels; i++)
        // MAX: 797979
        data[i] = 0X101010;
    for (int i = 0; i < size_in_pixels; i++)
    {
        data[i * size_in_pixels] = 0x797979;
        data[i * size_in_pixels + size_in_pixels - 1] = 0x797979;
        data[i] = 0x797979;
        data[nb_pixels - i] = 0x797979;
    }
    data[(size_in_pixels + 1) * size_in_pixels / 2] = 0x797979;
}

inline void set_pixel(t_texrect* chunk, int i, uint32_t color)
{
    chunk->data[i] = color;
}

void    update_pixel(t_texrect* chunk, int i)
{
    if (1)//(float)rand() / RAND_MAX < 0.01)
    {
        if (i)
        {
            uint32_t tmp = chunk->data[i];
            chunk->data[i] = chunk->data[i - 1];
            chunk->data[i - 1] = tmp;
        }
    }
}

void    update_pixel2(t_texrect* chunk, int i)
{
    if (i < CHUNK_SIZE * CHUNK_SIZE - CHUNK_SIZE)
    {
        uint32_t tmp = chunk->data[i];
        chunk->data[i] = chunk->data[i + CHUNK_SIZE];
        chunk->data[i + CHUNK_SIZE] = tmp;
    }
}

void    update_pixel3(t_texrect* chunk, int i)
{
    if (i > CHUNK_SIZE)
    {
        uint32_t tmp = chunk->data[i];
        chunk->data[i] = chunk->data[i - CHUNK_SIZE];
        chunk->data[i - CHUNK_SIZE] = tmp;
    }
}

uint32_t    crand()
{
    uint32_t r = (uint32_t)((float)rand() / RAND_MAX * 0x79);
    uint32_t g = (uint32_t)((float)rand() / RAND_MAX * 0x79);
    uint32_t b = (uint32_t)((float)rand() / RAND_MAX * 0x79);
    r = (b << 16) | (g << 8) | (r);
    return (r);
}

void    update_chunk(t_texrect* chunk)
{
    int val = (float)rand() * CHUNK_SIZE * CHUNK_SIZE / RAND_MAX;
    for (int i = 0; i < val; i++)
    {
        update_pixel(chunk, i);
    }
    for (int i = 0; i < val; i++)
    {
        update_pixel2(chunk, i % CHUNK_SIZE * CHUNK_SIZE + i / CHUNK_SIZE);
    }
    for (int i = 0; i < val / 2; i++)
    {
        update_pixel3(chunk, CHUNK_SIZE * CHUNK_SIZE - (i % CHUNK_SIZE * CHUNK_SIZE + i / CHUNK_SIZE));
    }
 //   chunk->data[CHUNK_SIZE * CHUNK_SIZE / 2] = crand();
 //   chunk->data[CHUNK_SIZE * CHUNK_SIZE / 2 - 3600] = crand();
 //   chunk->data[CHUNK_SIZE * CHUNK_SIZE / 2 - 7200] = crand();
 //   chunk->data[CHUNK_SIZE * CHUNK_SIZE / 2 + 3600] = crand();
    uint32_t    color = crand();
//    for (int i = 0; i < CHUNK_SIZE * 3; i++)
//        chunk->data[(int)((float)rand() / RAND_MAX * CHUNK_SIZE)] = color;
}

void    psychedelic_update_chunk(t_texrect* chunk)
{
    int val = (float)rand() * CHUNK_SIZE * CHUNK_SIZE / RAND_MAX;
    for (int i = 0; i < val; i++)
    {
        update_pixel(chunk, i);
    }
    for (int i = 0; i < val; i++)
    {
        update_pixel2(chunk, i % CHUNK_SIZE * CHUNK_SIZE + i / CHUNK_SIZE);
    }
    for (int i = 0; i < val / 2; i++)
    {
        update_pixel3(chunk, CHUNK_SIZE * CHUNK_SIZE - (i % CHUNK_SIZE * CHUNK_SIZE + i / CHUNK_SIZE));
    }
    chunk->data[CHUNK_SIZE * CHUNK_SIZE / 2] = crand();
    chunk->data[CHUNK_SIZE * CHUNK_SIZE / 2 - 3600] = crand();
    chunk->data[CHUNK_SIZE * CHUNK_SIZE / 2 - 7200] = crand();
    chunk->data[CHUNK_SIZE * CHUNK_SIZE / 2 + 3600] = crand();
    uint32_t    color = crand();
    for (int i = 0; i < CHUNK_SIZE * 3; i++)
        chunk->data[(int)((float)rand() / RAND_MAX * CHUNK_SIZE)] = color;
}