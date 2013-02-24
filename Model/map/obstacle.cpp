//
//  obstacle.cpp
//  
//
//  Created by Noémie RULLIER on 10/12/12.
//
//

#include "obstacle.h"

Obstacle::Obstacle(): tileId_(-1)
{}

Obstacle::Obstacle(Tile * tile, const int& tileId, const std::string& type, const std::map<std::string,std::string>& propriete):
tile_(tile), tileId_(tileId), type_(type), propriete_(propriete) {}

Obstacle::Obstacle(const Obstacle& o):
tile_(o.tile_), tileId_(o.tileId_), type_(o.type_), propriete_(o.propriete_) {}

Obstacle::~Obstacle(){}

Tile * Obstacle::getTile() const{
	return tile_;
}

int Obstacle::getTileId() const{
	return tileId_;
}

std::string Obstacle::getType() const{
	return type_;
}

std::map<std::string,std::string> Obstacle::getPropriete() const{
	return propriete_;
}

void Obstacle::setTile(Tile * tile){
	tile_=tile;
}

void Obstacle::setTileId(int& tileId){
	tileId_=tileId;
}

void Obstacle::setType(std::string& type){
	type_=type;
}

void Obstacle::setPropriete(std::map<std::string,std::string>& propriete){
	propriete_=propriete;
}

std::ostream& operator<<(std::ostream& os, const Obstacle& o)
{
	os << "l'obstacle " << o.getTileId() << " de type : " << o.getType().c_str();
	return os;
}

bool Obstacle::operator==(const Obstacle& o) const
{
	return tileId_ == o.tileId_;
}

bool Obstacle::operator!=(const Obstacle& o) const
{
	return tileId_ != o.tileId_;
}
