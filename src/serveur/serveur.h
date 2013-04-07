#include "jeux.h"

#define NBACTION 6

#define A_QUITTER 0
#define A_AVANCER 1
#define A_TOURNERDROITE 2
#define A_TOURNERGAUCHE 3
#define	A_TIRER 4
#define A_DESCENDRE 5

char arrows[4] = {'8', '6', '2', '4'};

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

typedef struct
{
    bool coherent;//indique si les données (reçues/envoyées) sont coherentes
    int direction;
    int playerPosX;
    int playerPosY;
    int tresurePosX;
    int tresurePosY;
    bool tresureFinf;
    bool fallInHole;
    bool wumpusFind;
    bool wumpusKill;
    int score;
    bool besideWumpus;
    bool besideHole;
    bool besideTresure;

} toClient;

//structure indiquant le score d'un joueur
typedef struct
{
    char playerName[TAILLE_MAX_NOM];
    int score;
} scoreP;

typedef struct
{
    int nbScore;
    scoreP scores[TAILLEMAX];
} scoreToClient;

typedef struct
{
    int type;
    char name[15];
    char structure[TAILLEMAX];
} sendToClient;

Action* findActionFromCommand(Action* action, char* command);
Action* initialisationActions();
player *playerInitialisation();
bool sensor(player* p, stairs* s, char o);
int checkPosition(player* p);
void initMovingSending(toClient* c, player* p, sendToClient* stc);
void initDownSending(sendToClient* stc);
void initStairSending(char* why, sendToClient* stc);
void initAcquitementSending(sendToClient* stc);
void initScoreSending(char* why, scoreToClient *sctc, player *player, sendToClient* stc);
