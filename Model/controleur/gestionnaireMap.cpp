#include "gestionnaireMap.h"

GestionnaireMap* GestionnaireMap::gestionnaire_ = NULL;

GestionnaireMap* GestionnaireMap::getInstance(char* tmxpath)
{
	if(NULL == gestionnaire_)
	{
		gestionnaire_ = new GestionnaireMap(tmxpath);
	}
	return gestionnaire_;
}

void GestionnaireMap::kill()
{
	if(NULL != gestionnaire_)
	{
		delete gestionnaire_;
		gestionnaire_ = NULL;
	}
}

GestionnaireMap::GestionnaireMap(char* tmxpath)
{
	parseurTMX_ = new Parseur(tmxpath);
	map_ = parseurTMX_->doParsing();
//	std::cout << *map_ << std::endl;
}

GestionnaireMap::~GestionnaireMap()
{
	delete parseurTMX_;
	delete map_;
}

Map* GestionnaireMap::getMap()
{
	return map_;
}

bool GestionnaireMap::proximitySensor(const int& x, const int& y)
{
	bool obstacle = true;
	std::pair<int,int> p(x,y);
	// Parcour of the field layer
	Cell* c = map_->getField()->findCellFromLayer(p);
	if(c != NULL){
			if (c->getObstacle()->getType() == "field"){
				obstacle = false;
			}
	}
	
	// Parcour of the object layer
    for (int i = 0; i<map_->getObjectLayers().size();i++)
    {
		c = map_->getObjectLayers()[i]->findCellFromLayer(p);
		if(c != NULL){
			if (c->getObstacle()->getType() == "field"){
				obstacle = false;
			}
		}
	}
	
	return obstacle;
}

std::vector<std::string> GestionnaireMap::cameraSensor(const int& x, const int& y)
{
	std::vector<std::string> value;
	std::pair<int,int> p(x,y);
	Cell* c;
	std::map<std::string,std::string>::iterator itPropriete;
	for (int i = 0; i<map_->getObjectLayers().size();i++){
	c = map_->getObjectLayers()[i]->findCellFromLayer(p);
        itPropriete = c->getObstacle()->getPropriete().find("name");
        if (itPropriete != c->getObstacle()->getPropriete().end())
        {
            value.push_back(itPropriete->second);
        }
	}
	return value;
}


std::string GestionnaireMap::getFieldValue(const int& x, const int& y)
{
	std::string value="";
	std::pair<int,int> p(x,y);
	Cell* c;
	std::map<std::string,std::string>::iterator itPropriete;
	c = map_->getField()->findCellFromLayer(p);
            if (c->getObstacle()->getType() == "fixed")
            {
				value = "fixed";
			}
            else if (c->getObstacle()->getType() == "field")
            {
				//if (it->second->getTile()->getPropriete() != NULL){
				itPropriete = c->getObstacle()->getPropriete().find("name");
                if (itPropriete != c->getObstacle()->getPropriete().end())
                {
					value = itPropriete->second;
				}
				//}
			}
	return value;
}

void GestionnaireMap::deleteObject(const int& x, const int& y)
{
    bool deleteO = false;
    std::pair<int,int> p(x,y);
    Cell* c;
    int i = map_->getObjectLayers().size() - 1;
    while (i >= 0 && deleteO == false){
        c = map_->getObjectLayers()[i]->findCellFromLayer(p);
        if(*(c->getObstacle()) != Obstacle())
        {
            delete c->getObstacle();
            c->setObstacle(new Obstacle());
            deleteO = true;
        }
        i--;
    }
}
