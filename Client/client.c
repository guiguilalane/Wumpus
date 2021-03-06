#include "client.h"

void aquitementRead(int socket_descriptor)
{
    char * aquit = "Acquitement";
    write(socket_descriptor, aquit, strlen(aquit));
}

int initialisationHost(hostent * ptr_host, char * host, sockaddr_in * adresse_locale)
{
    int res = 0;
    /* Connexion au serveur */
    if ((ptr_host = gethostbyname(host)) == NULL){
        res = -1;
    }
    else{
        /* Copie caractère par caractère des infos de ptr_host vers adresse_locale */
        bcopy((char*)ptr_host->h_addr, (char*)&adresse_locale->sin_addr, ptr_host->h_length);
        adresse_locale->sin_family = AF_INET; /* ou ptr_host->h_addrtype */
    }
    return res;
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

int connectionServeur(int socket_descriptor, sockaddr_in adresse_locale)
{
    /* Tentative de connexion au serveur dont les infos sont dans adresse_locale */
    return connect(socket_descriptor, (sockaddr*)(&adresse_locale), sizeof(adresse_locale));
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
//    aquitementWrite(socket_descriptor);
}

void envoiPseudoClient(char *p, int socket_descriptor)
{
    writeFunction(socket_descriptor, p);
}

void envoiCommandClient(int socket_descriptor, char *command)
{
    /* Envoi de la command vers le serveur */
    writeFunction(socket_descriptor, command);
}

void readData(int socket_descriptor, dispatchStruct* structure)
{
    int len = 0;
        while(!len && ioctl(socket_descriptor, FIONREAD, &len) >= 0)
        {
            usleep(500);
        }
        char buffer[len];
        int longueur;
        while((longueur = read(socket_descriptor, buffer, len)) <= 0)
        {
            exit(1);
        }
        *structure = *((dispatchStruct*)&buffer);
}

void dataProcessing(fromServer* server, scoreToClient *score, dispatchStruct* dispStruc)
{
    fromServer* tmp;
    scoreToClient* tmpS;
    switch(dispStruc->type)
    {
    case STRUCTMESSAGE:
        printf("%s\n", dispStruc->structure);
        break;

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
        break;

    case STRUCTDOWN:
        tmpS = ((scoreToClient*) dispStruc->structure);
        memcpy(score->scores, tmpS->scores, TAILLEMAX);
        score->nbScore = tmpS->nbScore;
        printf("%s \t %d\n", score->scores[0].pseudoP, score->scores[0].score);
        printf("%d\n", tmpS->nbScore);
        break;

    case STRUCTACQUITEMENT:
        printf("%s\n", dispStruc->structure);
        break;

    default:
        printf("type de structure inconnue");
    }
}

void receptionInfoClient(int socket_descriptor, fromServer * server, scoreToClient *score, dispatchStruct * dispStruc)
{
    /* Lecture des informations du jeu en provenance du serveur */
    readData(socket_descriptor, dispStruc);
    if(dispStruc->type > STRUCTACQUITEMENT)
    {
        aquitementRead(socket_descriptor);
    }
    dataProcessing(server, score, dispStruc);
    //aquitement d'un read
    printf("Fin de la reception.\n");
}

int connexionClient(int * socket_descriptor, hostent * ptr_host, char * host, sockaddr_in adresse_locale, int port)
{
    int add;
    int con;
    add = initialisationHost(ptr_host, host, &adresse_locale);
    attribuerPort(&adresse_locale, port);
    *socket_descriptor = createSocket();
    con = connectionServeur(*socket_descriptor, adresse_locale);
    if (con == -1 || add == -1)
    {
        return -1;
    }
    else {
        return 0;
    }
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
    receiv->score = 0;
    // TODO Initialiser la tableau avec des 0 ?
    //receiv->scores = (int *) malloc(NBJOUEUR-1xsizeof(int)); // A t'on acces à NB-JOUEUR, il faudrait auusi récupérer leur pseudo ?
    receiv->tresureFind = false;
    receiv->fallInHole = false;
    receiv->wumpusFind = false;
    receiv->wumpusKill = false;
}
