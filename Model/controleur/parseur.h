#ifndef PARSEUR_HPP
#define PARSEUR_HPP

/*!
*	\file Parseur.h
*	\brief the tmx parsor
*	\author Guillaume COUTABLE
*	\version 1.0
*/


#include "../../lib/pugixml_sources/pugixml.hpp"
#include "mapFactory.h"
#include <iostream>
#include <string>

/*!
*	\class Parseur
*	\brief the tmx parsor class
*	
*	This class read tmx files and create all class in reference to files tilesets
*/
class Parseur{

private:
    pugi::xml_document doc_; /**< the tmx file */
    MapFactory* mapFactory_; /**< the instance of MapFactory */
	
public:

    /*!
     *  \brief Constrcutor
     *
     *  Parseur constructor
     *
     *  \param the tmx file path to parse
     */
	Parseur(char* chemin);

    /*!
     *  \brief Destructor
     *
     *  Parseur destructor
     */
    /**
     * @brief
     *
     */
    ~Parseur();
	

    /**
     * @brief Create and allocate a TileSet object
     *
     * @param firstGid the first tile id
     * @param name the name of the tileset
     * @param tileWidth the width of a tile in the tileset
     * @param tileHeight the height of a tile in the tileset
     * @param image the TileSetImage
     * @return the new TileSet
     */
    TileSet* createTileSet(const int& firstGid, const char* name, const int tileWidth, const int tileHeight, const TileSetImage* image);
		
    /**
     * @brief Create and allocate a TileSetImage
     *
     * The TileSetImage represent the image of a tileset
     *
     * @param source the image that represent the tileset path
     * @param width the width of the image
     * @param height the height of the image
     * @return the new TileSetImage
     */
    TileSetImage* createTileSetImage(const char* source, const int& width, const int height);
	
    /**
     * @brief Create and allocate a tile
     *
     * @param tileId the tile id of the tileset
     * @param tileSet the tileset whom this tile
     * @param nbTileperRow the number of tile per row in the tileset, it use to calculate the position of the tile in its tileset
     * @return the new Tile
     */
    Tile* createTile(const int& tileId, TileSet* tileSet, const int& nbTileperRow);
		
    /**
     * @brief Parse the tmx file
     *
     *  Parse the tmx file : create and allocate all the map composants and return the Map object, the parsing result.
     *
     * @return a Map object, the parsing result
     */
    Map* doParsing();
};

#endif
