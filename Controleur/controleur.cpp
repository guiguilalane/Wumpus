#include "controleur.h"

Controleur::Controleur()
{
    // Initialisation de la machine avec l'adresse locale
    host = new char[strlen("127.0.0.1")+1];
    strcpy(host,"127.0.0.1");
    // Initialisation du port
    port = 5000;
}

Controleur::~Controleur()
{
}

void Controleur::envoiPseudo(QString *p)
{
    envoiPseudoClient((char*)p->toStdString().c_str(), socket_descriptor);
}

void Controleur::envoiCommand(char *command)
{
    envoiCommandClient(socket_descriptor, command);
    if (command == (char*)"quit"){
        /* Le joueur a quitter le jeu */
        connect_ = false;
        pthread_cancel(listener);
        deconnexionServeur(socket_descriptor);
        printf("Connexion avec le serveur fermee, fin du programme. \n");
    }
    else if (command == (char*)"down")
    {
        emit clearMap();
        emit initMap(&server);
    }
}

void Controleur::connexion()
{
    // Initialisation de la structure des datas
    fromServerInitialisation(&server);
    connexionClient(&socket_descriptor, ptr_host, host, adresse_locale, port);
    connect_ = true;
    std::cout << "Sock: " << socket_descriptor << std::endl;
    // Lancer un thread
    if(pthread_create(&listener, NULL, Controleur::ecouter, (void*) this) < 0)
    {
        exit(1);
    }
}

void Controleur::envoiValeurConnexion(const char* ad, int p)
{
    delete host;
    host = new char[strlen(ad)+1];
    strcpy(host,ad);
    port = p;
}

QString Controleur::getAdresse()
{
    return QString(host);
}

QString Controleur::getPort()
{
    return QString::number(port);
}

void * Controleur::ecouter(void * arg)
{
    Controleur * cont = ( Controleur * ) arg;
    while(true){
        receptionInfoClient(cont->socket_descriptor, &(cont->server), &(cont->dispStruc));
        emit cont->infoRecu(&(cont->server), &(cont->dispStruc));
    }
    return NULL;
}
