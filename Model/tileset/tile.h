#ifndef TILE_H
#define TILE_H

/*!
*	\file tile.h
*	\brief the tile representation
*	\author Guillaume COUTABLE
*	\version 1.0
*/

#include<map>
#include<vector>
#include<string>

class TileSet;

template< typename T>
struct point_less
{
    bool operator() (const T* x, const T* y)
	{
		return *x < *y;
	}
};

/**
 * @class Tile
 * @brief the tile class
 *
 */
class Tile
{

private:
    const int tileId_; /**< the tile id in a specific tileset */
    TileSet* tileSet_; /**< the tilset that contain this tile */
    std::map<std::string, std::string> properties_; /**< all the properties of this tile */
	//tile position in the tileset
    const int posX_; /**< the tile X position in the tileset, calculate by tileId_ mod (number of tile in a row) */
    const int posY_; /**< the tile Y position in the tileset, calculate by tileId_ div (number of tile in a row) */
	
public:
    /*!
     * \brief the empty Tile representation
     */
	Tile();

    /**
     * @brief Destructor
     *
     *  Tile destructor
     *
     */
    ~Tile();

    /**
     * @brief Constructor
     *
     *  Tile constructor
     *
     * @param tileId the tile id
     * @param tileSet the tilset that contain this tile
     * @param nbTileperRow the number of tile per row in the tileset, use to calculate the tile position in its tileset owner
     */
    Tile(const int& tileId, TileSet* tileSet, const int& nbTileperRow);

    /**
     * @brief add a property on the tile
     *
     * @param propertyName the new property name
     * @param propertyValue the new property value
     */
    void addProperty(std::string propertyName, std::string propertyValue);

    /**
     * @brief remove a tile property
     *
     * @param propertyName the property name to remove
     */
    void removeProperty(std::string propertyName);

    /**
     * @brief return all tile properties name
     *
     * @return the tile properties name
     */
    std::vector<std::string> getPropertiesName();

    /**
     * @brief return the tile property value associate with the given property name
     *
     * @param propertyName the property name
     * @return the property value to return
     */
    std::string getPropertyValue(std::string propertyName);

    /**
     * @brief return a copy of the tile properties
     *
     * @return the copy of the tile properties
     */
    std::map<std::string, std::string> getProperties();

    /**
     * @brief return the tile id
     *
     * @return the tile id
     */
    const int& getTileId();

    /**
     * @brief return the owner TileSet
     *
     * @return the owner TileSet
     */
    TileSet* getTileSet();

    /**
     * @brief return the Tile X_position in the TileSet
     *
     * @return the Tile X_position in the TileSet
     */
    int getTilePositionX() const;

    /**
     * @brief the Tile the Y_position in the TileSet
     *
     * @return the Y_position in the TileSet
     */
    int getTilePositionY() const;

    /**
     * @brief compare two Tile by their Id
     *
     * @param t the other Tile to compare
     * @return true if this is lower than t, false otherwise
     */
    bool operator<(const Tile& t) const;

    /**
     * @brief compare two Tile by their Id
     *
     * @param t the other Tile to compare
     * @return true if this is greater than t, false otherwise
     */
    bool operator>(const Tile& t) const;

    /**
     * @brief compare two Tile by their Id
     *
     * @param t the other Tile to compare
     * @return true if the two tiles are equals, false otherwise
     */
    bool operator==(const Tile& t) const;

    /**
     * @brief compare two Tile by their Id
     *
     * @param t the other Tile to compare
     * @return true if the two tiles are differents, false otherwise
     */
    bool operator!=(const Tile& t) const;

};

class ComparatorTile
{
    bool operator()(const Tile& t1, const Tile& t2);
};

#endif
