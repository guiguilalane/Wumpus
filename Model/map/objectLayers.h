//
//  objectLayers.h
//  
//
//  Created by Noémie RULLIER on 10/12/12.
//
//

#ifndef OBJECTLAYERS_H
#define OBJECTLAYERS_H

#include "layers.h"

/**
 * \class ObjectLayers
 * \brief ObjectLayers's representation of the map.
 This layers is the layers which is composed by all the objects.
 */
class ObjectLayers : public Layers{
public:
	
	/**
	 * \brief Constructor
	 * \param x the Layer's width
	 * \param y the Layer's height
	 * \param cellsMap the cells of the layers
	 */
	ObjectLayers(const int& x, const int& y, const ensCells& cellsMap);
	
	/**
	 * \brief Constructor
	 * \param x the Layer's width
	 * \param y the Layer's height
	 * \param cellsTab the cells of the layers
	 */
	ObjectLayers(const int& x, const int& y, const std::vector<Cell*>& cellsTab);
	
	/**
	 * \brief Destructor
	 */
	~ObjectLayers();
	
	//! \name Getters
	//@{
	virtual int getTailleX() const;
	virtual int getTailleY() const;
	virtual ensCells getCellsTab() const;
	//@}
	
	
	//! \name Setters
	//@{
	virtual void setTailleX(int& tailleX);
	virtual void setTailleY(int& tailleY);
	virtual void setCellsTab(ensCells& cellsMap);
	//@}
	
	virtual std::ostream& operator<<(std::ostream& os);
};

#endif
