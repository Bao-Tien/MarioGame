#include "MapLayer.h"
#include <vector>
#include <string>
#include "Utils.h"

CMapLayer::CMapLayer()
{
	this->id = 0;
	this->width = 1;
	this->height = 1;
}

CMapLayer::CMapLayer(TiXmlElement* data)
{
	data->QueryIntAttribute("id", &this->id);
	data->QueryIntAttribute("width", &this->width);
	data->QueryIntAttribute("height", &this->height);

	tiles = new int* [this->width];

	const char* content = data->FirstChildElement()->GetText();
	std::vector<std::string> splitted = split(content, ","); //mang 1 chieu

	for (int i = 0; i < this->width; i++) {
		tiles[i] = new int[height];
		for (int j = 0; j < this->height; j++) {
			tiles[i][j] = stoi(splitted[i + j * width]); // chuyen sang mang 2 chiu
		}
	}

	splitted.clear();
}

int CMapLayer::GetTileID(int x, int y)
{
	return tiles[x][y];
}

CMapLayer::~CMapLayer()
{
	for (int i = 0; i < width; i++) {
		delete[] tiles[i];
	}
	delete[] tiles;
}
