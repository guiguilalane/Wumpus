%%%%%%%%%% MISE A JOUR DU TODO %%%%%%%%%%

-- Quand on créer un nouvel étage, il faut mettre a jour les sensor dans la structure fromServer et la renvoyer au client ! --> Pour que quand on démarre l'appui les seniors soit affichée sans qu'aucune commande soit envoyée
-- Affichage des scores des autres joueurs dans l'IHM: est-ce que lorsqu'on quit on dit qui a gagné ?
-- Si un joueur quitte une partie en cours, le prochain qui se connecte arrive sur le même jeu ?
	(pour le moment)Les nouveaux joueurs sont insérés dans un jeu existant, si le nombre de joueur dans le jeu n'est pas dépassé et si le jeu est toujours sur le premier étage
	
%%%%%%%%%%% PB ULTRA-BIZARRE %%%%%%%%%%%%%
 1) lancer le serveur
 2) lancer deux clients
 3) tuer les deux clients sans fermer les premières boites de dialogue indiquant de quoi est mort le joueur.
 4) attendre la deuxieme boite de dialogue(pour chaque client).
 5) fermer toutes les boites de dialogues.
 6) (PB ici)pour cette partie, si un seul des joueurs est tué, l'étage sera reinitialisé comme si les deux joueurs étaient mort.

%%%%%%%%%% ANCIEN TODO %%%%%%%%%%

	- n client sur la même carte (multi-thread)
		- Lorsque le trésor est trouvé ou le Wumpus mort --> On modifie la carte (variable global)
		- Semaphore: a chaque commande regarder si on peut acceder àla case (un mutex par case)
