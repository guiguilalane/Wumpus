#include "mapFactory.h"

MapFactory* MapFactory::mapFactory_ = NULL;

MapFactory::MapFactory()
{
}

MapFactory* MapFactory::getInstance()
{
	if(NULL == mapFactory_)
	{
		mapFactory_ = new MapFactory();
	}
	return mapFactory_;
}

void MapFactory::kill()
{
	if(NULL != mapFactory_)
	{
		delete mapFactory_;
		mapFactory_ = NULL;
	}
}

MapFactory::~MapFactory()
{
}

Map* MapFactory::createMap(Background* layerBg, Field* layerField, const int& tailleX, const int& tailleY, const Map::MAPORIENTATION& mapOrient)
{
	return new Map(layerBg, layerField, tailleX, tailleY, mapOrient);
}

Background* MapFactory::createBackground()
{
	return new Background();
}

Field* MapFactory::createField(const int& x, const int& y, const ensCells& cellsMap)
{
	return new Field(x, y, cellsMap);
}

ObjectLayers* MapFactory::createObjectLayers(const int& x, const int& y, const ensCells& cellsMap)
{
	return new ObjectLayers(x, y, cellsMap);
}

Cell* MapFactory::createCell(const int& x, const int& y, Obstacle * tile)
{
	return new Cell(x, y, tile);
}

Obstacle* MapFactory::createObstacle(Tile * tile, const int& tileId, const std::string& type, const std::map<std::string,std::string>& propriete)
{
	return new Obstacle(tile, tileId, type, propriete);
}
