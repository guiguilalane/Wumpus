//
//  obstacle.h
//  
//
//  Created by Noémie RULLIER on 10/12/12.
//
//

#ifndef OBSTACLE_H
#define OBSTACLE_H

#include <string>
#include <map>
#include <iostream>

#include "../tileset/tile.h"

/**
 * \class Obstacle
 * \brief Representation of an obstacle on the map. This obstacle is a tile with parameters to know if the obstacle is passable or not and if is alterable or not.
 */
class Obstacle
{
public:

	/**
	 * \brief Constructor
	 * \param tileset
	 * \param type
	 * \param tileId
	 * \param propriete All the properties of the obstacle
	 */
	Obstacle(Tile * tile, const int& tileId, const std::string& type, const std::map<std::string,std::string>& propriete);
	
	/**
	 * \brief Default constructor
	 */
	Obstacle();
	
	/**
	 * \brief Copy destructor
	 */
	Obstacle(const Obstacle& o);
	
	/**
	 * \brief Destructor
	 */
	~Obstacle();
	
	
	//! \name Getters
	//@{
	Tile* getTile() const;
	int getTileId() const;
	std::string getType() const;
	std::map<std::string,std::string> getPropriete() const;
	//@}
	
	//! \name Setters
	//@{
	void setTile(Tile * tile);
	void setTileId(int& tileId);
	void setType(std::string& type);
	void setPropriete(std::map<std::string,std::string>& propriete);
	//@}
	
	bool operator==(const Obstacle& o) const;
	
	bool operator!=(const Obstacle& o) const;
private:
	
	Tile * tile_;
	int tileId_;
	std::string type_;
	std::map<std::string,std::string> propriete_;
	
};

std::ostream& operator<<(std::ostream& os, const Obstacle& o);


#endif
