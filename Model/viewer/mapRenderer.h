//
//  mapRenderer.h
//
//
//  Created by Noémie RULLIER on 13/12/12.
//
//

#ifndef MAPRENDERER_H
#define MAPRENDERER_H

#include <QPainter>
#include <QImage>
#include <QGraphicsPixmapItem>
#include <QPixmap>
#include <QImageReader>

#include "../map/map.h"
#include "../map/cell.h"

class MapRenderer
{
    
public:
	
	/**
     * \brief Constructor of the mapRenderer
	 */
	MapRenderer(Map* map);
	
	/**
     * \brief Method that create the renderer of the map
	 */
	QGraphicsPixmapItem* createRendu();
	
	/**
     * \brief Method that update the renderer of the map
	 */
	QGraphicsPixmapItem* updateRendu(const int& x, const int& y);
	
private:
	
	Map* map_;
	QPainter *painter_; // Contiendra tous les QPixmap
	QImage *mapImage_;
	
};

#endif
