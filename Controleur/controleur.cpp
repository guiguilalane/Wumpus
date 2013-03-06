#include "controleur.h"

Controleur::Controleur()
{
    // Initialisation de la machine avec l'adresse locale
    host = new char[strlen("127.0.0.1")+1];
    strcpy(host,"127.0.0.1");
    // Initialisation du port
    port = 5000;

    // Initialisation de la structure des datas
    fromServerInitialisation(&server);
}

Controleur::~Controleur()
{
}

void Controleur::envoiPseudo(QString *p)
{
    envoiPseudoClient((char*)p->toStdString().c_str(), socket_descriptor, server, dispStruc);
}

void Controleur::envoiCommand(char *command)
{
    envoiCommandClient(socket_descriptor, command);
    if (command == (char*)"quit"){
        /* Le joueur a quitter le jeu */
//        close(socket_descriptor);
//        deconnexionServeur(socket_descriptor);
        connect_ = false;
        printf("Connexion avec le serveur fermee, fin du programme. \n");
    }
//    receptionInfoClient(socket_descriptor, &server, &dispStruc);
//    emit infoRecu(&server);
}

void Controleur::connexion()
{
    connexionClient(&socket_descriptor, ptr_host, host, adresse_locale, port);
    connect_ = true;
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
        emit cont->infoRecu(&(cont->server));
    }
    return NULL;
}
