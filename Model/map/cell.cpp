//
//  cell.cpp
//  
//
//  Created by Noémie RULLIER on 19/11/12.
//
//

#include "cell.h"

Cell::Cell(const int& x, const int& y, Obstacle* obstacle):
x_(x), y_(y) , obstacle_(obstacle) {}

Cell::Cell(const int& x, const int& y):
x_(x), y_(y) {
	obstacle_=NULL;
}

Cell::Cell(const Cell& c):
x_(c.x_), y_(c.y_) {}

Cell::~Cell(){
}

int Cell::getX() const{
	return x_;
}

int Cell::getY() const{
	return y_;
}

Obstacle* Cell::getObstacle() const{
	return obstacle_;
}

void Cell::setX(int& x){
	x_=x;
}

void Cell::setY(int& y){
	y_=y;
}

void Cell::setObstacle(Obstacle* obstacle){
	obstacle_=obstacle;
}

std::ostream& operator<<(std::ostream& os, const Cell& c)
{
	os << "Cellule à la position : (" << c.getX() << ", " << c.getY() << ") contient " << *(c.getObstacle()) << std::endl;
	return os;
}
