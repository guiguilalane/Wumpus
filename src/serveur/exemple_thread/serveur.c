#include <stdlib.h>
#include <stdio.h>
#include <linux/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>
#include <pthread.h>
#define TAILLE_MAX_NOM 256
#define h_addr h_addr_list[0] //TODO pour eviter : "erreur: ‘hostent’ has no member named ‘h_addr’"

typedef struct sockaddr sockaddr;
typedef struct sockaddr_in sockaddr_in;
typedef struct hostent hostent;
typedef struct servent servent;

/**************************************/
void * prise_en_charge (void * sock)  {

	int *tmp = (int *) sock;
	int nouv_socket_descriptor = *tmp;
	char buffer[TAILLE_MAX_NOM];
	int longueur;
	
	if((longueur = read(nouv_socket_descriptor, buffer, sizeof(buffer))) <= 0)
	{
		return;
	}
	
	printf("message lu : %s \n", buffer);
	
	buffer[0] = 'R';
	buffer[1] = 'E';
	buffer[longueur] = '#';
	buffer[longueur+1] = '\0';
	
	printf("message après traitement : %s \n", buffer);
	
	printf("renvoi du message traité. \n");
	
	/*mise en attente du programme pour simuler un delai de transmission*/
	sleep(3);
	
	write(nouv_socket_descriptor, buffer, strlen(buffer)+1);
	printf("message envoye. \n");
	close(nouv_socket_descriptor);
	return;
}

/******************************************/

/******************************************/
int main(int argc, char *argv[])
{

	int socket_descriptor, 		/*descripteur de socket*/
		nouv_socket_descriptor;	/*descripteur du nouveau socket*/
	
	pthread_t nouveau_client;
		
	socklen_t longueur_adresse_courante;		/*longueur d'adresse courante d'un client*/
		
		sockaddr_in adresse_locale,		/*structure d'adresse locale*/
					adresse_client_courant;		/*adresse client courant*/
		hostent* ptr_hote;		/*les infos récupérées sur la machine hote*/
//		servent* ptr_service;		/*les infos récupérées sur le service de la machine*/
		char machine[TAILLE_MAX_NOM+1];		/*nom de la machine locale*/
		
		gethostname(machine, TAILLE_MAX_NOM);		/*récupération du nom de la machine*/
		
		/*récupération de la structure d'adresse en utilisant le nom*/
		if((ptr_hote = gethostbyname(machine)) == NULL)
		{
			perror("error : impossible de trouver le serveur a partir de son nom.");
			exit(1);
		}
		
		/*initialisation de la structure adresse_locale avec les infos récupérées*/
		
		/*copie de ptr_hote vers adresse_locale */
		bcopy((char*)ptr_hote->h_addr, (char*)&adresse_locale.sin_addr, ptr_hote->h_length);
		adresse_locale.sin_family = ptr_hote->h_addrtype;		/*ou AF_INET*/
		adresse_locale.sin_addr.s_addr = INADDR_ANY;		/*ou AF_INET*/
		
		/*2 façon de définir le service que l'on va utiliser a distance */
		/*(commenter l'une ou l'autre des solutions)*/
		
		/******************************************/
		/* SOLUTION 1 : utiliser un service déjà existant, par ex. "irc". */
		/*if(ptr_service = getservbyname("irc", "tcp") == NULL)
		{
			perror("erreur : impossible de récupérer le numéro de port du service désigné");
			exit(1);
		}
		adresse_locale.sin_port = htons(ptr_service->s_port);
		*/
		/*******************************************/
		/*SOLUTION 2 : utiliser un nouveau numero de port */
		adresse_locale.sin_port = htons(5000);
		
		/**************************************************/
		
		printf("numero de port pour la connexion serveur %d. \n",
				ntohs(adresse_locale.sin_port) /*ntohs(ptr_service->s_port) */);
				
		/*création de la socket */
		if((socket_descriptor = socket(AF_INET, SOCK_STREAM, 0)) < 0)
		{
			perror("erreur : impossible de créer la socket de connexion avec le client.\n");
			exit(1);
		}
		
		/*association du socket socket_descriptor à la structure de l'adresse adresse_locale */
		
		if((bind(socket_descriptor, (sockaddr*)(&adresse_locale), sizeof(adresse_locale))) < 0 )
		{
			perror("erreur : impossible de lier la socket à l'adresse de connexion.\n");
			exit(1);
		}
		
		/*initialisation de la file d'écoute*/
		listen(socket_descriptor, 5);
		
		/*attente des connexions et traitement de données reçues */
		for(;;){
		printf("plop 1\n");
			longueur_adresse_courante = sizeof(adresse_client_courant);
			printf("plop 2\n");
			/*adresse_client_courant = sera renseignée par accept via les infos du connect*/
			if((nouv_socket_descriptor = 
				accept(socket_descriptor,
					(sockaddr*)(&adresse_client_courant),
					&longueur_adresse_courante))
				< 0)
			{
				perror("erreur : impossible d'accepter la connexion avec le client.");
				exit(1);
			}
			printf("plop 3\n");
			if(pthread_create(&nouveau_client, NULL,
								prise_en_charge,
							  (void*) &nouv_socket_descriptor) < 0)
		    {
		    	perror("erreur : impossible de creer un nouveau thread.");
		    	exit(1);
		    }
			
/*			/*traitement du message*/
/*			printf("reception d'un message.\n.");*/
/*			renvoi(nouv_socket_descriptor);*/
			
/*			close(nouv_socket_descriptor);*/
		}
		close(socket_descriptor);
	return EXIT_SUCCESS;
}
