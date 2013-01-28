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

//Définition du joueur
typedef struct
{
	int posX;
	int posY;
	int direction;
	bool arrow;
} player;

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

Action* findActionFromCommand(Action* action, char* command);
Action* initialisation();
