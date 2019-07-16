#include "Sprite.h"
#include <iostream>

bool stVec2Compare (const glm::vec2& a, const glm::vec2& b) {
	return (a.x < b.x) || (a.x == b.x && a.y < b.y);
};

MSprite::MSprite() {
	Width = 0;
	Height = 0;
	BytesPerPixel = 0;
	Position = glm::vec2(0, 0);
}

void MSprite::Clear() {
	Points.clear();
	Colors.clear();
}

void MSprite::AddVisibleEdge(unsigned long Edge) {
	std::vector<glm::vec2>::iterator it_min;
	std::vector<glm::vec2>::iterator it_max;
	it_min = std::min_element(Points.begin(), Points.end(), stVec2Compare);
	it_max = std::max_element(Points.begin(), Points.end(), stVec2Compare);
	for (unsigned long i = 0; i < Width; i++) {
		Points.push_back(glm::vec2(i, 0));
		Colors.push_back(glm::vec4(1, 1, 1, 1));
		Points.push_back(glm::vec2(i, Height - 1));
		Colors.push_back(glm::vec4(1, 1, 1, 1));
	}
	for (unsigned long i = 0; i < Height; i++) {
		Points.push_back(glm::vec2(0, i));
		Colors.push_back(glm::vec4(1, 1, 1, 1));
		Points.push_back(glm::vec2(Width - 1 ,i));
		Colors.push_back(glm::vec4(1, 1, 1, 1));
	}
}

void MSprite::CropSize() {
	std::vector<glm::vec2>::iterator it_min;
	std::vector<glm::vec2>::iterator it_max;
	it_min = std::min_element(Points.begin(), Points.end(), stVec2Compare);
	it_max = std::max_element(Points.begin(), Points.end(), stVec2Compare);
	Width = (int)(it_max->x - it_min->x + 1) + Edge;
	Height = (int)(it_max->y - it_min->y + 1) + Edge;
}

void MSprite::SetWidth(unsigned long inWidth) {
	Width = inWidth;
}

void MSprite::SetHeight(unsigned long inHeight) {
	Height = inHeight;
}

void MSprite::SetBytesPerPixel(unsigned char inBytesPerPixel) {
	BytesPerPixel = inBytesPerPixel;
}

void MSprite::AddPoint(glm::vec2 Point) {
	Points.push_back(Point);
}

void MSprite::AddColor(float Color) {
	Colors.push_back(glm::vec4(Color, 0.0f, 0.0f, 0.0f));
}

void MSprite::AddColor(glm::vec2 Color) {
	Colors.push_back(glm::vec4(Color, 0.0f, 0.0f));
}

void MSprite::AddColor(glm::vec3 Color) {
	Colors.push_back(glm::vec4(Color, 0.0f));
}

void MSprite::AddColor(glm::vec4 Color) {
	Colors.push_back(Color);
}

std::vector<glm::vec2>* MSprite::GetPoints() {
	return &Points;
}

std::vector<glm::vec4>* MSprite::GetColors() {
	return &Colors;
}

