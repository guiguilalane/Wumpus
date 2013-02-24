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
}

void Controleur::envoiCommand(char *command)
{
    client->envoiCommand(command);
}

void Controleur::connexion()
{
    client->connexion();
}
