	G- serveur renvoi la carte (echelle + position du joueur + résultat des senseurs) (check!)
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
		
	- Affichage des scores quand l'un des joueurs quit la partie -> Comment est-ce qu'on va gérer le fait qu'un joueur quitte la partie (est-ce que ca arrete pour tout le monde ? si non: est-ce qu'un joueur qui se connecte peut rejoindre la partie en cours ou non)
	
	- Quand on joueur se fait tuer ou tombe dans le trou ! Comment ça se passe pour gérer avec les autres joueurs ! Il perd des points et attends qu'un joueur passe à un étage inférieur ??????????????

THE MOST IMPORTANT FOR TOMMOROW:
- Lors du down Avertir tous les autres joueurs --> Alors là j'ai essayé de le faire mais je pense qu'il y a un souci au nivo des socket récupérer. J'ai ajouter la socket pour chaque joueur (ligne 816) mais elle n'a pas l'air de correspondre avec le numéro de socket coté client !
- Puis réinitialiser le stair
- Quand on creér l'étage --> Ajouter les bon sensor au joueurs et les renvoyer au serveur???
- Dans IHM remettre à jour les données de la carte ... --> NO

