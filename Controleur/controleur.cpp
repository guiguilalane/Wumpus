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
    emit infoRecu();
}

void Controleur::connexion()
{
    client->connexion();
}
