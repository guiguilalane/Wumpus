#include "wumpus.h"

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
Action* initialisation()
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
	int objects[4][2] = {
							{STAIRSIZE - 1, 0},//echelle
							{-1, -1},//trou
							{-1, -1},//wumpus
							{-1, -1}//trésor
						};
	int x, y;
	srand(time(NULL));
	int k = 1;
	int i = 0, j = 0;
	bool used = false;
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
		 |20|21|22|23|24|
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

int main(int argc, char* argv[])
{

	player* p = (player *) malloc(sizeof(player));
	playerInitialisation(p);
	printPlayerStatus(p);
		
	stairs *s = (stairs *)malloc(sizeof(stairs));
	stairInitialisation(s);
	serveurPrintStairs(s, p);
	
	Action* playerActions = initialisation();
	Action* theAction;
	
    char command[15], temp[15];

    do
    {
        fgets(temp, sizeof(temp), stdin);	//récupère la ligne entrée sur stdin('\n' y compris)
        sscanf (temp, "%[a-zA-Z]s", command);	//filtrage de la chaine pour ne récupérer que les chaines de caractères (et enlever '\n')
        theAction = findActionFromCommand(playerActions, command);
        if(theAction == NULL)
        {
        	printf("la commande : %s, n'existe pas\n", command);
        }
        else
        {
			theAction->action(p);
			serveurPrintStairs(s, p);
        }
        //INFO: Si il y a un bug, permet de ne pas faire plusieurs tours de boucle(mais affichera un message)
        temp[0] = '\0';
        command[0] = '\0';
    }
    while(strcmp(command, "quit"));

    return EXIT_SUCCESS;
}
