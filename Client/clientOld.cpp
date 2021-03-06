#include "client.h"

Client::Client()
{
    server = (fromServer*) malloc(sizeof(fromServer));
    fromServerInitialisation(server);

    //    if (argc !=2) {
    //        perror("Usage: client <adresse-serveur>");
    //        exit(1);
    //    }

    //    prog = argv[0];
    host = new char[strlen("127.0.0.1")+1];
    strcpy(host,"127.0.0.1");
    port = 5000;
    //	command=argv[2];

    //    if(argc == 3)
    //	{
    //		port = atoi(argv[2]);
    //	}

    //    if ((ptr_host = gethostbyname(host)) == NULL){
    //        perror("Erreur : impossible de trouver le serveur à partir de son adresse.");
    //        exit(1);
    //    }

    //    /* Copie caractère par caractère des infos de ptr_host vers adresse_locale */
    //    bcopy((char*)ptr_host->h_addr, (char*)&adresse_locale.sin_addr, ptr_host->h_length);
    //    adresse_locale.sin_family = AF_INET; /* ou ptr_host->h_addrtype */

    //    adresse_locale.sin_port = htons(5000);

    //    /* Création de la socket */
    //    if ((socket_descriptor = socket(AF_INET, SOCK_STREAM, 0)) < 0){
    //        perror("Erreur : impossible de créer la socket de connexion avec le serveur");
    //        exit(1);
    //    }

    //    /* Tentative de connexion au serveur dont les infos sont dans adresse_locale */
    //    if ((connect(socket_descriptor, (sockaddr*)(&adresse_locale), sizeof(adresse_locale))) < 0){
    //        perror("Erreur : impossible de se connecter au serveur");
    //        exit(1);
    //    }
    //    connect_ = true;
    //    printf("Connexion établie avec le serveur \n");
    //    printf("Numéro de port pour la connexion au serveur : %d \n", ntohs(adresse_locale.sin_port));
}

void Client::envoiPseudo(char *p)
{
    if ((write(socket_descriptor, p, strlen(p))) < 0) {
        perror("Erreur : impossible d'écrire le pseudo destine au serveur.");
        exit(1);
    }
    if ((longueur = read(socket_descriptor, buffer, sizeof(buffer))) > 0) {
        write(1,buffer,longueur);
    }
}

void Client::envoiCommand(char *command)
{

    /* Envoi de la command vers le serveur */
    if ((write(socket_descriptor, command, strlen(command))) < 0) {
        perror("Erreur : impossible d'écrire le message destine au serveur.");
        exit(1);
    }
    if (command == (char*)"quit"){
        /* Le joueur a quitter le jeu */
        close(socket_descriptor);
        connect_ = false;
        printf("Connexion avec le serveur fermee, fin du programme. \n");
    }

    /* Mise en attente du programme pour simuler un délai de transmission */
    /*sleep(3);*/
    //    printf("Commande envoyée au serveur. \n");
}

void readData(int socket_descriptor, dispatchStruct* structure)
{
    char buffer[sizeof(dispatchStruct)];
    int longueur;

    while((longueur = read(socket_descriptor, buffer, sizeof(dispatchStruct))) <= 0)
    {
        exit(1);
    }
    *structure = *((dispatchStruct*)&buffer);
}

void Client::receptionInfo()
{

    /* Lecture des informations du jeu en provenance du serveur */
//    readData(socket_descriptor, &test);
//    printf("type : %d\n", test.type);
//    fromServer* tmp = ((fromServer*) test.structure);
//    // serveur = tmp ???????
//    server = tmp;
    // Ancienne version a supprimer
        if ((longueur = read(socket_descriptor, server, sizeof(fromServer))) > 0) {
            printf("Réponse du serveur : \n");
                    printf("PlayerPosX : %d, playerPosY : %d\n", server->playerPosX, server->playerPosY);
    //                printf("besideTreasure : %d\n", server->besideTresure);
                }
    printf("\nFin de la reception.\n");
}

void Client::connexion()
{
    fromServerInitialisation(server);
    // TODO A revoir si on met bien tout ça dedans
    // Pour la première connexion OK

    /* Connexion au serveur */
    if ((ptr_host = gethostbyname(host)) == NULL){
        perror("Erreur : impossible de trouver le serveur à partir de son adresse.");
        exit(1);
    }

    /* Copie caractère par caractère des infos de ptr_host vers adresse_locale */
    bcopy((char*)ptr_host->h_addr, (char*)&adresse_locale.sin_addr, ptr_host->h_length);
    adresse_locale.sin_family = AF_INET; /* ou ptr_host->h_addrtype */

    adresse_locale.sin_port = htons(5000);

    /* Création de la socket */
    if ((socket_descriptor = socket(AF_INET, SOCK_STREAM, 0)) < 0){
        perror("Erreur : impossible de créer la socket de connexion avec le serveur");
        exit(1);
    }

    /* Tentative de connexion au serveur dont les infos sont dans adresse_locale */
    if ((connect(socket_descriptor, (sockaddr*)(&adresse_locale), sizeof(adresse_locale))) < 0){
        perror("Erreur : impossible de se connecter au serveur");
        exit(1);
    }
    connect_ = true;
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
