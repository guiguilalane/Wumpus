//
//  field.h
//  
//
//  Created by Noémie RULLIER on 08/12/12.
//
//

#ifndef FIELD_H
#define FIELD_H

#include "layers.h"

/**
 * \class Field
 * \brief Field's representation of the map.
 This layers is the layers is the field of the map.
 */
class Field : public Layers{
public:
	
	/**
	 * \brief Constructor
	 * \param x the Layer's width
	 * \param y the Layer's height
	 * \param cellsMap the coordinate and the cells of the layers
	 */
	Field(const int& x, const int& y, const ensCells& cellsMap);
	
	/**
	 * \brief Constructor
	 * \param x the Layer's width
	 * \param y the Layer's height
	 * \param cellsTab the cells of the layers
	 */
	Field(const int& x, const int& y, const std::vector<Cell*>& cellsTab);
	
	/**
	 * \brief Destructor
	 */
	~Field();
	
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
	
//	virtual std::ostream& operator<<(std::ostream& os, const Layer& ly);
//	virtual std::ostream& operator<<(std::ostream& os, const Background& bg);
//	virtual std::ostream& operator<<(std::ostream& os, const Field& fd);
//	virtual std::ostream& operator<<(std::ostream& os, const ObjectLayers& ol);
};


#endif
