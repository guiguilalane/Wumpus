#ifndef CLIENT_H
#define CLIENT_H

#include <QString>

#include <stdlib.h>
#include <QObject>

#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>

typedef struct sockaddr sockaddr;
typedef struct sockaddr_in sockaddr_in;
typedef struct hostent hostent;
typedef struct servent servent;

typedef struct
{
    bool coherent; //Indique si les données (reçues/envoyées) sont coherentes (incohérentes si mauvaise commande)
    char dir; // Direction dans laquelle regarde le joueur n = nord, s = sud, e=est et o=ouest
    int playerPosX; //Position du joueur sur la carte, -1 si mauvaise commande
    int playerPosY;
    int tresurePosX; //Position du trésor sur la carte, -1 si le joueur n'a pas trouver le trésor
    int tresurePosY;
    bool tresureFind; //Flag indiquant que le joueur vient de trouver le trésor
    bool fallInHole; //Flag indiquant que le joueur vient de tomber dans un trou
    bool wumpusFind; //Flag indiquant que le joueur à fait une malencontreuse rencontre avec le wumpus
    bool wumpusKill; //Flag indiquant que le joueur vient de tuer le wumpus
    int score; //Score du joueur, -1 si mauvaise commande
    // il faudrait une map
    int * scores; //Scores de tous les autres joueurs
    bool besideWumpus; //Flag indiquant que le joueur est à côté du wumpus
    bool besideHole; //Flag indiquant que le joueur est à côté d'un trou
    bool besideTresure; //Flag indiquant que le joueur est à côté du trésor
} fromServer;

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
    fromServer* server;
};

void fromServerInitialisation(fromServer *receiv);

#endif // CLIENT_H
