#ifndef CLIENT_H
#define CLIENT_H

#include <QString>

#include <stdlib.h>
#include <QObject>

#include <stdio.h>
//#include <linux/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>

typedef struct sockaddr sockaddr;
typedef struct sockaddr_in sockaddr_in;
typedef struct hostent hostent;
typedef struct servent servent;

class Client
{
public:
    Client();
    void envoiPseudo(char* p);
    void envoiCommand(char* command);
    void receptionInfo();
    void connexion();

    int socket_descriptor, /* Descripteur de socket */
        longueur; /* Longueur d'un buffer utilisé */
    sockaddr_in adresse_locale; /* Adresse de socket local */
    hostent * ptr_host; /* Info sur une machine hote */
    servent * ptr_service; /* Info sur un service */
    char buffer[256];
    char * prog; /* Nom du programme */
    char * host; /* Nom de la machine distante */
    char command[15] /* Commande envoyée */, temp[15];
    bool connect_;
};

#endif // CLIENT_H
