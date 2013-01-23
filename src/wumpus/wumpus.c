#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include<stdbool.h>

#define NBACTION 6

#define A_QUITTER 0
#define A_AVANCER 1
#define A_TOURNERDROITE 2
#define A_TOURNERGAUCHE 3
#define	A_TIRER 4
#define A_DESCENDRE 5

/*déclaration du type T_FONC_ACTION*/
typedef void T_FONC_ACTION();


typedef struct {
	char* command;
    T_FONC_ACTION* action;
} Action;

/*Déclaration des fonctions prototypes*/
T_FONC_ACTION quit;
T_FONC_ACTION move;
T_FONC_ACTION turn_right;
T_FONC_ACTION turn_left;
T_FONC_ACTION shot;
T_FONC_ACTION down;

//lorsque le joueur veux quitter la partie
void quit()
{
    printf("terminer la partie et afficher le score\n");
}

//lorsque le personnage avance
void move()
{
    printf("avancer d'une case dans la direction pointée.\n");
}

//lorsque le personnage tourne à droite
void turn_right()
{
	printf("Le personnage change de direction dans le sens horaire.\n");
}

//lorsque le personnage tourne à gauche
void turn_left()
{
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
    tRightAction->command = "turn_right";
    tRightAction->action = turn_right;
    actions[A_TOURNERDROITE] = *tRightAction;
    
    //Action tourner à gauche
    Action* tLeftAction = (Action*)malloc(sizeof(Action));
    tLeftAction->command = "turn_left";
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

int main(int argc, char* argv[])
{

	Action* playerActions = initialisation();
	Action* theAction;
	
    char command[10], temp[10];

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
			theAction->action();
        }
        
    }
    while(strcmp(command, "quit"));

    return EXIT_SUCCESS;
}
