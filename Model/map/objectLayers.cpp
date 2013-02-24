//
//  objectLayers.cpp
//  
//
//  Created by Noémie RULLIER on 10/12/12.
//
//

#include "objectLayers.h"

ObjectLayers::ObjectLayers(const int& x, const int& y, const ensCells& cellsMap): Layers(x, y, cellsMap)
{}

ObjectLayers::ObjectLayers(const int& x, const int& y, const std::vector<Cell*>& cellsTab): Layers(x, y) {
	for(unsigned int i=0; i<cellsTab.size();i++){
		cellsMap_[std::pair<int,int>(cellsTab[i]->getX(),cellsTab[i]->getY())] = cellsTab[i];
	}
}

ObjectLayers::~ObjectLayers(){}


int ObjectLayers::getTailleX() const
{
	return tailleX_;
}

int ObjectLayers::getTailleY() const
{
	return tailleY_;
}

ensCells ObjectLayers::getCellsTab() const
{
	return cellsMap_;
}

void ObjectLayers::setTailleX(int& tailleX){
	tailleX_=tailleX;
}

void ObjectLayers::setTailleY(int& tailleY){
	tailleY_=tailleY;
}

void ObjectLayers::setCellsTab(ensCells& cellsMap){
	cellsMap_=cellsMap;
}

std::ostream& ObjectLayers::operator<<(std::ostream& os)
{
	os << "\t l'ObjectLayer d'une taille de " << tailleX_ << " * " << tailleY_ << " et est composé de : \n";
	ensCells::iterator it;
	for(it = cellsMap_.begin(); it != cellsMap_.end(); ++it)
	{
		os << "\t\t" << *((*it).second);
	}
	return os;
}
