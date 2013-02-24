#include"parseur.h"
#include <cstring>
#include <cassert>

using namespace pugi;

Parseur::Parseur(char* chemin)
{
	doc_.load_file(chemin);
    mapFactory_ = MapFactory::getInstance();
}

Parseur::~Parseur()
{
	MapFactory::kill();
}

TileSet* Parseur::createTileSet(const int& firstGid, const char* name, const int tileWidth, const int tileHeight, const TileSetImage* image)
{
	return new TileSet(firstGid, name, tileWidth, tileHeight, image);
}

TileSetImage* Parseur::createTileSetImage(const char* source, const int& width, const int height)
{
	return new TileSetImage(source, width, height);
}

Tile* Parseur::createTile(const int& tileId, TileSet* tileSet, const int& nbTileperRow)
{
	return new Tile(tileId, tileSet, nbTileperRow);
}

Map* Parseur::doParsing()
{
	Field* fieldLayer;
	std::vector<ObjectLayers*> layersObjects;
	xml_node mapXml = doc_.child("map");
	int mapWidth = mapXml.attribute("width").as_int();
	int mapHeight = mapXml.attribute("height").as_int();
	std::string orientation(mapXml.attribute("orientation").value());
	//"Header" TMX file parsing (tileset tags)
	xml_node firstTileSetXml = mapXml.child("tileset");
	std::vector<TileSet*> ensTileSet;
	for(xml_node aTileSet = firstTileSetXml; aTileSet; aTileSet = aTileSet.next_sibling("tileset"))
	{
		//create new tileSetImage
		xml_node tilsetImageXml = aTileSet.child("image");
		TileSetImage* tsi = createTileSetImage(tilsetImageXml.attribute("source").value(), tilsetImageXml.attribute("width").as_int(), tilsetImageXml.attribute("height").as_int());
		
		//create new tileSet with given attributs.
		TileSet* ts = createTileSet(aTileSet.attribute("firstgid").as_int(), aTileSet.attribute("name").value(), aTileSet.attribute("tilewidth").as_int(), aTileSet.attribute("tileheight").as_int(), tsi);
		
		//get tile tags
		xml_node firstTileXml = aTileSet.child("tile");
		for(xml_node aTile = firstTileXml; aTile; aTile = aTile.next_sibling("tile"))
		{
			//foreach tile tags create new tile
			Tile* tile = createTile(aTile.attribute("id").as_int(), ts, ts->getNbTilePerRow());
//			std::cout << "tileId : " << aTile.attribute("id").as_int() << " :" << std::endl;
			xml_node firstTileProperty = aTile.child("properties").child("property");
			for(xml_node aProperty = firstTileProperty; aProperty; aProperty = aProperty.next_sibling("property"))
			{
				//get property tags and add it to tile object
				char* name;
				name = new char[strlen(aProperty.attribute("name").value())+1];
				strcpy(name, aProperty.attribute("name").value());
				char* value;
				value = new char[strlen(aProperty.attribute("value").value())];
				strcpy(value, aProperty.attribute("value").value());
				tile->addProperty(name, value);
//				std::cout << "\t property " << name << " = " << value << std::endl;
			}
			ts->addTile(tile);
			ensTileSet.push_back(ts);
		}
	}
	
	//parsing layers
	xml_node firstLayerXml = mapXml.child("layer");
	Field* field;
	int x;
	int y;
	for(xml_node aLayer = firstLayerXml; aLayer; aLayer = aLayer.next_sibling("layer"))
	{
//		std::cout << aLayer.attribute("name").value() << std::endl;
		ensCells cellsMap;
		xml_node firstCellXml = aLayer.child("data").child("tile");
		x = 0;
		y = 0;
		for(xml_node aCellXml = firstCellXml; aCellXml; aCellXml = aCellXml.next_sibling("tile"))
		{
			
			int gid = aCellXml.attribute("gid").as_int();
			//get Tile with the current gid
			Tile* t;
			bool get = false;
			TileSet* currentTileSet;
			std::vector<TileSet*>::iterator it = ensTileSet.begin();
			Obstacle* obstacle;
			while(it != ensTileSet.end() && !get)
			{
				t = (*it)->getTile(gid-(*it)->getFirstGid());
				currentTileSet = (*it);
				if((*t)!=Tile())
				{
					obstacle = mapFactory_->createObstacle(t, t->getTileId(), t->getPropertyValue("type"), t->getProperties());
					get = true;
//					std::cout << t->getTileId() << std::endl;
				}
				++it;
			}
			if(!get)
			{
				obstacle = new Obstacle();
			}
			
			//pour créer un obstacle il faut un TileSet, un tileId, le type (Field, object, fixed), les propriété
			//Pour récupérer les Cells du layers, il faut un Obstacle et la position de la cellule
			Cell* cell = mapFactory_->createCell(x, y, obstacle);
			
//			std::cout << *cell << std::endl;
			cellsMap.insert(std::pair< std::pair<int, int>, Cell*>(std::pair<int, int>(x,y), cell));
//			std::cout << "(" << x << ", " << y << ") : tileId = " << t->getTileId() << std::endl;
//			
////			
			x++;
			if(x >= mapWidth)
			{
				x=0;
				y++;
			}
//			//normaly, when x=mapWidth and y=mapHeight we have finished to parse this layer
		}
		
		//pour le layer, il faut la taille du layer, et un ensemble de Cell
		std::string layerName(aLayer.attribute("name").value());
		if(layerName.compare("Field")==0)
		{
			fieldLayer = mapFactory_->createField(mapWidth, mapHeight, cellsMap);
		}
		else if(layerName.compare("Object")==0)
		{
			layersObjects.push_back(mapFactory_->createObjectLayers(mapWidth, mapHeight, cellsMap));
		}
	}
	Map::MAPORIENTATION mapOrient;
	if(orientation.compare("orthogonal")==0)
	{
		mapOrient = Map::MAPORIENTATION_ORTHOGONAL;
	}
	else
	{
		mapOrient = Map::MAPORIENTATION_ORTHOGONAL;
	}
    Map* theMap = mapFactory_->createMap(new Background(), fieldLayer, mapWidth, mapHeight, mapOrient);
	
	std::vector<ObjectLayers*>::iterator itm;
	for(itm = layersObjects.begin(); itm != layersObjects.end(); ++itm)
	{
		theMap->addObjectLayers((*itm));
	}

    theMap->addTileSets(ensTileSet);
	
//	std::cout << *theMap << std::endl;
	
	return theMap;
}
