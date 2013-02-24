#ifndef MAPFACTORY_H
#define MAPFACTORY_H

/*!
*	\file mapFactory.h
*	\brief object creator
*	\author Guillaume COUTABLE
*	\version 1.0
*/

#include "../tileset/tileSet.h"
#include "../map/map.h"
#include <vector>

/*!
*	\class MapFactory
*	\brief the objects creator class
*	
*	This class create all objects that use to represent a map
*/
class MapFactory{

	public:
		
		/*!
		*	\brief get an instance of MapFactory
		*	\return This MapFactory object if a instance allready exist, a new instance otherwise
		*/
		static MapFactory* getInstance();
		
		/*!
		*	\brief destroy this instance of MapFactory object
		*/
		static void kill();
		
		
		
		/*!
		*	\brief Create and allocate Map object
		*	\param layerBg the Background of the Map
		*	\param layerField the Field of the Map
		*	\param tailleX the Map width
		*	\param tailleY the Map height
		*	\param mapOrient the Map orientation
		*	\return A new instance of Map object
		*/
		Map* createMap(Background* layerBg, Field* layerField, const int& tailleX=300, const int& tailleY=300, const Map::MAPORIENTATION& mapOrient=Map::MAPORIENTATION_ORTHOGONAL);
		
		/*!
		*	\brief Create and allocate Background object
		*	\return A nex instance of Background object
		*/
		Background* createBackground();
		
		/*!
		*	\brief Create and allocate Field object
		*	\param tileset the path of the tileset
		*	\param x the Field width
		*	\param y the Field height
		*	\param cellsMap the cells contained in field
		*	\return A new instance of Field object
		*/
		Field* createField(const int& x, const int& y, const ensCells& cellsMap);
		
		/*!
		*	\brief Create and allocate ObjectLayers object
		*	\param tileset the path of the tileset
		*	\param x the object container layer width
		*	\param y the object container layer height
		*	\param cellsMap the cells contained in object container layer
		*	\return A new instance of ObjectLayers object
		*/
		ObjectLayers* createObjectLayers(const int& x, const int& y, const ensCells& cellsMap);
		
		/*!
		*	\brief Create and allocate Cell object
		*	\param x the cell abscissa
		*	\param y the cell ordinate
		*	\param tile the obstacle in the cell
		*	\return A new instance of Cell object
		*/
		Cell* createCell(const int& x, const int& y, Obstacle * tile);
		
		/*!
		*	\brief Create and allocate Obstacle object
		*	\param passable is the obstacle passable?
		*	\param alterable is the obstacle alterable?
		*	\param tileId the id of the tile
		*	\return A new instance of Obstacle object
		*/
		Obstacle* createObstacle(Tile * tile, const int& tileId, const std::string& type, const std::map<std::string,std::string>& propriete);
		
		
		
		
	private:
		
		static MapFactory* mapFactory_; /*!< This instance of MapFactory object */
	
		/*!
		*	\brief Constructor
		*
		*	MapFactory constructor
		*/
		MapFactory();
		
		/*!
		*	\brief Destructor
		*
		*	MapFactory destructor
		*/
		~MapFactory();
};

#endif
