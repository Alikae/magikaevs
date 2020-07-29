#include "magikae.h"

void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        GLLIB.camera.x += 0.1f;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        GLLIB.camera.x -= 0.1f;
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        GLLIB.camera.y += 0.1f;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        GLLIB.camera.y -= 0.1f;
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
        GLLIB.camera.zoom -= 0.01f;
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
        GLLIB.camera.zoom += 0.01f;
}

void    render(unsigned int shader_program, t_texrect* texrect)
{
    glClear(GL_COLOR_BUFFER_BIT);
    glUseProgram(shader_program);
    glUniform3f(glGetUniformLocation(shader_program, "camPos"), GLLIB.camera.x, GLLIB.camera.y, GLLIB.camera.zoom);
    draw_texrect(texrect, glGetUniformLocation(shader_program, "ourTexture"));
    glfwSwapBuffers(GLLIB.window);
}

void    game_logic(t_texrect* chunk)
{
    update_chunk(chunk);
}

void    blush_matter(t_texrect* chunk, int xp, int yp, int radius, uint32_t color)
{
    for (int x = -radius; x < radius; x++)
        for (int y = -radius; y < radius; y++)
            if (x * x + y * y <= radius * radius)
            {
                int p = (yp + y) * CHUNK_SIZE + xp + x;
                if (p > 0 && p < CHUNK_SIZE * CHUNK_SIZE)
                {
                    if (p % 2 == (p / CHUNK_SIZE) % 2)
                        chunk->data[p] = color;
                    else
                        chunk->data[p] = 0x000000;
                }
            }
}

void    draw_vertical_line(t_texrect* chunk, int ax, int ay, int bx, int by, float delta, uint32_t color)
{
    if (delta)
        delta = 1.f / delta;
    float ix = (float)ax;
    for (int iy = ay; iy <= by; iy++)
    {
        chunk->data[(int)ix + iy * CHUNK_SIZE] = color;
        ix += delta;
    }
}

void    draw_horizontal_line(t_texrect* chunk, int ax, int ay, int bx, int by, float delta, uint32_t color)
{
    float iy = (float)ay;
    for (int ix = ax; ix <= bx; ix++)
    {
        chunk->data[ix + (int)iy * CHUNK_SIZE] = color;
        iy += delta;
    }
}

void    draw_line(t_texrect* chunk, int ax, int ay, int bx, int by, uint32_t color)
{
    if (ax > bx)
    {
        int tmp = ax;
        ax = bx;
        bx = tmp;
        tmp = ay;
        ay = by;
        by = tmp;
    }
    if (bx == ax)
    {
        draw_vertical_line(chunk, ax, ay, bx, by, 0, color);
        return;
    }
    float delta = (float)(by - ay) / (bx - ax);
    if (delta < 1 || delta > -1)
        draw_horizontal_line(chunk, ax, ay, bx, by, delta, color);
    else
        draw_vertical_line(chunk, ax, ay, bx, by, delta, color);
}

void    draw_random_line(t_texrect* chunk)
{
    int ax = (int)((float)rand() / RAND_MAX * CHUNK_SIZE);
    int ay = (int)((float)rand() / RAND_MAX * CHUNK_SIZE);
    int bx = (int)((float)rand() / RAND_MAX * CHUNK_SIZE);
    int by = (int)((float)rand() / RAND_MAX * CHUNK_SIZE);
    if (ax % 2)
    {
        ax *= -1;
        ay *= -1;
        bx *= -1;
        by *= -1;
    }
    ax = CHUNK_SIZE / 4 + ax % 50;
    bx = 3 * CHUNK_SIZE / 4 + bx % 50;
    ay = CHUNK_SIZE / 2 + ay % 50;
    by = CHUNK_SIZE / 2 + by % 50;
    draw_line(chunk, ax, ay, bx, by, crand());
}

void    psychedelic_program(uint32_t shaderProgram, t_texrect* chunk)
{
    while (!glfwWindowShouldClose(GLLIB.window))
    {
        //blush_matter(chunk, CHUNK_SIZE / 2, CHUNK_SIZE / 2, 40, crand());
        //blush_matter(chunk, CHUNK_SIZE / 2, CHUNK_SIZE / 2, 38, 0x55);
        //blush_matter(chunk, 3 * CHUNK_SIZE / 4, CHUNK_SIZE / 2, 20, 0x5555);
        //blush_matter(chunk, CHUNK_SIZE / 4, 0, 40, 0x555500);
        //blush_matter(chunk, CHUNK_SIZE / 2, 0, 40, 0x55);
        //blush_matter(chunk, 0, CHUNK_SIZE / 2, 40, 0x5500);
        //draw_random_line(chunk);
        //draw_line(chunk, CHUNK_SIZE / 4, CHUNK_SIZE / 2, 3 * CHUNK_SIZE / 4, CHUNK_SIZE / 2, crand());
        //draw_line(chunk, CHUNK_SIZE / 2, CHUNK_SIZE / 4, CHUNK_SIZE / 2, 3 * CHUNK_SIZE / 4, crand());
        render(shaderProgram, chunk);
        glfwPollEvents();    processInput(GLLIB.window);
        psychedelic_update_chunk(chunk);
        if (glfwGetMouseButton(GLLIB.window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
        {
            draw_line(chunk, CHUNK_SIZE / 2 + cosf(glfwGetTime()) * CHUNK_SIZE / 2, CHUNK_SIZE / 2 + sinf(glfwGetTime()) * CHUNK_SIZE / 2, CHUNK_SIZE / 2 - cosf(glfwGetTime()) * CHUNK_SIZE / 2, CHUNK_SIZE / 2 - sinf(glfwGetTime()) * CHUNK_SIZE / 2, crand());
        }
    }
}

int main()
{
	printf("Launch Program\n");

    init_gllib();
    
    // Create Better Shader that render with Camera
    unsigned int shaderProgram = create_shader("shader.vert",
        "shader.frag");
    //unsigned int VAO = create_rectangle_example();
    //unsigned int VAO = create_texture_example();

    t_texrect* chunk = create_texrect(0, 0, CHUNK_SIZE, fchunk_center_bordered);
    for (int i = 0; i < CHUNK_SIZE * CHUNK_SIZE / 3; i++)
        chunk->data[3 * i] = 0x550000;
    //psychedelic_program(shaderProgram, chunk);
    while (!glfwWindowShouldClose(GLLIB.window))
    {
        handle_fps();
        processInput(GLLIB.window);
//        if (glfwGetMouseButton(GLLIB.window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
//        {
//            draw_line(chunk, CHUNK_SIZE / 2 + cosf(glfwGetTime()) * CHUNK_SIZE / 2, CHUNK_SIZE / 2 + sinf(glfwGetTime()) * CHUNK_SIZE / 2, CHUNK_SIZE / 2 - cosf(glfwGetTime()) * CHUNK_SIZE / 2, CHUNK_SIZE / 2 - sinf(glfwGetTime()) * CHUNK_SIZE / 2, crand());
//            double x, y;
//            glfwGetCursorPos(GLLIB.window, &x, &y);
//            x /= GLLIB.camera.zoom;
//            y /= GLLIB.camera.zoom;
//            x += GLLIB.camera.x;
//            y += GLLIB.camera.y;
//            blush_matter(chunk, (int)x, (int)y, 100, 0x550000);
//        }
        game_logic(chunk);
        render(shaderProgram, chunk);
        glfwPollEvents();
    }

    destroy_gllib();

    return 0;
}

// TODO: