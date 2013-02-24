#ifndef MAP_H
#define MAP_H

/*!
*	\file map.h
*	\brief the map representation
*	\author Guillaume COUTABLE
*	\version 1.0
*/

#include <vector>
#include "background.h"
#include "field.h"
#include "objectLayers.h"
#include "../tileset/tileSet.h"
#include <iostream>



/*!
 *  @class Map
 *
 */
class Map {

public:
	
	//Enumeration that specify the map orientation style
	//for our program, we only need the ORTHOGONAL orientation style.
    /**
     * @brief Displaying Tile information
     *
     *  the MAPORIENTATION give the mean to display the tile, depends on the tileset
     *
     */
    enum MAPORIENTATION{
		MAPORIENTATION_ORTHOGONAL,
		MAPORIENTATION_ISOMETRIC
	};

    /*!
     * \brief Constructor
     *
     *
     * \param layerBg the background layer
     * \param layerField the field layer
     * \param tailleX the map width
     * \param tailleY the map height
     * \param mapOrient the mean to display the tile
     */
	Map(Background* layerBg, Field* layerField, const int& tailleX, const int& tailleY, const MAPORIENTATION& mapOrient);
	
	//Destructeur
    /**
     * @brief Destructor
     *
     *  The Map destructor
     *
     */
    ~Map();

	//Getters
    /**
     * @brief the map width accessor
     *
     * @return the map width
     */
    int getTailleX() const;

    /**
     * @brief the map height accessor
     *
     * @return the map height
     */
    int getTailleY() const;

    /**
     * @brief the map orientation accessor
     *
     * @return the map orientation
     */
    MAPORIENTATION getMapOrientation() const;

    /**
     * @brief the background accessor
     *
     * @return the background layer
     */
    Background* getBackground() const;

    /**
     * @brief the field accessor
     *
     * @return the field layer
     */
    Field* getField() const;

    /**
     * @brief the objects layers accessor
     *
     * @return all the objects layers
     */
    std::vector<ObjectLayers*> getObjectLayers() const;

    /**
     * @brief the tilesets accessor
     *
     * @return all the tilesets
     */
    std::vector<TileSet*> getTileSets() const;

    /**
     * @brief an accessor to all layers
     *
     * @return all the layers
     */
    std::vector<Layers*> getAllLayers();
	
	
	
	//Methods
    /**
     * @brief add all tilesets needed
     *
     * @param tileSets the tilesets needed
     */
    void addTileSets(std::vector<TileSet*> tileSets);

    /**
     * @brief add a object layer
     *
     * @param layer the layer object to add
     */
    void addObjectLayers(ObjectLayers* layer);

private:
    const int tailleX_; /**< the map width */
    const int tailleY_; /**< the map height */
    const MAPORIENTATION orientation_; /**< the mean to display the tile */
    Background* background_; /**< the background layer */
    Field* field_; /**< the field layer */
    std::vector<ObjectLayers*> objectLayers_; /**< all the layers object*/
    std::vector<TileSet*> tileSets_; /**< all the tilesets */

};

std::ostream& operator<<(std::ostream& os, const Map& map);

#endif

