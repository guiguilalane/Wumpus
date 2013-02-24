#ifndef TILESET_H
#define TILESET_H

/*!
*	\file tileSet.h
*	\brief the tileset representation
*	\author Guillaume COUTABLE
*	\version 1.0
*/

#include <set>
#include "tileSetImage.h"
#include "tile.h"


typedef std::set<Tile*, point_less<Tile> > ensTileTrie;

/**
 * @class TileSet
 * @brief the TileSet class
 *
 */
class TileSet{

	private:
        const int firstGid_; /**< the first tile id*/
        const char* name_; /**< the name of the tileset */
        const int tileWidth_; /**< the width of a tile in the tileset */
        const int tileHeight_; /**< the height of a tile in the tileset */
        const TileSetImage* image_; /**< the TileSetImage */
        const int nbTilePerRow_; /**< the number of tile per row, it calculate by the tileset width divide by a tile width */
        ensTileTrie tiles_; /**< all the tiles in the tileset */
		
	public:

        /*!
         * \brief TileSet constructor
         *
         *  Calculate the number of tile per row (tilesetImage width divide by tileWidth)
         *
         * \param firstGid the first tile id
         * \param name the name of the tileset
         * \param tileWidth the width of a tile in the tileset
         * \param tileHeight the height of a tile in the tileset
         * \param image the TileSetImage
         */
		TileSet(const int& firstGid, const char* name, const int tileWidth, const int tileHeight, const TileSetImage* image);

        /**
         * @brief TileSetDestructor
         *
         */
        ~TileSet();
		
        /**
         * @brief add a Tile to the TileSet
         *
         * @param tile the Tile to add
         */
        void addTile(Tile* tile);

        /**
         * @brief remove a Tile from the TileSet
         *
         * @param tile the Tile to remove
         */
        void removeTile(Tile* tile);
		
        /**
         * @brief the FirstGid accessor
         *
         * @return the FirstGid
         */
        int getFirstGid() const;

        /**
         * @brief TileSet name accessor
         *
         * @return the TileSet name
         */
        const char* getName() const;

        /**
         * @brief the Tile width accessor
         *
         * @return the Tile width
         */
        int getTileWidth() const;
        /**
         * @brief the Tile height accessor
         *
         * @return the Tile height
         */
        int getTileHeight() const;

        /**
         * @brief the TileSetImage accessor
         *
         * @return the TileSetImage
         */
        const TileSetImage* getTileSetImage() const;

        /**
         * @brief a Tile accessor
         *
         *  Return the tile with the given id
         *
         * @param tileId the id of the Tile to return
         * @return the Tile with the given id
         */
        Tile* getTile(const int& tileId);

        /**
         * @brief the number of Tile Per Row accessor
         *
         * @return the number of Tile Per Row
         */
        int getNbTilePerRow();
};


#endif
