#include "jeux.h"

jeu *createGame(player* p)
{
    jeu* j = (jeu*) malloc(sizeof(jeu));
    if(j == NULL)
    {
        perror("Erreur d'allocation mémoire lors de la création d'un nouveau jeu");
        return NULL;
    }
    j->etage = stairInitialisation();
    j->joueur = p;
    p->game = j;
    j->nbPlayer = 1;
    j->numberOfStairs = 1;
    if(NULL == manager->firstCreatedGame)
    {//aucun jeu n'est créé
        j->previousGame = NULL;
        manager->firstCreatedGame = j;
        manager->lastCreatedGame = j;
    }
    else
    {//au moins un jeu est créé
        j->previousGame = manager->lastCreatedGame;
        manager->lastCreatedGame->nextGame = j;
        manager->lastCreatedGame = j;
    }
    j->nextGame = NULL;

    return j;
}

void createNewStair(jeu *j)
{
	free(j->etage);
	j->etage = stairInitialisation();
}

void removeGame(jeu * j)
{
    if(manager->lastCreatedGame != j)
    {//le jeu à supprimer n'est pas le dernier jeu créé
        j->nextGame->previousGame = j->previousGame;
    }
    else
    {//le jeu à supprimer est le dernier jeu créé
        manager->lastCreatedGame = j->previousGame;
    }
    if(manager->firstCreatedGame != j)
    {
        j->previousGame->nextGame = j->nextGame;
    }
    else
    {
        manager->firstCreatedGame = j->nextGame;
    }
    free(j->etage);
    free(j);
}

jeu *addPlayer(player *p)
{
    jeu* j = manager->lastCreatedGame;
    if(NULL != j)
    {//au moins un jeu est créé

        /*
         *Cette section définit si ajoute le nouveau joueur
         *à une partie déjà créée, ou si on doit créer une
         *nouvelle partie.
         *NOTE: Cette section est susceptible de changer.
        */
        {
            if(j->nbPlayer < NBPLAYERSPERGAME && j->numberOfStairs < 2)
            {
                player *lastPlayer = findLastPlayerFromGame(j);
                lastPlayer->nextPlayer = p;
                p->previousPlayer = lastPlayer;
                p->game = j;
                j->nbPlayer++;
            }
            else
            {
                j = createGame(p);
            }
        }

    }
    else
    {//aucun jeu n'est créé
        j = createGame(p);
    }

    return j;
}

jeu *removePlayer(player *p)
{
    jeu* j = p->game;
    if(j->joueur != p)
    {
        p->previousPlayer->nextPlayer = p->nextPlayer;
    }
    else
    {//le joueur à supprimer est le premier joueur dans la liste du jeu
        j->joueur = p->nextPlayer;
    }

    if(NULL != j->joueur)
    {//si le jeu n'est pas vide
        if(NULL != p->nextPlayer)
        {
            p->nextPlayer->previousPlayer = p->previousPlayer;
        }
        else
        {
            p->previousPlayer->nextPlayer = NULL;
        }
    }
    j->nbPlayer--;
    free(p);

    return j;
}

player *findLastPlayerFromGame(jeu *j)
{
    player *current = j->joueur;
    while(current->nextPlayer != NULL)
    {
        current = current->nextPlayer;
    }
    return current;
}

int numberOfPlayer()
{
    int count = 0;
    jeu* current = manager->firstCreatedGame;
    while(current != NULL)
    {
        count += current->nbPlayer;
        current = current->nextGame;
    }
    return count;
}
