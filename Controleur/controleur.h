#ifndef CONTROLEUR_H
#define CONTROLEUR_H

#include <iostream>

#include <QString>
#include <QObject>

#ifdef __cplusplus
extern "C" {
#include "Client/client.h"
}
#endif

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

    fromServer* server;

signals:
    void infoRecu(fromServer * s);

private:
    int socket_descriptor; /* Descripteur de socket */
    sockaddr_in adresse_locale; /* Adresse de socket local */
    hostent * ptr_host; /* Info sur une machine hote */
    servent * ptr_service; /* Info sur un service */
    char * prog; /* Nom du programme */
    char * host; /* Nom de la machine distante */
    int port; /* Port de connexion */
    char command[15] /* Commande envoyée */;
    bool connect_;
    dispatchStruct dispStruc;
};

#endif // CONTROLEUR_H
