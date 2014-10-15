#include "main.h"
#include "bjImage.h"

extern "C"
{
	#include "glew.h"
}

#define GLM_FORCE_RADIANS

#include "glm.hpp"
#include "ext.hpp"

#include "il.h"
#include "ilu.h"
#include "ilut.h"

#include <string>

const std::string image_vertex_shader_src =
    "#version 330 core\n"
    "uniform mat4 mvp;\n"
    "in vec3 position;\n"
    "in vec2 texcoord;\n"
    "out vec2 frag_texcoord;\n"
    "void main(void)\n"
    "{\n"
    "gl_Position = mvp * vec4(position, 1.0);\n"
    "frag_texcoord = texcoord;\n"
    "}\n";

const std::string image_fragment_shader_src =
    "#version 330 core\n"
    "uniform sampler2D color_texture;\n"
    "in vec2 frag_texcoord;\n"
    "out vec4 color;\n"
    "void main(void)\n"
    "{\n"
    "color = texture(color_texture, frag_texcoord);\n"
    "}\n";

bjImage::bjImage(void) : 
	image_id(0),
	data_size(0),
	image_data(nullptr),
	width(0),
	height(0) { }

bjImage::~bjImage(void) 
{
	if (image_data)
	{
		delete image_data;
		image_data = nullptr;
	}
}

void bjImage::Init(void)
{
    // Initialize DevIL
    ilInit();
    iluInit();
    ilutInit();

    // Initialize the renderer
    //ilutRenderer(ILUT_OPENGL);

    // Generate an image
    ilGenImages(1, &image_id);

    // Select the current image
    ilBindImage(image_id);

    // Enable an origin state
    //ilEnable(IL_ORIGIN_SET);

    // Set default image origin
    ilOriginFunc(IL_ORIGIN_LOWER_LEFT);
}

void bjImage::Load(const char *path)
{
    // Load an image into memory
    ILboolean error = ilLoadImage(path);

    if (!error)
    {
        std::cout << "DevIL image loading: WTF?" << std::endl;
    }

    // Convert image into specific format
    error = ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE);

    if (!error)
    {
        std::cout << "DevIL convert image: WTF?" << std::endl;
    }

    data_size = ilGetInteger(IL_IMAGE_SIZE_OF_DATA);
    image_data = new (std::nothrow) GLubyte[data_size];
    std::memset(image_data, 0, data_size);
    std::memcpy(image_data, ilGetData(), data_size);
	width  = ilGetInteger(IL_IMAGE_WIDTH);
	height = ilGetInteger(IL_IMAGE_HEIGHT);

    // Delete an image
    ilDeleteImages(1, &image_id);
    image_id = 0;

    // Generate an OpenGL texture
    glGenTextures(1, &image_id);

    // Activate texture
    glBindTexture(GL_TEXTURE_2D, image_id);

    // Set texture filtering
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Set texture wraping
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    // Load texture data
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image_data);

    // Texture data is not need
    delete image_data;
    image_data = nullptr;

    // Create shaders
    vertex_shader   = MakeShader(GL_VERTEX_SHADER, image_vertex_shader_src);
    fragment_shader = MakeShader(GL_FRAGMENT_SHADER, image_fragment_shader_src);

    program_id = glCreateProgram();
    glAttachShader(program_id, vertex_shader);
    glAttachShader(program_id, fragment_shader);
    glLinkProgram(program_id);

    GLint gl_error;
    glGetProgramiv(program_id, GL_LINK_STATUS, &gl_error);

    if (!gl_error)
    {
        std::cout << "Image program linking: WTF?" << std::endl;
        glDeleteProgram(program_id);
    }
}

void bjImage::Draw(void)
{
    glUseProgram(program_id);

    // Activate texture unit
    glActiveTexture(GL_TEXTURE0);

    // Set texture on activate texture unit
    glBindTexture(GL_TEXTURE_2D, image_id);

    GLint vertices_count = 4;
    GLfloat s = 10.0;

    /*const GLfloat quad_positions[] = 
    {
        -s * (4.0f / 3.0f), -s, -s * 0.8f,
         s * (4.0f / 3.0f), -s, -s * 0.8f,
         s * (4.0f / 3.0f),  s, -s * 0.8f,
        -s * (4.0f / 3.0f),  s, -s * 0.8f
    };*/

    const GLfloat quad_positions[] = 
    {
        0, 0, 0,
        0, WINDOW_HEIGHT, 0,
        WINDOW_WIDTH,  WINDOW_HEIGHT, 0,
        WINDOW_WIDTH, 0, 0
    };

    /*const GLfloat quad_texcoords[] =
    {
        0.0, 0.0,
        1.0, 0.0,
        1.0, 1.0,
        0.0, 1.0
    };*/

    const GLfloat quad_texcoords[] =
    {
        0.0, 0.0,
        0.0, 1.0,
        1.0, 1.0,
        1.0, 0.0
    };

    const GLuint quad_elements[] =
    {
        0, 1, 2,
        2, 3, 0
    };

    /*const GLuint quad_elements[] =
    {
        0, 3, 2,
        2, 0, 1
    };*/

    GLint texture_location = glGetUniformLocation(program_id, "color_texture");
    glUniform1i(texture_location, 0);

    GLuint vbo[3];
    GLuint vao;

    // Generate VAO
    glGenVertexArrays(1, &vao);

    glBindVertexArray(vao);

    // Generate VBO
    glGenBuffers(3, vbo);

    GLint position_location = glGetAttribLocation(program_id, "position");
    glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
    glBufferData(GL_ARRAY_BUFFER, vertices_count * 3 * sizeof(GLfloat), quad_positions, GL_STATIC_DRAW);
    glVertexAttribPointer(position_location, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);
    glEnableVertexAttribArray(position_location);

    GLint texcoord_location = glGetAttribLocation(program_id, "texcoord");
    glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
    glBufferData(GL_ARRAY_BUFFER, vertices_count * 2 * sizeof(GLfloat), quad_texcoords, GL_STATIC_DRAW);
    glVertexAttribPointer(texcoord_location, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), 0);
    glEnableVertexAttribArray(texcoord_location);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo[2]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(GLuint), quad_elements, GL_STATIC_DRAW);

    // Set matrices
    //glm::mat4 model;
    //glm::mat4 view;
    //glm::mat4 projection;
    glm::mat4 mvp;

    //projection = glm::perspective(45.0, 4.0 / 3.0, 0.1, 100.0);
    //view = glm::translate(glm::mat4(1.0), glm::vec3(0.0, 0.0, -10.0));
    //model = glm::scale(glm::mat4(1.0), glm::vec3(1.0));
    //mvp = projection * view * model;
    mvp = glm::ortho(0.f, static_cast<float>(WINDOW_WIDTH), 0.f, static_cast<float>(WINDOW_HEIGHT), -1.f, 1.f);

    GLint matrix_location = glGetUniformLocation(program_id, "mvp");
    glUniformMatrix4fv(matrix_location, 1, GL_FALSE, glm::value_ptr(mvp));

    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, NULL);
    glUseProgram(0);
}

GLuint bjImage::MakeShader(GLenum type, const std::string &source)
{
    GLuint shader_id = glCreateShader(type);
    std::cout << "Shader ID: " << shader_id << std::endl;

    const char *src = source.c_str();
    const GLint length = source.length();
    glShaderSource(shader_id, 1, &src, &length);

    glCompileShader(shader_id);

    GLint error;

    glGetShaderiv(shader_id, GL_COMPILE_STATUS, &error);

    if (!error)
    {
        std::cout << "Shader compile WTF?" << std::endl;
        glDeleteShader(shader_id);
        return 0;
    }
    
    return shader_id;
}