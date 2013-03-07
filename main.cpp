/* -----------------------------------------------
 Client a lancer après le serveur avec la commande:
 client <adresse-serveur> <message-a-transmettre>
 ------------------------------------------------*/

#include "UI/mainwindow.h"
#include <QApplication>
#include <QProcess>
#include <QObject>
#include <QTextCodec>

int main(int argc, char *argv[])
{

#if QT_VERSION < 0x050000
    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));
    QTextCodec::setCodecForTr(QTextCodec::codecForName("UTF-8"));
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));
#endif

    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}
