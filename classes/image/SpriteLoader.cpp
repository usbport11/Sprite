#include "SpriteLoader.h"

bool MSpriteLoader::Initialize() {
	try {
		Tga = new MTga;
		Png = new MPng;
	}
	catch (std::bad_alloc& ba) {
		return false;
	}

	ClearImageData();

	memset((void*)RemoveColor, 0, 4 * sizeof(unsigned char));

	return true;
}

bool MSpriteLoader::GetImage(const char* inFileName) {
	if (!inFileName) {
		return false;
	}

	errno_t err;
	FILE* pFile;
	err = fopen_s(&pFile, inFileName, "rb");
	if (!pFile || err) {
		return false;
	}
	fread((void*)Image.FileType, 1, 1, pFile);
	fclose(pFile);

	switch (Image.FileType[0]) {
	case FILEPNG:
		if (!Png->LoadPNG(inFileName)) {
			Png->FreePng();
			return false;
		}
		Image.pData = Png->GetImageData();
		Image.Width = Png->GetWidth();
		Image.Height = Png->GetHeight();
		Image.BitsPerPixel = Png->GetBitsPerPixel();
		break;
	case FILETGA:
		if (!Tga->LoadTGA(inFileName)) {
			Tga->FreeTga();
			return false;
		}
		Image.pData = Tga->GetImageData();
		Image.Width = Tga->GetWidth();
		Image.Height = Tga->GetHeight();
		Image.BitsPerPixel = Tga->GetBitsPerPixel();
		break;
	default:
		return false;
	}

	if (!Image.pData) return false;
	if (Image.Width <= 0 || Image.Height <= 0 || Image.BitsPerPixel <= 0) return false;

	return true;
}

MSprite* MSpriteLoader::GetSprite(const char* FileName) {
	if (!GetImage(FileName)) return NULL;

	int BytesPerPixel = Image.BitsPerPixel / 8;
	if (BytesPerPixel > 4) return false;

	long int TotalBytes = Image.Width * Image.Height * BytesPerPixel;
	bool* GoodPixels;
	bool Result;
	int pos, w, h;
	MSprite* Sprite = NULL;
	glm::vec4 Color;

	try {
		GoodPixels = new bool[BytesPerPixel];
		Sprite = new MSprite;
	}
	catch (std::bad_alloc& ba) {
		return NULL;
	}

	Sprite->Clear();
	Sprite->SetWidth(Image.Width);
	Sprite->SetHeight(Image.Height);
	Sprite->SetBytesPerPixel(BytesPerPixel);

	for (long int i = 0, j = 0; i<TotalBytes; i += BytesPerPixel, j++) {
		Color = glm::vec4(0, 0, 0, 0);
		Result = 0;
		memset((void*)GoodPixels, 1, BytesPerPixel * sizeof(bool));
		for (int j = 0; j<BytesPerPixel; j++) {
			if (Image.pData[i + j] == RemoveColor[j]) GoodPixels[j] = 0;
			Result |= GoodPixels[j];
			Color[j] = (float)Image.pData[i + j] / 255;
		}
		if (!Result) continue;
		pos = i / BytesPerPixel;
		h = pos / Image.Width;
		w = pos - h * Image.Width;

		Sprite->AddPoint(glm::vec2(w, h));
		Sprite->AddColor(Color);
	}

	delete[] GoodPixels;
	ClearImage();

	return Sprite;
}

void MSpriteLoader::SetRemoveColor(glm::vec4 inRemoveColor) {
	RemoveColor[0] = (unsigned char)inRemoveColor[0] * 255;
	RemoveColor[1] = (unsigned char)inRemoveColor[1] * 255;
	RemoveColor[2] = (unsigned char)inRemoveColor[2] * 255;
	RemoveColor[3] = (unsigned char)inRemoveColor[3] * 255;
}

void MSpriteLoader::ClearImage() {
	switch (Image.FileType[0]) {
	case FILEPNG:
		Png->FreePng();
		break;
	case FILETGA:
		Tga->FreeTga();
		break;
	}
	ClearImageData();
}

void MSpriteLoader::ClearImageData() {
	Image.pData = NULL;
	Image.Width = 0;
	Image.Height = 0;
	Image.BitsPerPixel = 0;
	Image.FileType[0] = FILETGA;
}

void MSpriteLoader::Close() {
	if (Tga) delete Tga;
	if (Png) delete Png;
}