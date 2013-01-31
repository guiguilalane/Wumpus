
	N- identification du client par un pseudo pour l'affichage des scores
	G- serveur renvoi la carte (echelle + position du joueur + résultat des senseurs) 
	- n client sur la même carte (multi-thread)
		- Lorsque le trésor est trouvé ou le Wumpus mort --> On modifie la carte (variable global)
		- Semaphore: a chaque commande regarder si on peut acceder àla case (un mutex par case)
		
	
	
	
	
	
	
	
	
	
	
	
	
	
	- limiter le nombre de client par carte. Si une carte est pleine, créer une nouvelle carte :
		1) definir une structure jeu avec un étage et n joueur (définir n)
		2) créer un tableau de jeu en global coté serveur
		3) chaque fois que le nombre de joueur d'un jeu est atteind créer un nouveau jeu.
		
	- finir l'implementation des commande du jeu (tirer la flèche (avertir seulement le joueur qui a tiré, si il a reussi a tuer the Wumpus), descendre*, les sensors (a chaque commande))
	
	*notification aux joueurs qui n'ont pas effectuer la commande:
		changement d'étage
