#ifndef spriteH
#define spriteH

#include <vector>
#include <algorithm>
#include <glm/glm.hpp>

class MSprite {
protected:
	unsigned long Edge;
	unsigned long Width;
	unsigned long Height;
	unsigned char BytesPerPixel;
	glm::vec2 Position;
	std::vector<glm::vec2> Points;
	std::vector<glm::vec4> Colors;
public:
	MSprite();
	void Clear();
	void AddVisibleEdge(unsigned long Edge);
	void CropSize();
	void SetWidth(unsigned long inWidth);
	void SetHeight(unsigned long inHeight);
	void SetBytesPerPixel(unsigned char inBytesPerPixel);
	void AddPoint(glm::vec2 Point);
	void AddColor(float Color);
	void AddColor(glm::vec2 Color);
	void AddColor(glm::vec3 Color);
	void AddColor(glm::vec4 Color);
	std::vector<glm::vec2>* GetPoints();
	std::vector<glm::vec4>* GetColors();
};

#endif