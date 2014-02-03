#ifndef _TEXT_H_
#define _TEXT_H_

// FreeType library types
typedef struct FT_LibraryRec_ *FT_Library;
typedef struct FT_FaceRec_    *FT_Face;

// OpenGL types
typedef unsigned int GLuint;
typedef unsigned int GLenum;

#define FONT_DIR "../res/fonts/"

class Text
{
public:
    Text(void);
    ~Text(void);

    void Init(void);
    void Load(void);
    void Draw(void);
    void RenderText(const char *text, float x, float y, float sx, float sy);

protected:
    Text(const Text &);
    Text & operator= (const Text &);

private:
    static GLuint MakeShader(GLenum type, const std::string &source);

private:
    FT_Library library;
    FT_Face    face;

    GLuint vertex_shader;
    GLuint fragment_shader;
    GLuint program_id;
};

#endif /* _TEXT_H_ */