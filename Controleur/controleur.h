#ifndef CONTROLEUR_H
#define CONTROLEUR_H

#include "Client/client.h"

class Controleur
{
public:
    Controleur();
    ~Controleur();

    void envoiPseudo(QString * p);
    void envoiCommand(char* command);
    void connexion();

private:
    Client * client;
};

#endif // CONTROLEUR_H
