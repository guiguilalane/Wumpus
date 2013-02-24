//
//  layers.h
//  
//
//  Created by Noémie RULLIER on 19/11/12.
//
//

#ifndef LAYERS_H
#define LAYERS_H

#include <string>
#include <vector>
#include <map>

#include "cell.h"

//a container for cells, identified by their coordinates
typedef struct std::map<std::pair<int, int>, Cell*> ensCells;

/**
 * \class Layers
 * \brief Representation of a layer on a map
 */
class Layers
{
public:
	
	/**
	 * \brief Virtual constructor
	 * \param x the Layer's width
	 * \param y the Layer's height
	 */
	Layers(const int& x, const int& y);
	
	
	
	/**
	 * \brief Virtual constructor
	 * \param x the Layer's width
	 * \param y the Layer's height
	 * \param cellsMap the cells of the layers
	 */
	Layers(const int& x, const int& y, const ensCells& cellsMap);
	 
	/**
	 * \brief Virtual destructor
	 */
	virtual ~Layers();
	
	
	//! \name Getters
	//@{
	virtual int getTailleX() const = 0;
	virtual int getTailleY() const = 0;
	virtual ensCells getCellsTab() const = 0;
	//@}
	
	//! \name Setters
	//@{
	virtual void setTailleX(int& tailleX) = 0;
	virtual void setTailleY(int& tailleY) = 0;
	virtual void setCellsTab(ensCells& cellsMap) = 0;
	//@}
	
	virtual std::ostream& operator<<(std::ostream& os) = 0;
	
	virtual Cell* findCellFromLayer(const std::pair<int, int>& p);
	
protected:

	int tailleX_;
	int tailleY_;
	ensCells cellsMap_;	

};


#endif
