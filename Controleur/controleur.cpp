#include "controleur.h"

Controleur::Controleur()
{
    //    client = new Client();

    // Initialisation de la machine avec l'adresse locale
    host = new char[strlen("127.0.0.1")+1];
    strcpy(host,"127.0.0.1");
    // Initialisation du port
    port = 5000;

    // Initialisation de la structure des datas
    server = (fromServer*) malloc(sizeof(fromServer));
    fromServerInitialisation(server);
}

Controleur::~Controleur()
{
    //    delete client;
}

void Controleur::envoiPseudo(QString *p)
{
    /*client->*/envoiPseudoClient((char*)p->toStdString().c_str(), socket_descriptor);
}

void Controleur::envoiCommand(char *command)
{
    /*client->*/envoiCommandClient(socket_descriptor, command);
    if (command == (char*)"quit"){
        /* Le joueur a quitter le jeu */
        deconnexionServeur(socket_descriptor);
        connect_ = false;
        printf("Connexion avec le serveur fermee, fin du programme. \n");
    }
    /*client->*/receptionInfoClient(socket_descriptor, server);
    emit infoRecu(/*client->*/server);
}

void Controleur::connexion()
{
    /*client->*/connexionClient(&socket_descriptor, ptr_host, host, adresse_locale);
    connect_ = true;
}

void Controleur::envoiValeurConnexion(const char* ad, int p)
{
    delete /*client->*/host;
    /*client->*/host = new char[strlen(ad)+1];
    strcpy(/*client->*/host,ad);
    /*client->*/port = p;
}

QString Controleur::getAdresse()
{
    return QString(/*client->*/host);
}

QString Controleur::getPort()
{
    return QString::number(/*client->*/port);
}
