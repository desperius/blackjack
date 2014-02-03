#ifndef _IMAGE_H_
#define _IMAGE_H_

// DevIL types
typedef unsigned int ILuint;
typedef int          ILint;

// OpenGL types
typedef unsigned char GLubyte;
typedef unsigned char GLboolean;
typedef int           GLint;

#define IMAGE_DIR "../res/textures/"

class Image
{
public:
    Image(void);
    ~Image(void);

    void Init(void);
    void Load(const char *path);

protected:
    Image(const Image &);
    Image & operator= (const Image &);

private:
    ILuint    image_id;
    ILint     data_size;
    GLubyte  *image_data;
	GLint     width;
	GLint     height;
	GLboolean alpha;
};

#endif /* _IMAGE_H_ */