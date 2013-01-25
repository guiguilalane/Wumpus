/* -----------------------------------------------
Client a lancer après le serveur avec la commande:
client <adresse-serveur> <message-a-transmettre> 
------------------------------------------------*/

#include <stdlib.h>
#include <stdio.h>
#include <linux/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>
#define h_addr h_addr_list[0] //TODO pour eviter : "erreur: ‘hostent’ has no member named ‘h_addr’"

typedef struct sockaddr sockaddr;
typedef struct sockaddr_in sockaddr_in;
typedef struct hostent hostent;
typedef struct servent servent;

int main(int argc, char **argv){
 
	int socket_descriptor, /* Descripteur de socket */
	    longueur; /* Longueur d'un buffer utilisé */
	sockaddr_in adresse_locale; /* Adresse de socket local */
	hostent * ptr_host; /* Info sur une machine hote */
	servent * ptr_service; /* Info sur un service */
	char buffer[256];
	char * prog; /* Nom du programme */
	char * host; /* Nom de la machine distante */
	char * mesg; /* Message envoye */
	
	if (argc !=3) {
		perror("Usage: client <adresse-serveur> <message-a-transmettre>");
		exit(1);
	}
	
	prog = argv[0];
	host=argv[1];
	mesg=argv[2];
	
	printf("Nom de l'executable : %s \n", prog);
	printf("Adresse du serveur : %s \n", host);
	printf("Message envoye : %s \n", mesg);
	
	if ((ptr_host = gethostbyname(host)) == NULL){
		perror("Erreur : impossible de trouver le serveur à partir de son adresse.");
		exit(1);
	}
	
	/* Copie caractère par caractère des infos de ptr_host vers adresse_locale */
	bcopy((char*)ptr_host->h_addr, (char*)&adresse_locale.sin_addr, ptr_host->h_length);
	adresse_locale.sin_family = AF_INET; /* ou ptr_host->h_addrtype */
	
	/* SOLUTION 1 : utiliser un service existant, par exemple "irc" */
	
	/*
	if(ptr_service=getservbyname("irc","tcp")) == NULL) {
		perror("Erreur : impossible de recuperer le numero de port du service desire.");
		exit(1);
	}
	adresse_locale.sin_port = htons(ptr_service->s_port);
	*/
	
	/* SOLUTION 2 : utiliser un nouveau numéro de port */
	adresse_locale.sin_port = htons(5000);
	
	printf("Numéro de port pour la connexion au serveur : %d \n", ntohs(adresse_locale.sin_port));
	
	/* Création de la socket */
	if ((socket_descriptor = socket(AF_INET, SOCK_STREAM, 0)) < 0){
		perror("Erreur : impossible de créer la socket de connexion avec le serveur");
		exit(1);
	}
	
	/* Tentative de connexion au serveur dont les infos sont dans adresse_locale */
	if ((connect(socket_descriptor, (sockaddr*)(&adresse_locale), sizeof(adresse_locale))) < 0){
		perror("Erreur : impossible de se connecter au serveur");
		exit(1);
	}
	
	printf("Connexion établie avec le serveur \n");
	printf("Envoi d'un message au serveur \n");
	
	/* Envoi du message vers le serveur */
	if ((write(socket_descriptor, mesg, strlen(mesg))) < 0) {
		perror("Erreur : impossible d'écrire le message destine au serveur.");
		exit(1);
	}
	
	/* Mise en attente du programme pour simuler un délai de transmission */
	sleep(3);
	
	printf("Message envoye au serveur. \n");
	
	/* Lecture de la réponse en provenance du serveur */
	while ((longueur = read(socket_descriptor, buffer, sizeof(buffer))) > 0) {
		printf("Réponse du serveur : \n");
		write(1,buffer,longueur);
	}
	
	printf("\nFin de la reception.\n");
	
	close(socket_descriptor);
	
	printf("Connexion avec le serveur fermee, fin du programme. \n");
	
	exit(0);
}
