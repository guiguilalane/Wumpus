%%%%%%%%%% MISE A JOUR DU TODO %%%%%%%%%%

-- Quand on créer un nouvel étage, il faut mettre a jour les sensor dans la structure fromServer et la renvoyer au client ! --> Pour que quand on démarre l'appui les seniors soit affichée sans qu'aucune commande soit envoyée
-- Quand on joueur se fait tuer ou tombe dans le trou ! Attendre qu'un changement d'étage soit effectué ! Sauf s'il est tout seul sur la carte, faire un changement d'étage automatique
-- Affichage des scores des autres joueurs dans l'IHM: est-ce que lorsqu'on quit on dit qui a gagné ?
-- Si un joueur quitte une partie en cours, le prochain qui se connecte arrive sur le même jeu ?

%%%%%%%%%% ANCIEN TODO %%%%%%%%%%

	- n client sur la même carte (multi-thread)
		- Lorsque le trésor est trouvé ou le Wumpus mort --> On modifie la carte (variable global)
		- Semaphore: a chaque commande regarder si on peut acceder àla case (un mutex par case)