#ifndef CLIENT_H
#define CLIENT_H

#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <netdb.h>
#include <string.h>
#include <stdbool.h>

#include <unistd.h>

#define TAILLEMAX 999

//Définition des types de structures qui peuvent être envoyées
#define STRUCTMESSAGE 0
#define STRUCTMOVING 1
#define STRUCTDOWN 2

typedef struct sockaddr sockaddr;
typedef struct sockaddr_in sockaddr_in;
typedef struct hostent hostent;
typedef struct servent servent;

typedef struct
{
    int type;
    char structure[TAILLEMAX];
} dispatchStruct;

typedef struct
{
    bool coherent; //Indique si les données (reçues/envoyées) sont coherentes (incohérentes si mauvaise commande)
    int dir; // Direction dans laquelle regarde le joueur 0 = nord, 2 = sud, 1 = est et 3 = ouest
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
//    int * scores; //Scores de tous les autres joueurs
    bool besideWumpus; //Flag indiquant que le joueur est à côté du wumpus
    bool besideHole; //Flag indiquant que le joueur est à côté d'un trou
    bool besideTresure; //Flag indiquant que le joueur est à côté du trésor
} fromServer;

void initialisationHost(hostent * ptr_host, char * host, sockaddr_in * adresse_locale); // Permet l'intitialisation du host

void attribuerPort(sockaddr_in * adresse_locale, int port); // Permet d'attribuer le port

int createSocket(); // Permet de créer la socket

/*void*/int connectionServeur(int socket_descriptor, sockaddr_in adresse_locale); // Permet de se connecter au serveur

void deconnexionServeur(int socket_descriptor); // Permet de se déconnecter du serveur

void writeFunction(int socket_descriptor, char* p); // Permet d'envoyer des informations au serveur

void readFunction(int socket_descriptor); // Permet de lire des informations venant du serveur

void envoiPseudoClient(char *p, int socket_descriptor); // Permet d'envoyer le pseudo au serveur

void envoiCommandClient(int socket_descriptor, char *command); // Permet d'envoyer une commande au serveur

void readData(int socket_descriptor, dispatchStruct* structure);

void dataProcessing(fromServer* server, dispatchStruct* dispStruc);

void receptionInfoClient(int socket_descriptor, fromServer *server, dispatchStruct *dispStruc);

/*void*/int connexionClient(int *socket_descriptor, hostent * ptr_host, char * host, sockaddr_in adresse_locale, int port); // Permet d'établir la connexion avec l'initialisation au serveur

void fromServerInitialisation(fromServer *receiv); // Initialisation de la structure de données

#endif // CLIENT_H
