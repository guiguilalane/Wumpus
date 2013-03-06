/* -----------------------------------------------
 Client a lancer après le serveur avec la commande:
 client <adresse-serveur> <message-a-transmettre>
 ------------------------------------------------*/

#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <netdb.h>
#include <string.h>
#include <stdbool.h>
#define h_addr h_addr_list[0] //TODO pour eviter : "erreur: ‘hostent’ has no member named ‘h_addr’"

#define TAILLEMAX 999

//définition des types de structures qui peuvent être envoyées
#define STRUCTMESSAGE 0
#define STRUCTMOVING 1

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
    bool coherent;//indique si les données (reçues/envoyées) sont coherentes (incohérentes si mauvaise commande)
    int direction;
    int playerPosX; //position du joueur sur la carte, -1 si mauvaise commande
    int playerPosY;
    int tresurePosX; //position du trésor sur la carte, -1 si le joueur n'a pas trouver le trésor
    int tresurPosY;
    bool tresureFinf; //flag indiquant que le joueur vient de trouver le trésor
    bool fallInHole; //flag indiquant que le joueur vient de tomber dans un trou
    bool wumpusFind; //flag indiquant que le joueur à fait une malencontreuse rencontre avec le wumpus
    bool wumpusKill; //flag indiquant que le joueur vient de tuer le wumpus
    int score;	//score du joueur, -1 si mauvaise commande
    bool besideWumpus; //flag indiquant que le joueur est à côté du wumpus
    bool besideHole; //flag indiquant que le joueur est à côté d'un trou
    bool besideTresure; //flag indiquant que le joueur est à côté du trésor
} fromServer;

void fromServerInitialisation(fromServer *receiv)
{
    receiv->coherent = false;
    receiv->direction = 0;
    receiv->playerPosX = -1;
    receiv->playerPosY = -1;
    receiv->tresurePosX = -1;
    receiv->tresurPosY = -1;
    receiv->besideHole = false;
    receiv->besideTresure = false;
    receiv->besideWumpus = false;
    receiv->score = -1;
    receiv->tresureFinf = false;
    receiv->fallInHole = false;
    receiv->wumpusFind = false;
    receiv->wumpusKill = false;
}

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
		
	case STRUCTMOVING:
		tmp = ((fromServer*) dispStruc->structure);
		server->coherent = tmp->coherent;
		server->direction = tmp->direction;
		server->playerPosX = tmp->playerPosX;
		server->playerPosY = tmp->playerPosY;
		server->tresurePosX = tmp->tresurePosX;
		server->tresurPosY = tmp->tresurPosY;
		server->tresureFinf = tmp->tresureFinf;
		server->fallInHole = tmp->fallInHole;
		server->wumpusFind = tmp->wumpusFind;
		server->wumpusKill = tmp->wumpusKill;
		server->score = tmp->score;
		server->besideWumpus = tmp->besideWumpus;
		server->besideHole = tmp->besideHole;
		server->besideTresure = tmp->besideTresure;
	
		printf("PlayerPosX : %d, playerPosY : %d\n", server->playerPosX, server->playerPosY);
		printf("besideTreasure : %d\n", server->besideTresure);
		printf("direction : %d\n", server->direction);
		printf("findTreasureS : %d\n", server->tresureFinf);
		printf("findTreasureT : %d\n", tmp->tresureFinf);
		break;
		
	default:
		printf("type de structure inconnue");
	}
}


int main(int argc, char **argv){
	
    char *defaulthost = "127.0.0.1";
	int socket_descriptor, /* Descripteur de socket */
		longueur; /* Longueur d'un buffer utilisé */
	sockaddr_in adresse_locale; /* Adresse de socket local */
	hostent * ptr_host; /* Info sur une machine hote */
	servent * ptr_service; /* Info sur un service */
	char buffer[256];
	char * prog; /* Nom du programme */
	char * host; /* Nom de la machine distante */
    uint16_t port = 5000;
	char command[15] /* Commande envoyée */, temp[15];
	
	fromServer server;
	fromServerInitialisation(&server);
	
	dispatchStruct dispStruc;
	
	if (argc < 2) {
        host = defaulthost;
//		perror("Usage: client <adresse-serveur>");
//		exit(1);
    }
    else
    {
        prog = argv[0];
        host = argv[1];
    }
	//	command=argv[2];
	
	if(argc == 3)
	{
		port = atoi(argv[2]);
	}
	
	if ((ptr_host = gethostbyname(host)) == NULL){
        perror("Erreur : impossible de trouver le serveur à partir de son adresse.");
		exit(1);
	}
	
	/* Copie caractère par caractère des infos de ptr_host vers adresse_locale */
	bcopy((char*)ptr_host->h_addr, (char*)&adresse_locale.sin_addr, ptr_host->h_length);
	adresse_locale.sin_family = AF_INET; /* ou ptr_host->h_addrtype */
	
	/* SOLUTION 1 : utiliser un service existant, par exemple "irc" */
	
	/*
	 if(ptr_service=getservbyname("irc","tcp")) == NULL) {
	 perror("Erreur : impossible de recuperer le numero de port du service desire.");
	 exit(1);
	 }
	 adresse_locale.sin_port = htons(ptr_service->s_port);
	 */
	
	/* SOLUTION 2 : utiliser un nouveau numéro de port */
	adresse_locale.sin_port = htons(port);
	
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
	
	printf("Connexion établie avec le serveur \n");
	printf("Numéro de port pour la connexion au serveur : %d \n", ntohs(adresse_locale.sin_port));
	
	/* Demande du pseudo du joueur */
	printf("Veuillez entrez votre pseudo \n");
	fgets(temp, sizeof(temp), stdin);	// Récupère la ligne entrée sur stdin('\n' y compris)
	sscanf (temp, "%[a-zA-Z]s", command);	// Filtrage de la chaine pour ne récupérer que les chaines de caractères (et enlever '\n')
	/* Envoi du pseudo vers le serveur */
	if ((write(socket_descriptor, command, strlen(command))) < 0) {
        perror("Erreur : impossible d'écrire le pseudo destine au serveur.");
		exit(1);
	}
	
	printf("debugg 1\n");
	readData(socket_descriptor, &dispStruc);
	dataProcessing(&server, &dispStruc);
/*	if ((longueur = read(socket_descriptor, server, sizeof(fromServer))) > 0) {*/
/*	printf("Réponse du serveur : \n");*/
/*			printf("PlayerPosX : %d, playerPosY : %d\n", server->playerPosX, server->playerPosY);*/
/*			printf("besideTreasure : %d\n", server->besideTresure);*/
/*//			write(1,buffer,longueur);*/
/*	}*/
	printf("debugg 2\n");
/*	sleep(3);*/
	
	/* Détection d'une commande ecrite par le joueur */
    do
    {
		printf("Attente commande \n");
		fgets(temp, sizeof(temp), stdin);	// Récupère la ligne entrée sur stdin('\n' y compris)
		sscanf (temp, "%[a-zA-Z]s", command);	// Filtrage de la chaine pour ne récupérer que les chaines de caractères (et enlever '\n')
        
		/* Envoi de la command vers le serveur */
		if ((write(socket_descriptor, command, strlen(command))) < 0) {
            perror("Erreur : impossible d'écrire le message destine au serveur.");
			exit(1);
		}
		
		/* Mise en attente du programme pour simuler un délai de transmission */
/*		sleep(3);*/
		printf("Commande envoyée au serveur. \n");
		
		//**********************************************************************
		
		//Début modification du code client pour la lecture d'une structure de données envoyée par le serveur
		
		
		readData(socket_descriptor, &dispStruc);
		dataProcessing(&server, &dispStruc);
		
		
		/* Lecture des informations du jeu en provenance du serveur */
/*		if ((longueur = read(socket_descriptor, server, sizeof(fromServer))) > 0) {*/
/*			printf("Réponse du serveur : \n");*/
/*			printf("PlayerPosX : %d, playerPosY : %d\n", server->playerPosX, server->playerPosY);*/
/*			printf("besideTreasure : %d\n", server->besideTresure);*/
/*/*			write(1,buffer,longueur);*/
/*		}*/
/*		if((longueur = read(socket_descriptor, test, )) > 0)*/
/*		{*/
/*			printf("type : %d\n", test->type);*/
/*		}*/
/*		printf("\nFin de la reception.\n");*/
		
		//Fin modification du code client pour la lecture d'une structure de données envoyée par le serveur
		
		//**********************************************************************
		
		// INFO: Si il y a un bug, permet de ne pas faire plusieurs tours de boucle(mais affichera un message)
		temp[0] = '\0';
    }
    while(strcmp(command, "quit"));
    
	/* Le joueur a quitter le jeu */
	close(socket_descriptor);
	
	printf("Connexion avec le serveur fermee, fin du programme. \n");
	return 0;
	
}
