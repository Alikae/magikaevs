#include "gllib.h"

t_gllib GLLIB;

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void init_camera()
{
    GLLIB.camera.x = 0.5f;
    GLLIB.camera.y = 0.5f;
    GLLIB.camera.zoom = 1.f;
}

void    init_gllib()
{
    printf("\nINIT GLLIB ... - ");
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    GLLIB.window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
    if (GLLIB.window == NULL)
    {
        printf("Failed to create GLFW window");
        glfwTerminate();
        exit(2);
    }
    glfwMakeContextCurrent(GLLIB.window);
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        printf("Failed to initialize GLAD");
        exit(2);
    }
    glViewport(0, 0, 800, 600);
    glfwSetFramebufferSizeCallback(GLLIB.window, framebuffer_size_callback);
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    init_camera();
    printf("DONE\n\n");
}

void    destroy_gllib()
{
    printf("\nDESTROY GLLIB ... - ");
    glfwTerminate();
    printf("DONE\n\n");
}

void        handle_fps()
{
    static double   last_frame = glfwGetTime();
    static double   last_print = last_frame;
    double          this_frame = last_frame;
    static int      nb_frames = 0;

    nb_frames++;
    this_frame = glfwGetTime();
    if (this_frame - last_print > 1.0f)
    {
        printf("--FPS: %4i --SPF: %.4f\n", nb_frames, 1.f / nb_frames);
        nb_frames = 0;
        last_print = this_frame;
    }
    // WHY DOES IT SAY 120 FPS !?!
    //while (glfwGetTime() < last_frame + 1.f / 60.f)
        ;
    last_frame = this_frame;
}

t_texrect*  create_texrect(int xi, int yi, int size_in_pixels, void (*texture_function)(uint32_t*, int))
{
    t_texrect*      texrect;
    float           x = (float)xi;
    float           y = (float)yi;
    unsigned int    EBO;
    float           vertices[] = {
        // positions       // texture coords
        x + 1,  y + 1,     0.0f,   1.0f, 1.0f, // top right
        x + 1,  y,         0.0f,   1.0f, 0.0f, // bottom right
        x,      y,         0.0f,   0.0f, 0.0f, // bottom left
        x,      y + 1,     0.0f,   0.0f, 1.0f  // top left 
    };
    unsigned int    indices[] = {
        0, 1, 3, // first triangle
        1, 2, 3  // second triangle
    };

    if (!(texrect = (t_texrect*)malloc(sizeof(t_texrect))))
        exit(printf("MALLOC ERROR"));
    glGenVertexArrays(1, &texrect->VAO);
    glGenBuffers(1, &texrect->VBO);
    glGenBuffers(1, &EBO);
    glBindVertexArray(texrect->VAO);
    glBindBuffer(GL_ARRAY_BUFFER, texrect->VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_DYNAMIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_DYNAMIC_DRAW);
    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // texture coord attribute
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glGenTextures(1, &texrect->texture);
    glBindTexture(GL_TEXTURE_2D, texrect->texture);
    // set the texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    int nb_pixel = size_in_pixels * size_in_pixels;
    texrect->data = (uint32_t*)malloc(nb_pixel * sizeof(int));
    if (!texrect->data)
        exit(printf("MALLOC ERROR"));
    texture_function(texrect->data, size_in_pixels);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, size_in_pixels, size_in_pixels, 0, GL_RGBA, GL_BYTE, texrect->data);
    //glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 2, 2, GL_RGBA, GL_INT, data);
    texrect->size = size_in_pixels;
    return (texrect);
}

void    draw_texrect(t_texrect* texrect, GLint shader_location)
{
    glBindTexture(GL_TEXTURE_2D, texrect->texture);
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, texrect->size, texrect->size, GL_RGBA, GL_BYTE, texrect->data);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texrect->texture);
    glUniform1i(shader_location, 0);
    glBindVertexArray(texrect->VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}