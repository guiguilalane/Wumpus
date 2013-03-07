#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <netdb.h>
#include <pthread.h>

#define TAILLE_MAX_NOM 256
#define NBPLAYERSPERGAME 3

//taille de l'étage
#define STAIRSIZE 5

/*Le point d'origine se trouve en haut à gauche de la map*/
/**
 * @brief structure about stairs
 *
 */
typedef struct
{
    char map[STAIRSIZE][STAIRSIZE]; /**< the map representation */
    bool wumpusAlive; /**< whether the wumpus was killed or not */
    bool tresureFounded; /**< whether the tresure was found or not */
} stairs;

//Définition du joueur
//Double liste chainée

/**
 * @brief structure about players informations
 *
 *  This structure is a double Linked list that contain all informations about players
 *
 */
typedef struct player player;
struct player
{
    player *previousPlayer; /**< The previous player*/

    char pseudo[TAILLE_MAX_NOM]; /**< The player pseudonym*/
    int score; /**< The player score */
    int posX; /**< The player X position in the map */
    int posY; /**< The player Y position in the map */
    int tresurPosX;/**< The Tresur X position in the map when the player found it */
    int tresurPosY;/**< The player Y position in the map when the player found it */
    int direction; /**< The player watching direction */
    bool arrow; /**< Whether the player have his arrow */
    bool deadByWumpus; /**< Whether the player was killed by the Wumpus */
    bool fallInHole; /**< Whether the player was fell in a hole */
    bool findTresure; /**< Whether the player find the tresure */
    bool shotTheWumpus; /**< Whether the player had killed the wumpus */
    struct jeu *game; /**< The game that contain the player */
    int sock; /**< The socket to communicate with the player */

    player *nextPlayer; /**< The next player */

};

//Double liste chainée
/**
 * @brief structure about games
 *
 *  This structure is a double linked list that contain game informations
 *
 */
typedef struct jeu jeu;
struct jeu
{

    struct jeu *previousGame; /**< The previous game */

    pthread_mutex_t playerMutex;
    stairs *etage; /**< The stair where players move */
    player *joueur; /**<  The first created player*/
    int nbPlayer; /**< The number of player in the game */
    int numberOfStairs; /**< The number of stairs down */

    struct jeu *nextGame; /**< The next game */

};

/**
 * @brief the game manager that contain the first and the last created game
 *
 */
typedef struct{

    jeu *firstCreatedGame; /**< The first created game */
    jeu *lastCreatedGame; /**< The last created game */

} gameManager;

gameManager* manager;

//créer un nouveau jeu
/*
 * - Créer un nouvelle étage
 * - ajouter le joueur
*/
/**
 * @brief create a new stair and add the player to the game
 *
 *  @param the first player in the game
 *
 *  @return the new created game
 *
 */
jeu *createGame(player* p);

/**
 * @brief create a new stair and update the game
 *
 * Use when player send the 'down' command
 *
 * @param the game to update with
*/
void createNewStair(jeu *j);

/**
 * @brief remove and delete a game from the gameManager
 *
 *  A game have been destroyed when all game player leave the specific game
 *
 * @param j the game to remove
 */
void removeGame(jeu * j);

/**
 * @brief add a player in a game
 *
 *  the structure add a player in the last game whether the game is on the first floor,
 *      create a new game otherwise.
 *
 * @param p the player to add
 * @return the game that the player was added
 */
jeu *addPlayer(player *p);

/**
 * @brief remove and delete a specific player from a game
 *
 *  A player is remove when he leave the game.
 *
 *  It is strongly recommended to check whether the game is empty of player
 *
 * @param p the player to remove
 * @return the game that the player was removed or NULL if the player was not in the specific game
 */
jeu *removePlayer(player *p);

/**
 * @brief search the last player of a specific game
 *
 * @pre j must contain at least one player
 *
 * @param j the game which run through to find the last player
 * @return the last player of the specific game
 */
player *findLastPlayerFromGame(jeu* j);


void resetGamePlayer(jeu* j);


int numberOfPlayer();

//************************************************

/**
 * @brief create and allocate a stairs
 *
 * @return the new stairs
 */
stairs *stairInitialisation();

player* reinitPlayer(player *p);

