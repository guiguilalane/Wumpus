#include "serveur.h"

#include <linux/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <netdb.h>
#include <pthread.h>
#define TAILLE_MAX_NOM 256
#define h_addr h_addr_list[0] //TODO pour eviter : "erreur: ‘hostent’ has no member named ‘h_addr’"

#define NBPLAYERSPERGAME 2

int vardebug;

typedef struct
{
    bool coherent;//indique si les données (reçues/envoyées) sont coherentes
    int playerPosX;
    int playerPosY;
    int tresurePosX;
    int tresurPosY;
    bool tresureFinf;
    bool fallInHole;
    bool wumpusFind;
    bool wumpusKill;
    int score;
    bool besideWumpus;
    bool besideHole;
    bool besideTresure;
} toClient;

typedef struct
{
	void* sock;
	player* p;
	int gameNumber;
} arg_struct;

typedef struct{

	stairs *s;
	player** p;
	int nbPlayerInGame;
	
} jeu;

jeu* jeux;

typedef struct sockaddr sockaddr;
typedef struct sockaddr_in sockaddr_in;
typedef struct hostent hostent;
typedef struct servent servent;

/*// Joueur*/
/*player* p;*/
// Etage

// Actions disponibles
Action* playerActions;

// Lorsque le joueur veux quitter la partie
void quit(player* p)
{
    printf("Terminer la partie et afficher le score\n");
}

// Lorsque le personnage avance
void move(player* p)
{
	switch(p->direction)
	{
		case NORTH :
			if(p->posY > 0){
				p->posY--;
			}
			break;
			
		case EAST :
			if(p->posX < STAIRSIZE - 1){
				p->posX++;
			}
			break;
			
		case SOUTH :
			if(p->posY < STAIRSIZE - 1){
				p->posY++;
			}
			break;
			
		case WEST :
			if(p->posX > 0){
				p->posX--;
			}
			break;
	}
	/*    printf("Avancer d'une case dans la direction pointée.\n");*/
}

// Lorsque le personnage tourne à droite
void turn_right(player* p)
{
	p->direction = (p->direction+1)%4;
	printf("Le personnage change de direction dans le sens horaire.\n");
}

// Lorsque le personnage tourne à gauche
void turn_left(player* p)
{
	p->direction = (p->direction+3)%4;//tourne 3 fois dans le sens horaire
	printf("Le personnage change de direction dans le sens anti-horaire.\n");
}

// Lorsque le peronnage tire une flèche
void shot()
{
	printf("Le personnage tire une flèche.\n");
}

// Lorsque le personnage descend l'échelle
void down()
{
	printf("Le personnage descend d'un étage.\n");
}

// Recherche l'action qui correspond à la commande
/*
 param action : ensemble des actions disponibles dans le jeu
 return l'action désignée par command
 */
Action* findActionFromCommand(Action* action, char* command)
{
    Action* a = NULL;
    int i = 0;
    bool find = false;
    
    while(i < NBACTION && !find)
    {
    	if(strcmp(action[i].command, command)==0)
        {
            a = &action[i];
            find = true;
        }
        i++;
    }
    return a;
}

// Initialiser les actions disponibles
Action* initialisationActions()
{
    Action* actions = (Action*)malloc(NBACTION*sizeof(Action));
    
    //Action quitter
    Action* quitAction = (Action*)malloc(sizeof(Action));
    quitAction->command = "quit";
    quitAction->action = quit;
    actions[A_QUITTER] = *quitAction;
    
    //Action avancer
    Action* moveAction = (Action*)malloc(sizeof(Action));
    moveAction->command = "move";
    moveAction->action = move;
    actions[A_AVANCER] = *moveAction;
    
    //Action tourner à droite
    Action* tRightAction = (Action*)malloc(sizeof(Action));
    tRightAction->command = "turnright";
    tRightAction->action = turn_right;
    actions[A_TOURNERDROITE] = *tRightAction;
    
    //Action tourner à gauche
    Action* tLeftAction = (Action*)malloc(sizeof(Action));
    tLeftAction->command = "turnleft";
    tLeftAction->action = turn_left;
    actions[A_TOURNERGAUCHE] = *tLeftAction;
    
    //Action tirer une fleche
    Action* shotAction = (Action*)malloc(sizeof(Action));
    shotAction->command = "shot";
    shotAction->action = shot;
    actions[A_TIRER] = *shotAction;
    
    //Action descendre
    Action* downAction = (Action*)malloc(sizeof(Action));
    downAction->command = "down";
    downAction->action = down;
    actions[A_DESCENDRE] = *downAction;
    
    return actions;
}

// Initialise le joueur à chaque début de niveau
void playerInitialisation(player* p)
{
	p->pseudo = "";
	p->score = 0;
	p->posX = 0;
	p->posY = (STAIRSIZE - 1);
	p->direction = NORTH;
	p->arrow = true;
    p->deadByWumpus = false;
    p->fallInHole = false;
    p->findTresure = false;
    p->shotTheWumpus = false;
}

// Initialise l'étage
void stairInitialisation(stairs *s)
{
	/* Définition des coordonnées des objets */
	
	// Contiendra les coordonnées de l'échelles, du trou, du wumpus et du trésor.
	int objects[4][2] = { //{y, x}
		{STAIRSIZE - 1, 0},// Echelle
		{-1, -1},// Trou
		{-1, -1},// Wumpus
		{-1, -1}// Trésor
	};
	int x, y;
	srand(time(NULL));
	int k = 1; // Represente tours après tours : trou, wumpus, trésor
	int i = 0, j = 0;
	bool used = false;// Coordonnées déja utilisées ou non
	while(k <= 3)
	{
		// Définition aléatoire des coordonnées du prochain objet à placer
		x = (rand()%STAIRSIZE);
		y = (rand()%STAIRSIZE);
		used = false;
		i = 0;
		// Verification que les coordonnées ne sont pas déjà utilisée
		while(!used && i < 4)
		{
			used = used || ((objects[i][0] == x) && (objects[i][1] == y));
			++i;
		}
		if(!used)
		{// Si non utiliser on stock les coordonnées dans une variables objects
			objects[k][0] = x;
			objects[k][1] = y;
			++k;
		}
	}
	
	/* Mise en place des objets sur la map */
	s->wumpusAlive = true;
	s->tresureFounded = false;
	for(i = 0 ; i < STAIRSIZE ; ++i)
	{
		j = 0;
		for(j; j < STAIRSIZE; ++j)
		{
			
			s->map[i][j] = ' ';
		}
	}
	
	// Lecture de la variables objects et placement des objects
	for(k = 0; k < 4 ; ++k)
	{
		x = objects[k][0];
		y = objects[k][1];
		switch(k)
		{
			case 0:
				s->map[x][y] = 'E';// Echelle
				break;
				
			case 1:
				s->map[x][y] = 'H';// Trou(hole)
				break;
				
			case 2:
				s->map[x][y] = 'W';// Wumpus
				break;
				
			case 3:
				s->map[x][y] = 'T';// Tresor
				break;
		}
	}
}

//Senseur de détection de l'objet
bool sensor(player* p, stairs* s, char o)
{
    //Coordonnées de la case sur laquelle on test la présence du wumpus
    int testX = p->posX-1;
    int testY = p->posY-1;
/*	printf("première coordonnées : (%d, %d)\n", testX, testY);*/
    //Coordonnées de la dernière case à tester
    int xFin = p->posX+1;
    int yFin = p->posY+1;
/*	printf("derniere coordonnées : (%d, %d)\n", xFin, yFin);*/
    bool find = false;
    while(!find && testY <= yFin)//lorsque la 3ième ligne à été testée on sort de la boucle
    {
        if(testX < STAIRSIZE && testX >= 0 && testY < STAIRSIZE && testY >= 0)
        {
            find = (s->map[testY][testX] == o);
        }
/*		printf("(%d, %d)\n", testX, testY);*/
        testX++;
        if(testX > xFin){
            testX = xFin - 2;
            testY++;
        }
    }
    return find;
}

void toClientInitialisation(toClient *toSend)
{
    toSend->coherent = false;
    toSend->playerPosX = -1;
    toSend->playerPosY = -1;
    toSend->tresurePosX = -1;
    toSend->tresurPosY = -1;
    toSend->besideHole = false;
    toSend->besideTresure = false;
    toSend->besideWumpus = false;
    toSend->score = -1;
    toSend->tresureFinf = false;
    toSend->fallInHole = false;
    toSend->wumpusFind = false;
    toSend->wumpusKill = false;
}

/***************************************TO FINISH***************************************/

void initSending(toClient* c, player* p, stairs* s, int tresurePos[2])
{
    c->coherent = true;
    c->playerPosX = p->posX;
    c->playerPosY = p->posY;
    c->tresurePosX = tresurePos[0];
    c->tresurPosY = tresurePos[1];
    c->besideHole = sensor(p, s, 'H');
    c->besideTresure = sensor(p, s, 'T');
    c->besideWumpus = sensor(p, s, 'W');
    c->score = p->score;
    c->tresureFinf= p->deadByWumpus;
    c->fallInHole = p->fallInHole;
    c->wumpusFind = p->findTresure;
    c->wumpusKill = p->shotTheWumpus;
}

// Fonction temporaire
void getDirection(int d, char* direction)
{
	switch(d)
	{
		case NORTH :
			strcpy(direction, "du Nord");
			break;
			
		case EAST :
			strcpy(direction, "de l'Est");
			break;
			
		case SOUTH :
			strcpy(direction, "du Sud");
			break;
			
		case WEST :
			strcpy(direction, "de l'Ouest");
			break;
	}
}

//Peut provoquer un bufferOverflow si la taille de la map excède 999*999
void printPlayerStatus(player* p, stairs* s, char* temp)
{
	char direction[11];
	getDirection(p->direction, direction);
	
	char* t = (char*) malloc(strlen("Le joueur est à la position : (xxx, xxx)\nregarde en direction  et  sa flèche.\n") + 11 + strlen("ne possède plus"));
	
	sprintf(t, "Le joueur est à la position : (%d, %d)\nregarde en direction %s et %s sa flèche.\n",
		   p->posX, p->posY, direction, p->arrow ? "possède encore" : "ne possède plus");
	strcat(temp, t);
	if(sensor(p, s,'W'))
	{
		strcat(temp, "Pouark! Ça pue!!\n");
	}
	if(sensor(p, s, 'T'))
	{
		strcat(temp, "Ça sonne comme des pièces d'or à mes oreilles.\n");
	}
	if(sensor(p, s, 'H'))
	{ 
		strcat(temp, "PFFFFF! Tiens un courant d'air!\n");
	}
	
}

// Affiche l'étage sans les informations sur la position des éléments (trésor, wumpus, trou)
/*
 (0,0)___________
	  |0|1|2|3|4|
	  |5|6|7|8|9|
	  |10|11|12|13|14|
	  |15|16|17|18|19|
 (4,0)|20|21|22|23|24|(4,4)
	  ___________
 */
void serveurPrintStairs(player* p, stairs *s)
{
	printf("___________\n");
	int i = 0, j = 0;
	for(i ; i < STAIRSIZE ; ++i)
	{
		j = 0;
		printf("|");
		for(j; j < STAIRSIZE; ++j)
		{		//TODO : changer l'affichage et mettre les fleche selon la direction regardée par le joueur
			printf("%c|", (j == p->posX && i == p->posY) ? arrows[p->direction] : s->map[i][j]);
		}
		printf("\n");
	}
	printf("___________\n");
}


char* clientPrintStairs(player* p, stairs *s, char* temp)
{
	strcat(temp, "___________\n");
/*	printf("___________\n");*/
	int i = 0, j = 0;
	for(i ; i < STAIRSIZE ; ++i)
	{
		j = 0;
		strcat(temp, "|");
/*		printf("|");*/
		for(j; j < STAIRSIZE; ++j)
		{		//TODO : changer l'affichage et mettre les fleche selon la direction regardée par le joueur
			if((j == p->posX && i == p->posY))
			{
				strcat(temp, "P|");
			}
			else
			{
				strcat(temp, s->map[i][j]=='E' ? "E|" : " |");
			}
/*			printf("%c|", (j == p->posX && i == p->posY) ? 'P' : ' ');*/
		}
		strcat(temp, "\n");
/*		printf("\n");*/
	}
	strcat(temp, "___________\n");
/*	printf("___________\n");*/
/*	printf("%s", temp);*/
	return temp;
}

//on suppose que le tableau tabP, à une taille de NBPLAYERSPERGAME
bool isEmpty(player** tabP)
{
	int i = 0;
	bool res = true;
	for(i; i < NBPLAYERSPERGAME; ++i)
	{
/*		printf("%d\n", tabP[i]);*/
		res = res && (tabP[i]==NULL);
	}
	return res;
}

void * jeuNjoueur (void * arguments)
{
	arg_struct *args = arguments;
	int *tmp = args->sock;
	int nouv_socket_descriptor = tmp;
	int gameNum = args->gameNumber;

    //initilalisation de la structur
    toClient *toSend = (toClient*) malloc(sizeof(toClient));

	player* p = args->p;
	stairs* s = jeux[gameNum].s;

    //TODO: valeur à changer quand le joueur trouve le trésor
    int tresurPos[2];
    tresurPos[0] = -1;//tresurePosX
    tresurPos[1] = -1;//tresurePosY
	
	
	
	while(1)
	{
        //sera envoyé tel quel si erreur si la commande
        toClientInitialisation(toSend);

		printf("Reception d'une commande\n.");
		// Attente de la reception d'un message pour en connaitre la longueur.
		int len = 0;
		while(!len && ioctl(nouv_socket_descriptor, FIONREAD, &len) >= 0)
		{
			usleep(500);
		}
		// Initialisation de la taille de la commande en fonction de la longueur reçu par la socket
		char command[len];
		
		// Pour le moment notifie le client que ça commande à bien été reçue
		/*TODO: retourner le traitement de la nouvelle position*/
		char* result = (char*) malloc(strlen("Commande reçue"));
		sprintf(result, "Commande reçue\n");
		
		int longueur;
		if((longueur = read(nouv_socket_descriptor, command, len)) <= 0)
		{
			return;
		}
		
		/* Il arrive que malgrès la connaissance de la longueur de la commande retournée
		 par la socket il y ai des soucis de longueur.
		 Pour palier à ce probleme on créer une nouvelle variable qui ne contient que les 'len' premiers caractères.*/
		char realCommand[len];
		strncpy(realCommand, command, len);
		realCommand[len] = '\0';
		
		// Récupère la commande envoyé par l'utilisateur
		printf("Commande lue : %s\n", realCommand);
		
		// Faire le traitement en fonction de la commande
		Action* theAction = findActionFromCommand(playerActions, realCommand);
		
		if(theAction == NULL)
		{
			result = (char*) realloc(result, strlen("La commande : , n'existe pas\n") + strlen(realCommand) + 1);
			sprintf(result, "La commande : %s, n'existe pas\n", realCommand);
		}
		else
		{
			theAction->action(p);
			if(strcmp(realCommand, "quit")==0)
			{
				
/*				&p = NULL;*/
				jeux[gameNum].p[0] = NULL;
				printf("bonjour : %d\n", jeux[gameNum].p);
				printf("bonjour : %d\n", &p);
				printf("bonjour : %d\n", p);
				printf("bonjour : %d\n", *p);
				free(p);
				p = NULL;
				printf("bonjour : %d\n", jeux[gameNum].p[jeux[gameNum].nbPlayerInGame-1]);
				jeux[gameNum].nbPlayerInGame--;
				printf("youhou 2 : %d\n", jeux[gameNum].nbPlayerInGame);
/*				printf("bonjour : %s\n", sizeof(jeux[gameNum].p[1]));*/
				if(isEmpty(jeux[gameNum].p))
				{//supprimer le jeu
					printf("le tableau est vide\n");
				}
				break;
			}
			char temp[277] = "\0";
			clientPrintStairs(p, s, temp);
			printPlayerStatus(p, s, temp);
			result = (char*) realloc(result, strlen(temp));
			sprintf(result, "%s", temp);
			serveurPrintStairs(p, s);

            initSending(toSend, p, s, tresurPos);

		}
		
		// Ecrit le nouvel état de l'étage
		write(nouv_socket_descriptor, result, strlen(result)+1);
/*        write(nouv_socket_descriptor, toSend, sizeof(toClient));*/

		printf("Message envoye. \n");
	}
    free(toSend);
	close(nouv_socket_descriptor);
	return;
}

int main(int argc, char* argv[])
{
	vardebug = 0;
	int port = 5000;
	if(argc == 2)
	{
		port = atoi(argv[1]);
	}
	
	//initialisation du nombre d'étage à 1
	jeux = (jeu*) malloc(sizeof(jeu));
	int lastGame = 0;
	int nbPlayer = 0;
	jeu* jeuxTmp = NULL;
	
	//initialisation du premier étage
	jeux[lastGame].s = (stairs *)malloc(sizeof(stairs));
	if(jeux[lastGame].s == NULL)
	{
		perror("Erreur d'allocation mémoire lors de l'initialisation de l'étage");
		return -1;
	}
	stairInitialisation(jeux[lastGame].s);
	
	//on veut NBPLAYERSPERGAME par jeux
	jeux[lastGame].p = (player**) calloc(NBPLAYERSPERGAME, sizeof(player*));
	if(jeux[lastGame].p == NULL)
	{
		perror("Erreur d'allocation mémoire lors de l'initialisation d'un joueur");
		return -1;
	}
	
	int socket_descriptor, 		/* Descripteur de socket */
	nouv_socket_descriptor;	/* Descripteur du nouveau socket */
	
	pthread_t nouveau_client;
	
	socklen_t longueur_adresse_courante;	/* Longueur d'adresse courante d'un client */
	
	sockaddr_in adresse_locale,		/* Structure d'adresse locale */
		adresse_client_courant;		/* Adresse client courant */
	hostent* ptr_hote;		/* Les infos récupérées sur la machine hote */
	//		servent* ptr_service;		/* Les infos récupérées sur le service de la machine */
	char machine[TAILLE_MAX_NOM+1];		/* Nom de la machine locale */
	
	gethostname(machine, TAILLE_MAX_NOM);		/* Récupération du nom de la machine */
	
	/* Récupération de la structure d'adresse en utilisant le nom*/
	if((ptr_hote = gethostbyname(machine)) == NULL)
	{
		perror("Erreur : impossible de trouver le serveur a partir de son nom.");
		exit(1);
	}
	
	/* Initialisation de la structure adresse_locale avec les infos récupérées */
	
	/* Copie de ptr_hote vers adresse_locale */
	bcopy((char*)ptr_hote->h_addr, (char*)&adresse_locale.sin_addr, ptr_hote->h_length);
	adresse_locale.sin_family = ptr_hote->h_addrtype;		/* ou AF_INET */
	adresse_locale.sin_addr.s_addr = INADDR_ANY;		/* ou AF_INET */
	
	/*2 façon de définir le service que l'on va utiliser a distance */
	/*(commenter l'une ou l'autre des solutions)*/
	
	/******************************************/
	/* SOLUTION 1 : utiliser un service déjà existant, par ex. "irc". */
	/*if(ptr_service = getservbyname("irc", "tcp") == NULL)
	 {
	 perror("erreur : impossible de récupérer le numéro de port du service désigné");
	 exit(1);
	 }
	 adresse_locale.sin_port = htons(ptr_service->s_port);
	 */
	/*******************************************/
	/*SOLUTION 2 : utiliser un nouveau numero de port */
	adresse_locale.sin_port = htons(port);
	
	/**************************************************/
	
	printf("Numero de port pour la connexion serveur %d. \n",
		   ntohs(adresse_locale.sin_port) /*ntohs(ptr_service->s_port) */);
	
	/* Création de la socket */
	if((socket_descriptor = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		perror("Erreur : impossible de créer la socket de connexion avec le client.");
		exit(1);
	}
	
	/* Association du socket socket_descriptor à la structure de l'adresse adresse_locale */
	if((bind(socket_descriptor, (sockaddr*)(&adresse_locale), sizeof(adresse_locale))) < 0 )
	{
		perror("Erreur : impossible de lier la socket à l'adresse de connexion.");
		exit(1);
	}
	
	/* Initialisation de la file d'écoute */
	listen(socket_descriptor, 5);
	
	// Fin initialisation socket
	
/*	// Initialisation du joueur*/
/*	player* p = (player *) malloc(sizeof(player));*/
/*	playerInitialisation(p);*/
	
	/* Associe la chaine de caractère entrée par l'utilisateur
	 à la fonction qui correspond */
	playerActions = initialisationActions();// Retourne le tableau de toutes les actions
	
    while(1)
    {
    	printf("deboggage 1: %d\n", vardebug++);
		longueur_adresse_courante = sizeof(adresse_client_courant);
		printf("deboggage 2: %d\n", vardebug++);
		/* adresse_client_courant = sera renseignée par accept via les infos du connect */
		if((nouv_socket_descriptor =
			accept(socket_descriptor,
				   (sockaddr*)(&adresse_client_courant),
				   &longueur_adresse_courante))
		   < 0)
		{
			perror("Erreur : impossible d'accepter la connexion avec le client.");
			exit(1);
		}
		printf("deboggage 3: %d\n", vardebug++);
		
		// Initialisation du joueur
		player* p = (player *) malloc(sizeof(player));
		printf("deboggage 4: %d\n", vardebug++);
		playerInitialisation(p);
		printf("deboggage 5: %d\n", vardebug++);
		
		//asup
/*		player* p1 = (player *) malloc(sizeof(player));*/
/*		playerInitialisation(p1);*/
		
		printf("deboggage 6: %d\n", vardebug++);
		if(nbPlayer%NBPLAYERSPERGAME==0)
		{
            if(nbPlayer > 0)
            {
                lastGame++;
            }
            jeuxTmp = realloc(jeux, sizeof(jeux) + sizeof(jeu*));
			if(jeuxTmp != NULL)
			{
                //TODO: faire une copie mémoire
                jeux = (jeu*) memmove(jeux, jeuxTmp, sizeof(jeux) + sizeof(jeu*));
				jeux = jeuxTmp;
			}
			else
			{
				perror("Erreur d'allocation de mémoire lors de l'ajout d'un nouveau jeu.");
				return -1;
			}
			//initialisation du nouvel étage
			jeux[lastGame].s = (stairs *)malloc(sizeof(stairs));
			stairInitialisation(jeux[lastGame].s);
			jeux[lastGame].nbPlayerInGame = 0;
		}
        nbPlayer++;
		printf("deboggage 7: %d\n", vardebug++);
/*		jeux[lastGame].p[(nbPlayer%NBPLAYERSPERGAME)-1] = p;*/
		printf("youhou : %d \n", jeux[lastGame].nbPlayerInGame);
		printf("deboggage 8: %d\n", vardebug++);
		
        //TODO: erreur
        printf("azertyuiop : %d \n", jeux[lastGame].p[0]);
        printf("azertyuiop : %d \n", jeux[lastGame].p[jeux[lastGame].nbPlayerInGame]);
		jeux[lastGame].p[jeux[lastGame].nbPlayerInGame] = p;
/*		jeux[lastGame].p[jeux[lastGame].nbPlayerInGame+1] = p1;*/
		printf("deboggage 9: %d\n", vardebug++);
		jeux[lastGame].nbPlayerInGame++;
		printf("deboggage 10: %d\n", vardebug++);
/*		jeux[lastGame].p[(nbPlayer%NBPLAYERSPERGAME)] = p1; //asup*/
/*		nbPlayer--;*/
		
		arg_struct args;
		args.sock = nouv_socket_descriptor;
		args.p = p;
		args.gameNumber = lastGame;
		
		/* Traitement du pseudo */
		printf("Reception du pseudo. \n");
		// Attente de la reception d'un message pour en connaitre la longueur.
		int len = 0;
		while(!len && ioctl(nouv_socket_descriptor, FIONREAD, &len) >= 0)
		{
			usleep(500);
		}
		// Initialisation de la taille de la commande en fonction de la longueur reçu par la socket
		char command[len];
		int longueur;
		if((longueur = read(nouv_socket_descriptor, command, len)) <= 0)
		{
			return;
		}
		/*
		 Il arrive que malgrès la connaissance de la longueur de la commande retournée par la socket il y ai des soucis de longueur.
		 Pour palier à ce probleme on créer une nouvelle variable qui ne contient que les 'len' premiers caractères.
		 */
		char realCommand[len];
		strncpy(realCommand, command, len);
		realCommand[len] = '\0';
		p->pseudo = realCommand;
		
        char* temp = "Vous venez d'entrer dans le temple de la mort. Vous n'en ressortirez pas vivant!!!!\n";
//		clientPrintStairs(p, jeux[lastGame].s, temp);
//		printPlayerStatus(p, jeux[lastGame].s, temp);
		serveurPrintStairs(p, jeux[lastGame].s);

        toClient tc;
        toClientInitialisation(&tc);
        int tresurPos[2];
        tresurPos[0] = -1;
        tresurPos[1] = -1;
        initSending(&tc, p, jeux[lastGame].s, tresurPos);

/*        write(nouv_socket_descriptor, &tc, sizeof(toClient));*/
		write(nouv_socket_descriptor, temp, strlen(temp));
		
		if(pthread_create(&nouveau_client, NULL,
								jeuNjoueur,
							  (void*) &args) < 0)
		    {
		    	perror("erreur : impossible de creer un nouveau thread.");
		    	exit(1);
		    }
		/* Traitement du message */
/*		jeuNjoueur(nouv_socket_descriptor, p);*/
		

    }
	close(socket_descriptor);
    return EXIT_SUCCESS;
}
