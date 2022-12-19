#pragma once
#include "Utils.h"

class CMapLayer
{
private:
	int id;
	int width; // so row
	int height; // so col

	int** tiles; 

public:
	CMapLayer();
	~CMapLayer();

	CMapLayer(TiXmlElement* data);

	bool Hidden = false;

	int GetTileID(int x, int y);

};

