/* -----------------------------------------------
 Client a lancer après le serveur avec la commande:
 client <adresse-serveur> <message-a-transmettre>
 ------------------------------------------------*/

#include "UI/mainwindow.h"
#include <QApplication>
#include <QProcess>
#include <QObject>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    
    return a.exec();
}
