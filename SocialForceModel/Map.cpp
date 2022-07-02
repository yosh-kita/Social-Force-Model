#include "Map.h"
#include "Dxlib.h"

Map::Map() {
	h = 100;
	w = 100;
	graphHandle = LoadGraph("Graphics/Map.png");
}

Map::Map(double width, double height) {
	w = width;
	h = height;
	graphHandle = LoadGraph("Graphics/Map.png");
}

//ï`é ä÷êî
void Map::Draw(int originX, int originY, double drawMagnification) {
	DrawExtendGraph(originX, originY, originX + static_cast<int>(w * drawMagnification), originY + static_cast<int>(h * drawMagnification), graphHandle, FALSE);
}