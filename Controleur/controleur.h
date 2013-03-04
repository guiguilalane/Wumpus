#ifndef CONTROLEUR_H
#define CONTROLEUR_H

#include "Client/client.h"

#include <iostream>

class Controleur : public QObject
{
    Q_OBJECT

public:
    Controleur();
    ~Controleur();

    void envoiPseudo(QString * p);
    void envoiCommand(char* command);
    void connexion();
    void envoiValeurConnexion(const char *ad, int p);
    void getValeur(QString& ad, QString &p);

    QString getAdresse();
    QString getPort();

signals:
    void infoRecu(fromServer * s);

private:
    Client * client;
};

#endif // CONTROLEUR_H
