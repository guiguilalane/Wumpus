#include "serveur.h"

#include <linux/types.h>
#include <sys/socket.h>
#include <netdb.h>
#define TAILLE_MAX_NOM 256
//#define h_addr h_addr_list[0] //TODO pour eviter : "erreur: ‘hostent’ has no member named ‘h_addr’"

typedef struct sockaddr sockaddr;
typedef struct sockaddr_in sockaddr_in;
typedef struct hostent hostent;
typedef struct servent servent;

	//joueur
	player* p;
	//etage
	stairs *s;
	//actions disponibles
	Action* playerActions;

//lorsque le joueur veux quitter la partie
void quit(player* p)
{
    printf("terminer la partie et afficher le score\n");
}

//lorsque le personnage avance
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
/*    printf("avancer d'une case dans la direction pointée.\n");*/
}

//lorsque le personnage tourne à droite
void turn_right(player* p)
{
	p->direction = (p->direction+1)%4;
	printf("Le personnage change de direction dans le sens horaire.\n");
}

//lorsque le personnage tourne à gauche
void turn_left(player* p)
{
	p->direction = (p->direction+3)%4;//tourne 3 fois dans le sens horaire
	printf("Le personnage change de direction dans le sens anti-horaire.\n");
}

//lorsque le peronnage tire une flèche
void shot()
{
	printf("Le personnage tire une flèche.\n");
}

//lorsque le personnage descend l'échelle
void down()
{
	printf("Le personnage descend d'un étage.\n");
}

//recherche l'action qui correspond à la commande
/*
param action : ensemble des actions disponibles dans le jeu
return l'action désignée par command*/
Action* findActionFromCommand(Action* action, char* command)
{
    Action* a;
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

//initialiser les actions disponibles
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

//initialise le joueur à chaque début de niveau
void playerInitialisation(player* p)
{
	p->posX = 0;
	p->posY = (STAIRSIZE - 1);
	p->direction = NORTH;
	p->arrow = true;
}

//initialise l'étage
void stairInitialisation(stairs *s)
{	
			/*définition des coordonnées des objets*/
			
	//contiendra les coordonnées de l'échelles, du trou, du wumpus et du trésor.
	int objects[4][2] = { //{ y,  x}
							{STAIRSIZE - 1, 0},//echelle
							{-1, -1},//trou
							{-1, -1},//wumpus
							{-1, -1}//trésor
						};
	int x, y;
	srand(time(NULL));
	int k = 1; //represente tours après tours : trou, wumpus, trésor
	int i = 0, j = 0;
	bool used = false;//coordonnées déja utilisées ou non
	while(k <= 3)
	{
		//définition aléatoire des coordonnées du prochain objet à placer
		x = (rand()%STAIRSIZE);
		y = (rand()%STAIRSIZE);
		used = false;
		i = 0;
		//verification que les coordonnées ne sont pas déjà utilisée
		while(!used && i < 4)
		{
			used = used || ((objects[i][0] == x) && (objects[i][1] == y));
			++i;
		}
		if(!used)
		{//si non utiliser on stock les coordonnées dans une variables objects
			objects[k][0] = x;
			objects[k][1] = y;
			++k;
		}
	}

		/*mise en place des objets sur la map*/

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
	
	//lecture de la variables objects et placement des objects
	for(k = 0; k < 4 ; ++k)
	{
		x = objects[k][0];
		y = objects[k][1];
		switch(k)
		{
			case 0:
				s->map[x][y] = 'E';//echelle
				break;
			
			case 1:
				s->map[x][y] = 'H';//trou(hole)
				break;
			
			case 2:
				s->map[x][y] = 'W';//wumpus
				break;
			
			case 3:
				s->map[x][y] = 'T';//tresor
				break;
		}
	}
}

/***************************************TO FINISH***************************************/

//fonction temporaire
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
	printf("le joueur est à la position : (%d, %d)\nregarde en direction %s et %s sa flèche.\n", 
			p->posX, p->posY, direction, p->arrow ? "possède encore" : "ne possède plus");
}

//affiche l'étage sans les informations sur la position des éléments(trésor, wumpus, trou)
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

void traitementCommande (int sock)  {

	char buffer[TAILLE_MAX_NOM];
	int longueur;
	printf("plop 2\n");
	
	if((longueur = read(sock, buffer, sizeof(buffer))) <= 0)
	{
		return;
	}
	printf("plop 3\n");
	
	//récupère la commande envoyé par l'utilisateur
	printf("message lu : %s \n", buffer);
	
	//faire le traitement en fonction de la commande
	Action* theAction = findActionFromCommand(playerActions, buffer);
        if(theAction == NULL)
        {
        	printf("la commande : %s, n'existe pas\n", buffer);
        }
        else
        {
			theAction->action(p);
			serveurPrintStairs(s, p);
        }
        //INFO: Si il y a un bug, permet de ne pas faire plusieurs tours de boucle(mais affichera un message)
        buffer[0] = '\0';

	//écrit le nouvel état de l'étage
/*	write(sock, buffer, strlen(buffer)+1);*/
	printf("message envoye. \n");
	return;
}

int main(int argc, char* argv[])
{

	int socket_descriptor, 		/*descripteur de socket*/
		nouv_socket_descriptor;	/*descripteur du nouveau socket*/
		
	socklen_t longueur_adresse_courante;		/*longueur d'adresse courante d'un client*/
		
		sockaddr_in adresse_locale,		/*structure d'adresse locale*/
					adresse_client_courant;		/*adresse client courant*/
		hostent* ptr_hote;		/*les infos récupérées sur la machine hote*/
//		servent* ptr_service;		/*les infos récupérées sur le service de la machine*/
		char machine[TAILLE_MAX_NOM+1];		/*nom de la machine locale*/
		
		gethostname(machine, TAILLE_MAX_NOM);		/*récupération du nom de la machine*/
		
		/*récupération de la structure d'adresse en utilisant le nom*/
		if((ptr_hote = gethostbyname(machine)) == NULL)
		{
			perror("error : impossible de trouver le serveur a partir de son nom.");
			exit(1);
		}
		
		/*initialisation de la structure adresse_locale avec les infos récupérées*/
		
		/*copie de ptr_hote vers adresse_locale */
		bcopy((char*)ptr_hote->h_addr, (char*)&adresse_locale.sin_addr, ptr_hote->h_length);
		adresse_locale.sin_family = ptr_hote->h_addrtype;		/*ou AF_INET*/
		adresse_locale.sin_addr.s_addr = INADDR_ANY;		/*ou AF_INET*/
		
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
		
		printf("numero de port pour la connexion serveur %d. \n",
				ntohs(adresse_locale.sin_port) /*ntohs(ptr_service->s_port) */);
				
		/*création de la socket */
		if((socket_descriptor = socket(AF_INET, SOCK_STREAM, 0)) < 0)
		{
			perror("erreur : impossible de créer la socket de connexion avec le client.");
			exit(1);
		}
		
		/*association du socket socket_descriptor à la structure de l'adresse adresse_locale */
		if((bind(socket_descriptor, (sockaddr*)(&adresse_locale), sizeof(adresse_locale))) < 0 )
		{
			perror("erreur : impossible de lier la socket à l'adresse de connexion.");
			exit(1);
		}
		
		/*initialisation de la file d'écoute*/
		listen(socket_descriptor, 5);
		
		//fin initialisation socket

	//initialisation du jeu
	p = (player *) malloc(sizeof(player));
	playerInitialisation(p);
/*	printPlayerStatus(p);*/
		
	s = (stairs *)malloc(sizeof(stairs));
	stairInitialisation(s);
/*	serveurPrintStairs(s, p);*/
	
	/*associe la chaine de caractère entrée par l'utilisateur
	  à la fonction qui correspond*/
	playerActions = initialisationActions();//retourne le tableau de toutes les actions
	
    while(1)
    {
    	longueur_adresse_courante = sizeof(adresse_client_courant);
		/*adresse_client_courant = sera renseignée par accept via les infos du connect*/
		if((nouv_socket_descriptor = 
			accept(socket_descriptor,
				(sockaddr*)(&adresse_client_courant),
				&longueur_adresse_courante))
			< 0)
		{
			perror("erreur : impossible d'accepter la connexion avec le client.");
			exit(1);
		}
		
		/*traitement du message*/
		printf("reception d'un message.\n.");
		traitementCommande(nouv_socket_descriptor);
		printf("plop 1\n");
		
		close(nouv_socket_descriptor);
    }

    return EXIT_SUCCESS;
}
