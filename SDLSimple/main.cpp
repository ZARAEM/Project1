/**
* Author: Mearaj Ahmed
* Assignment: Simple 2D Scene
* Date due: 2024-06-15, 11:59pm
* I pledge that I have completed this assignment without
* collaborating with anyone else, in conformance with the
* NYU School of Engineering Policies and Procedures on
* Academic Misconduct.
**/

/*
#### _Tips_

1. [**Common Issues**](#common-issues)
2. [**Extra Credit**](#extra-credit)

### Requirement 1: _At Least Two Objects (50%)_

- There needs to be at least 2 different "objects" in the scene.
- Each object must use a different texture. These textures must _not_ be solid colours, but images. You may use any images that you like.

### Requirement 2: _Movement (25%)_

- At least one of your objects must be **moving in relation to other objects** (i.e. translation).

### Requirement 3: _Rotation (25%)_

- At least one of your objects must be **rotating**.

### Common Issues

If your screen is flickering, remember that you have to, _in order_:

1. Call `glClear`.
2. Draw every single one of your objects (i.e. everything you want to be in your scene)
3. Call `SDL_GL_SwapWindow`.

### Extra Credit

- Have at least one of your objects change scale.
- Have _both_ of your objects translate in a pattern _other than_ vertically and horizontally.

*/

#define GL_SILENCE_DEPRECATION
#define STB_IMAGE_IMPLEMENTATION
#define LOG(argument) std::cout << argument << '\n'
#define GL_GLEXT_PROTOTYPES 1

#ifdef _WINDOWS
#include <GL/glew.h>
#endif

#include <SDL.h>
#include <SDL_opengl.h>
#include "glm/mat4x4.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "ShaderProgram.h"
#include "stb_image.h"

enum AppStatus { RUNNING, TERMINATED };

// Game Window Size
constexpr int WINDOW_WIDTH = 640 * 2,
WINDOW_HEIGHT = 480 * 2;

// Background Color from 0-1
constexpr float BG_RED = (float) 255/255,
BG_GREEN = (float) 255/255,
BG_BLUE = (float) 255/255,
BG_OPACITY = (float) 1;

// Viewport (Basically the Camera)
constexpr int VIEWPORT_X = 0,
VIEWPORT_Y = 0,

VIEWPORT_WIDTH = WINDOW_WIDTH,
VIEWPORT_HEIGHT = WINDOW_HEIGHT;

// Shader Path
constexpr char V_SHADER_PATH[] = "shaders/vertex_textured.glsl",
F_SHADER_PATH[] = "shaders/fragment_textured.glsl";

// Milliseconds in 1 Second
constexpr float MILLISECONDS_IN_SECOND = 1000.0;

// Number of Textures Generated
constexpr GLint NUMBER_OF_TEXTURES = 2,

/* Don't Pay Attention to these Two */
LEVEL_OF_DETAIL = 0,
TEXTURE_BORDER = 0;

// Path to Sprites
constexpr char PENGUIN_BLUE_FILEPATH[] = "assets/penguin_blue",
PUFFLE_ORANGE_FILEPATH[] = "assets/puffle_orange";

// Initial Scale of Sprites
constexpr glm::vec3 INIT_SCALE = glm::vec3(2.5f, 5.263f, 0.0f),

// Initial Position of Sprites
INIT_POS_PENGUIN_BLUE = glm::vec3(0.0f, 0.0f, 0.0f),
INIT_POS_PUFFLE_ORANGE = glm::vec3(0.0f, 0.0f, 0.0f);

// Rotational Increment
constexpr float ROT_INCREMENT = 1.0f;

SDL_Window* g_display_window;
AppStatus g_app_status = RUNNING;
ShaderProgram g_shader_program = ShaderProgram();

glm::mat4 g_view_matrix,
g_penguin_matrix,
g_puffle_matrix,
g_projection_matrix;

float g_previous_ticks = 0.0f;

glm::vec3 g_rotation_penguin = glm::vec3(0.0f, 0.0f, 0.0f),
g_rotation_puffle = glm::vec3(0.0f, 0.0f, 0.0f);

GLuint g_penguin_texture_id,
g_puffle_texture_id;


gluint load_texture(const char* filepath)
{
    // step 1: loading the image file
    int width, height, number_of_components;
    unsigned char* image = stbi_load(filepath, &width, &height, &number_of_components, stbi_rgb_alpha);

    //if (image == null)
    //{
    //    log("unable to load image. make sure the path is correct.");
    //    assert(false);
    //}

    //// step 2: generating and binding a texture id to our image
    //gluint textureid;
    //glgentextures(number_of_textures, &textureid);
    //glbindtexture(gl_texture_2d, textureid);
    //glteximage2d(gl_texture_2d, level_of_detail, gl_rgba, width, height, texture_border, gl_rgba, gl_unsigned_byte, image);

    //// step 3: setting our texture filter parameters
    //gltexparameteri(gl_texture_2d, gl_texture_min_filter, gl_nearest);
    //gltexparameteri(gl_texture_2d, gl_texture_mag_filter, gl_nearest);

    //// step 4: releasing our file from memory and returning our texture id
    //stbi_image_free(image);

    //return textureid;
}


void initialise()
{
    // Initialise video and joystick subsystems
    SDL_Init(SDL_INIT_VIDEO);

    g_display_window = SDL_CreateWindow("Hello, Textures!",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        WINDOW_WIDTH, WINDOW_HEIGHT,
        SDL_WINDOW_OPENGL);

    SDL_GLContext context = SDL_GL_CreateContext(g_display_window);
    SDL_GL_MakeCurrent(g_display_window, context);

    if (g_display_window == nullptr)
    {
        std::cerr << "Error: SDL window could not be created.\n";
        SDL_Quit();
        exit(1);
    }

#ifdef _WINDOWS
    glewInit();
#endif

    glViewport(VIEWPORT_X, VIEWPORT_Y, VIEWPORT_WIDTH, VIEWPORT_HEIGHT);

    g_shader_program.load(V_SHADER_PATH, F_SHADER_PATH);

    //g_kano_matrix = glm::mat4(1.0f);
    //g_mahiru_matrix = glm::mat4(1.0f);
    //g_view_matrix = glm::mat4(1.0f);
    //g_projection_matrix = glm::ortho(-5.0f, 5.0f, -3.75f, 3.75f, -1.0f, 1.0f);

    //g_shader_program.set_projection_matrix(g_projection_matrix);
    //g_shader_program.set_view_matrix(g_view_matrix);

    //glUseProgram(g_shader_program.get_program_id());

    glClearColor(BG_RED, BG_GREEN, BG_BLUE, BG_OPACITY);

    //g_kano_texture_id = load_texture(KANO_SPRITE_FILEPATH);
    //g_mahiru_texture_id = load_texture(MAHIRU_SPRITE_FILEPATH);

    //glEnable(GL_BLEND);
    //glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}


void process_input()
{
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE)
        {
            g_app_status = TERMINATED;
        }
    }
}


void update()
{
    /* Delta time calculations */
    float ticks = (float)SDL_GetTicks() / MILLISECONDS_IN_SECOND;
    float delta_time = ticks - g_previous_ticks;
    g_previous_ticks = ticks;

    ///* Game logic */
    //g_rotation_kano.y += ROT_INCREMENT * delta_time;
    //g_rotation_mahiru.y += -1 * ROT_INCREMENT * delta_time;

    ///* Model matrix reset */
    //g_kano_matrix = glm::mat4(1.0f);
    //g_mahiru_matrix = glm::mat4(1.0f);

    ///* Transformations */
    //g_kano_matrix = glm::translate(g_kano_matrix, INIT_POS_KANO);
    //g_kano_matrix = glm::rotate(g_kano_matrix,
    //    g_rotation_kano.y,
    //    glm::vec3(0.0f, 1.0f, 0.0f));
    //g_kano_matrix = glm::scale(g_kano_matrix, INIT_SCALE);

    //g_mahiru_matrix = glm::translate(g_mahiru_matrix, INIT_POS_MAHIRU);
    //g_mahiru_matrix = glm::rotate(g_mahiru_matrix,
    //    g_rotation_mahiru.y,
    //    glm::vec3(0.0f, 1.0f, 0.0f));
    //g_mahiru_matrix = glm::scale(g_mahiru_matrix, INIT_SCALE);
}


void draw_object(glm::mat4& object_g_model_matrix, GLuint& object_texture_id)
{
    //g_shader_program.set_model_matrix(object_g_model_matrix);
    //glBindTexture(GL_TEXTURE_2D, object_texture_id);
    //glDrawArrays(GL_TRIANGLES, 0, 6); // we are now drawing 2 triangles, so use 6, not 3
}


void render()
{
    glClear(GL_COLOR_BUFFER_BIT);

    //// Vertices
    //float vertices[] =
    //{
    //    -0.5f, -0.5f, 0.5f, -0.5f, 0.5f, 0.5f,  // triangle 1
    //    -0.5f, -0.5f, 0.5f, 0.5f, -0.5f, 0.5f   // triangle 2
    //};

    //// Textures
    //float texture_coordinates[] =
    //{
    //    0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,     // triangle 1
    //    0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f,     // triangle 2
    //};

    //glVertexAttribPointer(g_shader_program.get_position_attribute(), 2, GL_FLOAT, false,
    //    0, vertices);
    //glEnableVertexAttribArray(g_shader_program.get_position_attribute());

    //glVertexAttribPointer(g_shader_program.get_tex_coordinate_attribute(), 2, GL_FLOAT,
    //    false, 0, texture_coordinates);
    //glEnableVertexAttribArray(g_shader_program.get_tex_coordinate_attribute());

    //// Bind texture
    //draw_object(g_kano_matrix, g_kano_texture_id);
    //draw_object(g_mahiru_matrix, g_mahiru_texture_id);

    //// We disable two attribute arrays now
    //glDisableVertexAttribArray(g_shader_program.get_position_attribute());
    //glDisableVertexAttribArray(g_shader_program.get_tex_coordinate_attribute());

    SDL_GL_SwapWindow(g_display_window);
}


void shutdown() { SDL_Quit(); }


int main(int argc, char* argv[])
{
    initialise();

    while (g_app_status == RUNNING)
    {
        process_input();
        update();
        render();
    }

    shutdown();
    return 0;
}
