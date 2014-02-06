#ifndef _IMAGE_H_
#define _IMAGE_H_

// DevIL types
typedef unsigned int ILuint;
typedef int          ILint;

// OpenGL types
typedef unsigned char GLubyte;
typedef unsigned char GLboolean;
typedef int           GLint;
typedef unsigned int  GLuint;
typedef unsigned int  GLenum;

#define IMAGE_DIR "../res/textures/"

class Image
{
public:
    Image(void);
    ~Image(void);

    void Init(void);
    void Load(const char *path);
    void Draw(void);

protected:
    Image(const Image &);
    Image & operator= (const Image &);

private:
    static GLuint MakeShader(GLenum type, const std::string &source);

private:
    ILuint    image_id;
    ILint     data_size;
    GLubyte  *image_data;
	GLint     width;
	GLint     height;
	GLboolean alpha;
    GLuint    vertex_shader;
    GLuint    fragment_shader;
    GLint     program_id;
};

#endif /* _IMAGE_H_ */