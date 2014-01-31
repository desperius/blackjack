#include "main.h"
#include "font.h"
#include "ft2.h"
#include FT_FREETYPE_H
#include <string>

extern "C"
{
	#include "glew.h"
}

struct point {
	GLfloat x;
	GLfloat y;
	GLfloat s;
	GLfloat t;
};

const std::string font_vertex_shader_src =
    "#version 120\n"
    "attribute vec4 coord;\n"
    "varying   vec2 texcoord;\n"
    "void main(void)\n"
    "{\n"
    "gl_Position = vec4(coord.xy, 0, 1);\n"
    "texcoord = coord.zw;\n"
    "}\n";

const std::string font_fragment_shader_src =
    "#version 120\n"
    "varying vec2 texcoord;\n"
    "uniform sampler2D tex;\n"
    "uniform vec4 color;\n"
    "void main(void)\n"
    "{\n"
    "gl_FragColor = vec4(color.rgb, texture2D(tex, texcoord).a) * color;\n"
    "}\n";

#define FONT_DIR "../res/fonts/"

Font::Font(void) : library(nullptr), face(nullptr) { }

Font::~Font(void)
{
    FT_Done_FreeType(library);
    library = nullptr;

    FT_Done_Face(face);
    face = nullptr;
}

void Font::Init(void)
{
    FT_Error error = FT_Init_FreeType(&library);

    if (error)
    {
        std::cout << "FT libarary initialize WTF?" << std::endl;
    }

    error = FT_New_Face(library, FONT_DIR "arial.ttf", 0, &face);

    if (error)
    {
        std::cout << "FT face initialize WTF?" << std::endl;
    }
}

void Font::Load(void)
{
    vertex_shader   = MakeShader(GL_VERTEX_SHADER, font_vertex_shader_src);
    fragment_shader = MakeShader(GL_FRAGMENT_SHADER, font_fragment_shader_src);

    program_id = glCreateProgram();
    std::cout << "Program ID: " << program_id << std::endl;
    glAttachShader(program_id, vertex_shader);
    glAttachShader(program_id, fragment_shader);
    glLinkProgram(program_id);

    GLint error;
    glGetProgramiv(program_id, GL_LINK_STATUS, &error);

    if (!error)
    {
        std::cout << "Program linking WTF?" << std::endl;
        glDeleteProgram(program_id);
    }
}

void Font::Draw(void)
{
    glUseProgram(program_id);

    // Enable blending
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    GLfloat black[] = {0.0, 0.0, 0.0, 1.0};

    GLint attribute_coord = glGetAttribLocation(program_id, "coord");
    GLint uniform_color = glGetUniformLocation(program_id, "color"); 
    GLint uniform_tex = glGetUniformLocation(program_id, "tex");

    FT_Set_Pixel_Sizes(face, 0, 48);
    glUniform4fv(uniform_color, 1, black);

    GLuint tex;
    glActiveTexture(GL_TEXTURE0);
    glGenTextures(1, &tex);
    glBindTexture(GL_TEXTURE_2D, tex);
    glUniform1i(uniform_tex, 0);

    // Set alignment restriction for data different from 4-byte
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);


    GLuint vbo;
    glGenBuffers(1, &vbo);
    glEnableVertexAttribArray(attribute_coord);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glVertexAttribPointer(attribute_coord, 4, GL_FLOAT, GL_FALSE, 0, 0);

    RenderText("Hello, world!", -0.3f, 0.f, 2.f / 800, 2.f / 600);

    glDisableVertexAttribArray(attribute_coord);
    glDeleteTextures(1, &tex);

    glUseProgram(0);
}

void Font::RenderText(const char *text, float x, float y, float sx, float sy)
{
    const char *p = nullptr;
    FT_GlyphSlot glyph = face->glyph;

    for (p = text; *p; ++p)
    {
        if (FT_Load_Char(face, *p, FT_LOAD_RENDER))
        {
            continue;
        }

        glTexImage2D(GL_TEXTURE_2D, 0, GL_ALPHA, glyph->bitmap.width, glyph->bitmap.rows, 0, GL_ALPHA, GL_UNSIGNED_BYTE, glyph->bitmap.buffer);

        float x2 = x + glyph->bitmap_left * sx;
        float y2 = -y - glyph->bitmap_top * sy;
        float w = glyph->bitmap.width * sx;
        float h = glyph->bitmap.rows  * sy;

        point box[4] = {{x2, -y2, 0, 0}, {x2 + w, -y2, 1, 0}, {x2, -y2 - h, 0, 1}, {x2 + w, -y2 - h, 1, 1}};

        // Draw the caracter on the screen
        glBufferData(GL_ARRAY_BUFFER, sizeof(box), box, GL_DYNAMIC_DRAW);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

        // Advance the cursor to the start of the next character
        x += (glyph->advance.x >> 6) * sx;
        y += (glyph->advance.y >> 6) * sy;
    }
}

GLuint Font::MakeShader(GLenum type, const std::string &source)
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
