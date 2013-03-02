#ifndef CONTROLEUR_H
#define CONTROLEUR_H

#include "Client/client.h"

class Controleur : public QObject
{
    Q_OBJECT

public:
    Controleur();
    ~Controleur();

    void envoiPseudo(QString * p);
    void envoiCommand(char* command);
    void connexion();

signals:
    void infoRecu();

private:
    Client * client;
};

#endif // CONTROLEUR_H
