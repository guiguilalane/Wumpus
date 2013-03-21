
#define TAILLE_MAX_NOM 256
#define NBPLAYERSPERGAME 3

//taille de l'étage
#define STAIRSIZE 5

//définition des directions;
#define NORTH 0
#define EAST 1
#define SOUTH 2
#define WEST 3

//taille maximum d'une structure d'envoi
#define TAILLEMAX (TAILLE_MAX_NOM+sizeof(int))*(NBPLAYERSPERGAME)+sizeof(int)

//définition des types de structures qui peuvent être envoyées
#define STRUCTACQUITEMENT 0
#define STRUCTMESSAGE 1
#define STRUCTMOVING 2
#define STRUCTDOWN 3
