%%%%%%%%%% MISE A JOUR DU TODO %%%%%%%%%%

-- Quand on créer un nouvel étage, il faut mettre a jour les sensor dans la structure fromServer et la renvoyer au client ! --> Pour que quand on démarre l'appui les seniors soit affichée sans qu'aucune commande soit envoyée
-- Affichage des scores des autres joueurs dans l'IHM: est-ce que lorsqu'on quit on dit qui a gagné ?
-- Si un joueur quitte une partie en cours, le prochain qui se connecte arrive sur le même jeu ?
	(pour le moment)Les nouveaux joueurs sont insérés dans un jeu existant, si le nombre de joueur dans le jeu n'est pas dépassé et si le jeu est toujours sur le premier étage

%%%%%%%%%% ANCIEN TODO %%%%%%%%%%

	- n client sur la même carte (multi-thread)
		- Lorsque le trésor est trouvé ou le Wumpus mort --> On modifie la carte (variable global)
		- Semaphore: a chaque commande regarder si on peut acceder àla case (un mutex par case)
