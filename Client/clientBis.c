#include "clientBis.h"

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
        perror("Erreur : impossible d'écrire le pseudo destine au serveur.");
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

void envoiPseudoClient(char *p, int socket_descriptor)
{
    writeFunction(socket_descriptor, p);
    //    readFunction(socket_descriptor);
    /* Ce qui suit sera à supprimer quand je saurais comment récupérer les infos */
    char buffer[256];
    int longueur;
    if ((longueur = read(socket_descriptor, buffer, sizeof(buffer))) > 0) {
        write(1,buffer,longueur);
    }
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
    char buffer[sizeof(dispatchStruct)];
    //    readFunction(socket_descriptor, structure);
    //    while((longueur = read(socket_descriptor, buffer, sizeof(dispatchStruct))) <= 0)
    //    {
    //        exit(1);
    //    }
    *structure = *((dispatchStruct*)&buffer);
}

/* Idem à revoir quand terminer surce qu'on recoit */
void receptionInfoClient(int socket_descriptor, fromServer * server)
{

    int longueur;
    /* Lecture des informations du jeu en provenance du serveur */
    //    readData(socket_descriptor, &test);
    //    printf("type : %d\n", test.type);
    //    fromServer* tmp = ((fromServer*) test.structure);
    //    // serveur = tmp ???????
    //    server = tmp;
    // Ancienne version a supprimer
    //    readFunction(socket_descriptor, server);
    if ((longueur = read(socket_descriptor, server, sizeof(fromServer))) > 0) {
        printf("Réponse du serveur : \n");
        printf("PlayerPosX : %d, playerPosY : %d\n", server->playerPosX, server->playerPosY);
    }
    printf("\nFin de la reception.\n");
}

void connexionClient(int * socket_descriptor, hostent * ptr_host, char * host, sockaddr_in adresse_locale)
{
    initialisationHost(ptr_host, host, &adresse_locale);
    attribuerPort(&adresse_locale, 5000/*port*/);
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
