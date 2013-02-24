//
//  cell.h
//  
//
//  Created by Noémie RULLIER on 19/11/12.
//
//

#ifndef CELL_H
#define CELL_H

#include <stdlib.h>

#include "obstacle.h"


/** 
  * \class Cell
  * \brief Representation of on cell on a map
  */
class Cell
{
public:
	
	/**
	 * \brief Constructor with three parameters
	 * \param x Abscissa
	 * \param y Ordinate
	 * \param obstacle Obstacle
	 */
	Cell(const int& x, const int& y, Obstacle * obstacle);
	
	/** 
	  * \brief Constructor with two parameters
	  * \param x Abscissa
	  * \param y Ordinate
	  */
	Cell(const int& x, const int& y);

	/**
	  * \brief Copy constructor
	  * \param c the cell to copy
	  */
	Cell(const Cell& c);
	
	/**
	  * \brief Destructor
	  */
	~Cell();
	
	//! \name Getters
	//@{
	int getX() const;
	int getY() const;
	Obstacle* getObstacle() const;
	//@}
	
	//! \name Setters
	//@{
	void setX(int& x);
	void setY(int& y);
	void setObstacle(Obstacle* obstacle);
	//@}
	
private:
	int x_;  //! abscissa
	int y_;  //! ordinate
	Obstacle* obstacle_;
};

std::ostream& operator<<(std::ostream& os, const Cell& c);

#endif
