#include "map.h"


Map::Map(Background* layerBg, Field* layerField, const int& tailleX, const int& tailleY, const MAPORIENTATION& mapOrient): tailleX_(tailleX), tailleY_(tailleY), orientation_(mapOrient), background_(layerBg), field_(layerField) {}

Map::~Map()
{

}

int Map::getTailleX() const
{
	return tailleX_;
}

int Map::getTailleY() const
{
	return tailleY_;
}

Background* Map::getBackground() const{
	return background_;
}

Field* Map::getField() const{
	return field_;
}

std::vector<ObjectLayers*> Map::getObjectLayers() const{
	return objectLayers_;
}

std::vector<TileSet*> Map::getTileSets() const
{
	return tileSets_;
}

void Map::addObjectLayers(ObjectLayers* layer)
{
	objectLayers_.push_back(layer);
}

void Map::addTileSets(std::vector<TileSet*> tileSets)
{
	tileSets_.insert(tileSets_.end(), tileSets.begin(), tileSets.end());
}

std::vector<Layers*> Map::getAllLayers()
{
	std::vector<Layers*> layers;
	layers.push_back(field_);
	std::vector<ObjectLayers*>::iterator it;
	for(it = objectLayers_.begin(); it != objectLayers_.end(); ++it)
	{
		layers.push_back((*it));
	}
	return layers;
}

std::ostream& operator<<(std::ostream& os, const Map& map)
{
	os << "la map possede " << map.getObjectLayers().size()+2 << " layers : \n";
	map.getBackground()->operator<<(os);
	map.getField()->operator<<(os);
	
	std::vector<ObjectLayers*>::iterator it;
	std::vector<ObjectLayers*> ly = map.getObjectLayers();
	for(it = ly.begin(); it != ly.end(); ++it)
	{
		(*it)->operator<<(os);
	}
 	return os;
}
