#include "client.h"

void initialisationHost(hostent * ptr_host, char * host, sockaddr_in * adresse_locale)
{
    /* Connexion au serveur */
    if ((ptr_host = gethostbyname(host)) == NULL){
        perror("Erreur : impossible de trouver le serveur à partir de son adresse.");
        exit(1);
    }
    /* Copie caractère par caractère des infos de ptr_host vers adresse_locale */
    bcopy((char*)ptr_host->h_addr, (char*)&adresse_locale->sin_addr, ptr_host->h_length);
    adresse_locale->sin_family = AF_INET; /* ou ptr_host->h_addrtype */
}

void attribuerPort(sockaddr_in * adresse_locale, int port)
{
    adresse_locale->sin_port = htons(port);
}

int createSocket()
{
    int socket_descriptor;
    /* Création de la socket */
    if ((socket_descriptor = socket(AF_INET, SOCK_STREAM, 0)) < 0){
        perror("Erreur : impossible de créer la socket de connexion avec le serveur");
        exit(1);
    }
    return socket_descriptor;
}

void connectionServeur(int socket_descriptor, sockaddr_in adresse_locale)
{
    /* Tentative de connexion au serveur dont les infos sont dans adresse_locale */
    if ((connect(socket_descriptor, (sockaddr*)(&adresse_locale), sizeof(adresse_locale))) < 0){
        perror("Erreur : impossible de se connecter au serveur");
        exit(1);
    }
}

void deconnexionServeur(int socket_descriptor)
{
    close(socket_descriptor);
}

void writeFunction(int socket_descriptor, char* p)
{
    if ((write(socket_descriptor, p, strlen(p))) < 0) {
        perror("Erreur : impossible d'envoyer au serveur.");
        exit(1);
    }
}

void readFunction(int socket_descriptor)
{
    char buffer[256];
    int longueur;
    if ((longueur = read(socket_descriptor, buffer, sizeof(buffer))) > 0) {
        write(1,buffer,longueur);
    }
}

void envoiPseudoClient(char *p, int socket_descriptor, fromServer server, dispatchStruct dispStruct)
{
    writeFunction(socket_descriptor, p);
    //    readFunction(socket_descriptor);
    /* Ce qui suit sera à supprimer quand je saurais comment récupérer les infos */
//    char buffer[256];
//    int longueur;
//    if ((longueur = read(socket_descriptor, buffer, sizeof(buffer))) > 0) {
//        write(1,buffer,longueur);
//    }
    receptionInfoClient(socket_descriptor, &server, dispStruct);
}

void envoiCommandClient(int socket_descriptor, char *command)
{
    /* Envoi de la command vers le serveur */
    writeFunction(socket_descriptor, command);
    printf("Commande envoyée au serveur. \n");
}

/* A revoir quand on connaitra la structure à recevoir */
void readData(int socket_descriptor, dispatchStruct* structure)
{
    int len = 0;
        while(!len && ioctl(socket_descriptor, FIONREAD, &len) >= 0)
        {
            usleep(500);
        }
    /*	char buffer[sizeof(dispatchStruct)];*/
        char buffer[len];
        int longueur;

        while((longueur = read(socket_descriptor, buffer, len)) <= 0)
        {
            exit(1);
        }
        *structure = *((dispatchStruct*)&buffer);
}

void dataProcessing(fromServer* server, dispatchStruct* dispStruc)
{
    printf("type : %d\n", dispStruc->type);
    fromServer* tmp;
    switch(dispStruc->type)
    {
    case STRUCTMESSAGE:
        printf("%s", dispStruc->structure);
        break;

    // TODO Vérifier si le serveur renvoi bien les bonnes valeurs
    case STRUCTMOVING:
        tmp = ((fromServer*) dispStruc->structure);
        server->coherent = tmp->coherent;
        server->dir = tmp->dir;
        server->playerPosX = tmp->playerPosX;
        server->playerPosY = tmp->playerPosY;
        server->tresurePosX = tmp->tresurePosX;
        server->tresurePosY = tmp->tresurePosY;
        server->tresureFind = tmp->tresureFind;
        server->fallInHole = tmp->fallInHole;
        server->wumpusFind = tmp->wumpusFind;
        server->wumpusKill = tmp->wumpusKill;
        server->score = tmp->score;
        server->besideWumpus = tmp->besideWumpus;
        server->besideHole = tmp->besideHole;
        server->besideTresure = tmp->besideTresure;
        printf("PlayerPosX : %d, playerPosY : %d\n", server->playerPosX, server->playerPosY);
        printf("besideTreasure : %d\n", server->besideTresure);
        printf("findTreasure : %d\n", tmp->tresureFind);
        break;

    default:
        printf("type de structure inconnue");
    }
}

/* Idem à revoir quand terminer sur ce qu'on recoit */
void receptionInfoClient(int socket_descriptor, fromServer * server, dispatchStruct dispStruc)
{
    /* Lecture des informations du jeu en provenance du serveur */
    readData(socket_descriptor, &dispStruc);
    dataProcessing(server, &dispStruc);

//    int longueur;
//    //    readData(socket_descriptor, &test);
//    //    printf("type : %d\n", test.type);
//    //    fromServer* tmp = ((fromServer*) test.structure);
//    //    // serveur = tmp ???????
//    //    server = tmp;
//    // Ancienne version a supprimer
//    //    readFunction(socket_descriptor, server);
//    if ((longueur = read(socket_descriptor, server, sizeof(fromServer))) > 0) {
//        printf("Réponse du serveur : \n");
//    }
    printf("\nFin de la reception.\n");
}

void connexionClient(int * socket_descriptor, hostent * ptr_host, char * host, sockaddr_in adresse_locale, int port)
{
    initialisationHost(ptr_host, host, &adresse_locale);
    attribuerPort(&adresse_locale, port);
    *socket_descriptor = createSocket();
    connectionServeur(*socket_descriptor, adresse_locale);
    printf("Connexion établie avec le serveur \n");
    printf("Numéro de port pour la connexion au serveur : %d \n", ntohs(adresse_locale.sin_port));
}

void fromServerInitialisation(fromServer *receiv)
{
    receiv->coherent = false;
    receiv->dir = 0;
    receiv->playerPosX = 0;
    receiv->playerPosY = 4;
    receiv->tresurePosX = -1;
    receiv->tresurePosY = -1;
    receiv->besideHole = false;
    receiv->besideTresure = false;
    receiv->besideWumpus = false;
    receiv->score = -1; // TODO Initialisatio à 0 ?
    // TODO Initialiser la tableau avec des -1 ou 0 ?
    //receiv->scores = (int *) malloc(NBJOUEUR-1xsizeof(int)); // A t'on acces à NB-JOUEUR, il faudrait auusi récupérer leur pseudo ?
    receiv->tresureFind = false;
    receiv->fallInHole = false;
    receiv->wumpusFind = false;
    receiv->wumpusKill = false;
}
