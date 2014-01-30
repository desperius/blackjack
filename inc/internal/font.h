#ifndef _FONT_H_
#define _FONT_H_

typedef struct FT_LibraryRec_ *FT_Library;
typedef struct FT_FaceRec_    *FT_Face;

class Font
{
public:
	Font(void);
	~Font(void);

	void Init(void);

protected:
	Font(const Font &);
	Font & operator= (const Font &);

private:
	FT_Library library;
	FT_Face    face;
};

#endif /* _FONT_H_ */