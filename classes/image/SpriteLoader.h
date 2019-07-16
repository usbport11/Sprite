#ifndef spriteloaderH
#define spriteloaderH

#include "Tga.h"
#include "Png.h"
#include "Sprite.h"
#include <iostream>

#define FILETGA 0x00
#define FILEPNG 0x89

struct stImage {
	unsigned char* pData;
	unsigned long Width;
	unsigned long Height;
	unsigned int BitsPerPixel;
	unsigned char FileType[1];
};

class MSpriteLoader {
private:
	MTga* Tga;
	MPng* Png;
	stImage Image;
	unsigned char RemoveColor[4];
	bool GetImage(const char* FileName);
	void ClearImage();
	void ClearImageData();
public:
	bool Initialize();
	MSprite* GetSprite(const char* FileName);
	void SetRemoveColor(glm::vec4 inRemoveColor);
	void Close();
};

#endif