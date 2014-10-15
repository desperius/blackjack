#ifndef _BJ_IMAGE_H_
#define _BJ_IMAGE_H_

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

class bjImage
{
public:
    bjImage(void);
    ~bjImage(void);

    void Init(void);
    void Load(const char *path);
    void Draw(void);

protected:
    bjImage(const bjImage &);
    bjImage & operator= (const bjImage &);

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

#endif /* _BJ_IMAGE_H_ */