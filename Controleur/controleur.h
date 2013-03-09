#ifndef CONTROLEUR_H
#define CONTROLEUR_H

#include <iostream>
#include <pthread.h>

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
    int connexion();
    void envoiValeurConnexion(const char *ad, int p);
    void getValeur(QString& ad, QString &p);
    bool getConnect_();

    QString getAdresse();
    QString getPort();

    fromServer server;

signals:
    void infoRecu(fromServer * s, dispatchStruct * d);
    void initMap(fromServer * s);
    void clearMap();

private:

    static void *ecouter(void * arg);

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
    pthread_t listener;
};

#endif // CONTROLEUR_H
