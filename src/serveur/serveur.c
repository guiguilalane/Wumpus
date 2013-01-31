#include "serveur.h"

#include <linux/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <netdb.h>
#define TAILLE_MAX_NOM 256
#define h_addr h_addr_list[0] //TODO pour eviter : "erreur: ‘hostent’ has no member named ‘h_addr’"

typedef struct sockaddr sockaddr;
typedef struct sockaddr_in sockaddr_in;
typedef struct hostent hostent;
typedef struct servent servent;

// Joueur
player* p;
// Etage
stairs *s;
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
	p->posX = 0;
	p->posY = (STAIRSIZE - 1);
	p->direction = NORTH;
	p->arrow = true;
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

/***************************************TO FINISH***************************************/

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


void printPlayerStatus(player* p)
{
	char direction[11];
	getDirection(p->direction, direction);
	printf("Le joueur est à la position : (%d, %d)\nregarde en direction %s et %s sa flèche.\n",
		   p->posX, p->posY, direction, p->arrow ? "possède encore" : "ne possède plus");
}

// Affiche l'étage sans les informations sur la position des éléments (trésor, wumpus, trou)
/*
 (0,0)___________
 |0|1|2|3|4|
 |5|6|7|8|9|
 |10|11|12|13|14|
 |15|16|17|18|19|
 (4,0)|20|21|22|23|24|
 ___________(4,4)
 */
void serveurPrintStairs(stairs* s, player* p)
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


void clientPrintStairs(stairs* s, player* p)
{//TODO: faire un prétraitement pour afficher la map avec le résultat des senseurs.
	
}

void jeu1joueur (int sock)  {
	
	while(1)
	{
		printf("Reception d'une commande\n.");
		// Attente de la reception d'un message pour en connaitre la longueur.
		int len = 0;
		while(!len && ioctl(sock, FIONREAD, &len) >= 0)
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
		if((longueur = read(sock, command, len)) <= 0)
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
			serveurPrintStairs(s, p);
		}
		//INFO: Si il y a un bug, permet de ne pas faire plusieurs tours de boucle(mais affichera un message)
		/*        buffer[0] = '\0';*/
		
		// Ecrit le nouvel état de l'étage
		write(sock, result, strlen(result)+1);
		printf("Message envoye. \n");
	}
	return;
}

int main(int argc, char* argv[])
{
	
	int socket_descriptor, 		/* Descripteur de socket */
	nouv_socket_descriptor;	/* Descripteur du nouveau socket */
	
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
	adresse_locale.sin_port = htons(5000);
	
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
	
	// Initialisation du jeu
	p = (player *) malloc(sizeof(player));
	playerInitialisation(p);
	/* printPlayerStatus(p);*/
	
	s = (stairs *)malloc(sizeof(stairs));
	stairInitialisation(s);
	/*	serveurPrintStairs(s, p);*/
	
	/* Associe la chaine de caractère entrée par l'utilisateur
	 à la fonction qui correspond */
	playerActions = initialisationActions();// Retourne le tableau de toutes les actions
	
    while(1)
    {
		longueur_adresse_courante = sizeof(adresse_client_courant);
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
		write(nouv_socket_descriptor, p->pseudo, strlen(p->pseudo)+1);
		
		/* Traitement du message */
		jeu1joueur(nouv_socket_descriptor);
		
		close(nouv_socket_descriptor);
    }
	
    return EXIT_SUCCESS;
}
