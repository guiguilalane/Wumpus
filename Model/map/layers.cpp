//
//  layers.cpp
//  
//
//  Created by Noémie RULLIER on 19/11/12.
//
//

#include "layers.h"

Layers::Layers(const int& x, const int& y) :
tailleX_(x), tailleY_(y) {}

Layers::Layers(const int& x, const int& y, const ensCells& cellsMap) :
tailleX_(x), tailleY_(y), cellsMap_(cellsMap) {}

Layers::~Layers() {}

Cell* Layers::findCellFromLayer(const std::pair<int, int>& p)
{
	ensCells::iterator it = cellsMap_.begin();
	Cell* c = NULL;
	bool find = false;
	while(!find && it != cellsMap_.end())
	{
		if(it->first.first==p.first && it->first.second==p.second)
		{
			c = it->second;
		}
		++it;
	}
	return c;
}
