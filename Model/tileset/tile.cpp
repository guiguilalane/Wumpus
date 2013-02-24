#include "tile.h"

Tile::Tile(): tileId_(-1), posX_(-1), posY_(-1)
{
}

Tile::Tile(const int& id, TileSet* tileSet, const int& nbTileperRow): tileId_(id), tileSet_(tileSet), posX_(id % nbTileperRow), posY_(id / nbTileperRow)
{
}

Tile::~Tile()
{
    tileSet_ = NULL;
}

void Tile::addProperty(std::string propertyname, std::string propertyValue)
{
	properties_.insert ( std::pair<std::string, std::string>(propertyname, propertyValue) );
}

void Tile::removeProperty(std::string propertyName)
{
	properties_.erase ( propertyName );
}

std::vector<std::string> Tile::getPropertiesName()
{
	std::vector<std::string> v;
	std::map<std::string, std::string>::iterator it;

	for(it = properties_.begin(); it != properties_.end(); ++it)
	{
		v.push_back(it->first);
	}

	return v;
}

std::string Tile::getPropertyValue(std::string propertyName)
{
	return (properties_.find(propertyName))->second;
}

std::map<std::string, std::string> Tile::getProperties()
{
	return properties_;
}

const int& Tile::getTileId()
{
	return tileId_;
}

TileSet* Tile::getTileSet()
{
	return tileSet_;
}

int Tile::getTilePositionX() const
{
	return posX_;
}

int Tile::getTilePositionY() const
{
	return posY_;
}

bool Tile::operator<(const Tile& t) const
{
	return tileId_ < t.tileId_;
}

bool Tile::operator>(const Tile& t) const
{
	return tileId_ > t.tileId_;
}

bool Tile::operator==(const Tile& t) const
{
	return tileId_ == t.tileId_;
}

bool Tile::operator!=(const Tile& t) const
{
	return tileId_ != t.tileId_;
}

bool ComparatorTile::operator()(const Tile& t1, const Tile& t2)
{
	return t1 < t2;
}
