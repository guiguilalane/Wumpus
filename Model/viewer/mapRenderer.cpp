//
//  mapRenderer.cpp
//
//
//  Created by Noémie RULLIER on 13/12/12.
//
//

#include "mapRenderer.h"

MapRenderer::MapRenderer(Map* map): map_(map)
{
    std::vector<Layers*> layers = map->getAllLayers();
    int tileWidth = map_->getTileSets()[0]->getTileWidth();
    int tileHeight = map_->getTileSets()[0]->getTileHeight();
    int posTileX, posTileY;
    mapImage_ = new QImage(map->getTailleX()*tileWidth, map->getTailleY()*tileHeight, QImage::Format_ARGB32);
    painter_ = new QPainter(mapImage_);
    // Parcours des cell du layer de plus haut niveau, par position.
    // Recherche sur le calque de plus au niveau le tile* et créer le QPixmap pour chaque cellule
    std::vector<Layers*>::iterator rit;
    ensCells mapCells;
    ensCells::iterator it;
    Cell* c;
    Obstacle* o;
    for(rit = layers.begin(); rit != layers.end(); ++rit)
    {
        mapCells = (*rit)->getCellsTab();
        for(it = mapCells.begin(); it != mapCells.end(); ++it)
        {
            c = it->second;
            std::pair<int, int> position(c->getX(), c->getY());
            // Récupérer l'obstacle.
            o = c->getObstacle();
            if(o->getTileId()!=-1)
            {
                // Si l'id != -1 on récupère le Tile, sa position dans le tileSet, le tileSet
                Tile* tile = o->getTile();
                posTileX = tile->getTilePositionX();
                posTileY = tile->getTilePositionY();
                TileSet* tileOwner = tile->getTileSet();

                // On récupère le chemin de l'image
                QImageReader reader(QString("examples/Map/")+QString(tileOwner->getTileSetImage()->getSource()));

                // Découpage du tileSet pour récupérer la Tile
                reader.setClipRect(QRect(posTileX*tileWidth, posTileY*tileHeight, tileWidth, tileHeight));
                QImage image = reader.read();

                QPixmap pixmap = QPixmap::fromImage(image);
				
                painter_->drawTiledPixmap(position.first*tileWidth, position.second*tileHeight, tileWidth, tileHeight, pixmap);

            }
        }
    }
    // Painter est complet.
}

QGraphicsPixmapItem* MapRenderer::updateRendu(const int& x, const int& y)
{
	std::vector<Layers*> layers = map_->getAllLayers();
    int tileWidth = map_->getTileSets()[0]->getTileWidth();
    int tileHeight = map_->getTileSets()[0]->getTileHeight();
    int posTileX, posTileY;
	
    std::vector<Layers*>::iterator rit;
    Cell* c;
    Obstacle* o;
	std::pair<int, int> position(x,y);
	QPixmap pixmap;
	
	painter_->eraseRect(position.first*tileWidth, position.second*tileHeight, tileWidth, tileHeight);
    for(rit = layers.begin(); rit!=layers.end() ; ++rit)
    {
        c = (*rit)->findCellFromLayer(position);
        o = c->getObstacle();
        if(o->getTileId()!=-1)
        {
			// Si l'id != -1 on récupère le Tile, sa position dans le tileSet, le tileSet
			Tile* tile = o->getTile();
			posTileX = tile->getTilePositionX();
			posTileY = tile->getTilePositionY();
			TileSet* tileOwner = tile->getTileSet();
			
			// On récupère le chemin de l'image
            QImageReader reader(QString(tileOwner->getTileSetImage()->getSource()));
			
			// Découpage du tileSet pour récupérer la Tile
			reader.setClipRect(QRect(posTileX*tileWidth, posTileY*tileHeight, tileWidth, tileHeight));
			QImage image = reader.read();
			
			pixmap = QPixmap::fromImage(image);
			
			painter_->drawTiledPixmap(position.first*tileWidth, position.second*tileHeight, tileWidth, tileHeight, pixmap);
        }
    }
	
	QGraphicsPixmapItem* map = createRendu();
    return map;
}

QGraphicsPixmapItem* MapRenderer::createRendu(){
	// Creation of the QPixmap which contain the image of the entire map
	QPixmap mapPixmap = QPixmap::fromImage(*mapImage_);
	// Creation of the QGraphicsPixmapItem which contain the previous QPixmap
	QGraphicsPixmapItem* map = new QGraphicsPixmapItem(mapPixmap);
	
	return map;
}
