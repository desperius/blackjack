#ifndef _FONT_H_
#define _FONT_H_

// FreeType library types
typedef struct FT_LibraryRec_ *FT_Library;
typedef struct FT_FaceRec_    *FT_Face;

// OpenGL types
typedef unsigned int GLuint;
typedef unsigned int GLenum;

class Font
{
public:
    Font(void);
    ~Font(void);

    void Init(void);
    void Load(void);
    void Draw(void);
    void RenderText(const char *text, float x, float y, float sx, float sy);

protected:
    Font(const Font &);
    Font & operator= (const Font &);

private:
    static GLuint MakeShader(GLenum type, const std::string &source);

private:
    FT_Library library;
    FT_Face    face;

    GLuint vertex_shader;
    GLuint fragment_shader;
    GLuint program_id;
};

#endif /* _FONT_H_ */