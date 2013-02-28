#include "client.h"

Client::Client()
{
    //    if (argc !=2) {
    //        perror("Usage: client <adresse-serveur>");
    //        exit(1);
    //    }

    //    prog = argv[0];
    host = (char*)"127.0.0.1"/*argv[1]*/;
    //	command=argv[2];

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
    printf("Commande envoyée au serveur. \n");
    receptionInfo();
}

void Client::receptionInfo()
{
    /* Lecture des informations du jeu en provenance du serveur */
    if ((longueur = read(socket_descriptor, buffer, sizeof(buffer))) > 0) {
        printf("Réponse du serveur : \n");
        write(1,buffer,longueur);
    }
    printf("\nFin de la reception.\n");
}

void Client::connexion()
{
    // A voir ce qu'il faut mettre dedans
}
