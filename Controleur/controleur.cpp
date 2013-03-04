#include "controleur.h"

Controleur::Controleur()
{
    client = new Client();
}

Controleur::~Controleur()
{
    delete client;
}

void Controleur::envoiPseudo(QString *p)
{
    client->envoiPseudo((char*)p->toStdString().c_str());
//    client->receptionInfo();
//    emit infoRecu();
}

void Controleur::envoiCommand(char *command)
{
    client->envoiCommand(command);
    client->receptionInfo();
    emit infoRecu(client->server);
}

void Controleur::connexion()
{
    client->connexion();
}

void Controleur::envoiValeurConnexion(const char* ad, int p)
{
    delete client->host;
    client->host = new char[strlen(ad)+1];
    strcpy(client->host,ad);
    client->port = p;
}

QString Controleur::getAdresse()
{
    return QString(client->host);
}

QString Controleur::getPort()
{
    return QString::number(client->port);
}
