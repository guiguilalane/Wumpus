Wumpus
======

Projet Réseau :  Wumpus

    Présentation :
  			Le Wumpus est un jeu ou un personnage va devoir parcourir des étages d'une tour sousterraine, à la recherche de
				trésors. Munis de ces flèches il devra tuer le Wumpus pour rester en vie.
			
			But du jeu :
				La tour sousterraine est composée d'étages.
				Chaque étage est représentée par une grille qui comprends :
					- une échelle pour descendre à l'étage inférieur (lorsque le trésors à été trouvé), située dans le coin
					inférieur gauche du souterrain.
					- un trésors (une fois que le trésor à été trouvé, le joueur à la possibilité de descendre à l'étage inférieur
					à l'aide de l'échelle)
					- un trou hérissé de lances (le joueur qui tombe dans le trou à perdu)
					- un Wumpus (le Wumpus peut être tué à l'aide d'une flèche)
				Le but du jeu est de parcourir les étages, en trouvant les trésors, tuant les Wumpus et en évitant les trous.
				
			Le joueur possède :
					- une flèche qui lui permet de tuer le Wumpus
					
			Pour parcourir un étage le joueur devra effectuer un certain nombre de commandes.
			commandes :
				- avancer : le personnage avance d'une case dans la direction dans laquelle il regarde.
				- tourner à droite : le personnage change de direction en tournant dans le sens horaire.
				- tourner à gauche : le personnage change de direction en tournant dans le sens anti-horaire.
				- tirer : le personnage tire son unique flèche dans la direction dans laquelle il regarde.
				- descendre : équivaut à finir le niveau. Cette commande n'a d'effet que si le personnage à 
				trouvé le trésor et est sur la case de l'echelle(en bas à gauche).
				- quitter : Le joueur remonte à la surface (affiche le score du joueur)
				
			Le Joueur connaît sa position sur la grille représentant l'étage de la tour sousterraine.
			Au cours de ces aventures précédentes le personnage à acquis un certain nombre de sensors.
			Ces sensors permettent au joueur d'obtenir certaines informations sur son environement proche.
			sensors :
				- WIND : Ce sensor est activé lorsque le personnage se situe sur une case adjacente à un trou.
				- TWINKLE(scintiller) : Ce sensor s'active losrque le personnage se situe sur une case adjacente au trésors.
				- STINK(empester) : Ce sensor s'active losrque le personnage se situe sur une case adjacente au Wumpus.
				
			Dans ce jeu il y a plusieurs façons de terminer une partie.
			Terminer une partie :
				- Le joueur est tué, soit par le Wumpus, soit en tombant dans un trou.
				- Le joueur décide qu'il est temps de remonter à la surface.
			
			Lorsque la partie est terminée le joueur se voit attribuer un score en fonction :
				- du nombre d'étage parcourus
				- du nombre de trésors trouvés
				- du nombre de Wumpus tués
				
			
					
			commande optionnel :
				- jetter un caillou : le personnage jette un caillou dans la direction dans laquelle il regarde.
					Le joueur ne possede qu'un caillou pour chaque étage. Si le joueur jette son caillou,
					le caillou est définitvement utilisé. Si le caillou est lancé dans un trou, un 'TOC' se fait entendre.
					Si le caillou est lancé sur le Wumpus, le Wumpus avance sur la case du joueur et le joueur à perdu.
