//
//  background.cpp
//  
//
//  Created by Noémie RULLIER on 08/12/12.
//
//

#include "background.h"

Background::Background(): Layers(300, 300)
{}

Background::~Background(){}


int Background::getTailleX() const
{
	return tailleX_;
}

int Background::getTailleY() const
{
	return tailleY_;
}

ensCells Background::getCellsTab() const
{
	return cellsMap_;
}

void Background::setTailleX(int& tailleX){
	tailleX_=tailleX;
}

void Background::setTailleY(int& tailleY){
	tailleY_=tailleY;
}

void Background::setCellsTab(ensCells& cellsMap){
	cellsMap_=cellsMap;
}

std::ostream& Background::operator<<(std::ostream& os)
{
	os << "\t le background d'une taille de " << tailleX_ << " * " << tailleY_ << std::endl;
	return os;
}

//std::ostream& Background::operator<<(std::ostream& os, const Background& bg)
//{
//	os << "\t le background d'une taille de " << 
//}

//std::ostream& Background::operator<<(std::ostream& os, const Field& fd)
//{
//	
//}
//std::ostream& Background::operator<<(std::ostream& os, const ObjectLayers& ol)
//{
//	
//}
