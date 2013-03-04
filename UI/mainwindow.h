#ifndef MAINWINDOW_H
#define MAINWINDOW_H


#include <QMainWindow>
#include <QGraphicsPixmapItem>
#include <QMessageBox>

#include "pseudo.h"
#include "option.h"
#include "../Controleur/controleur.h"
#include "../Model/viewer/mapRenderer.h"
#include "../Model/controleur/gestionnaireMap.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void loadCharacter(fromServer *s);

private slots:
    void acceptPseudo(QString *pseudo);

    void acceptValeur(QString *ad, QString *p);

    void on_shoot_clicked();

    void on_turnR_clicked();

    void on_turnL_clicked();

    void on_move_clicked();

    void on_down_clicked();

    void on_connect_clicked();

    void on_quit_clicked();

    void updateInfo(fromServer * s);

    void on_toolButton_clicked();

private:
    Ui::MainWindow *ui;
    QGraphicsScene * scene_;
    QGraphicsPixmapItem * mapItem_;
    QGraphicsPixmapItem * characterItem_;
    QGraphicsPixmapItem * treasureItem_;
    Controleur * cont_;
    bool pseudoRenseigne_;
    bool popupWK_;
    bool popupWF_;
    bool popupTF_;
    bool popupH_;

    Pseudo * pseudoDialog_;
    Option * optionDialog_;
};

#endif // MAINWINDOW_H
