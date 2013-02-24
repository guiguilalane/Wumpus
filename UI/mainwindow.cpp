#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    cont_ = new Controleur();

    ui->setupUi(this);

    // Désactiver le bouton connexion
    ui->connect->setEnabled(false);

    // Fenêtre pseudo
    pseudoDialog_ = new Pseudo(this);
    pseudoDialog_->show();
    connect(pseudoDialog_, SIGNAL(newPseudo(QString *)), this, SLOT(acceptPseudo(QString *)));

    // Initialization of the scene and its components.
    _scene = new QGraphicsScene;
    _mapItem = new QGraphicsPixmapItem;
    _characterItem = new QGraphicsPixmapItem;

    _scene->addItem(_mapItem);
    _scene->addItem(_characterItem);
    ui->view->setScene(_scene);

//    mapLoader(QString("../Wumpus.tmx"));
}

MainWindow::~MainWindow()
{
    delete ui;
    delete cont_;
}

void MainWindow::mapLoader(QString file)
{
    MapRenderer renderer(GestionnaireMap::getInstance((char *)file.toStdString().c_str())->getMap());
    _mapItem->setPixmap(renderer.createRendu()->pixmap()/*.scaled(600,600)*/);
}

void MainWindow::acceptPseudo(QString* pseudo)
{
    delete pseudoDialog_;
    cont_->envoiPseudo(pseudo);
}

void MainWindow::on_shoot_clicked()
{
    cont_->envoiCommand((char*)"shot");
    ui->shoot->setEnabled(false);
}

void MainWindow::on_turnR_clicked()
{
    cont_->envoiCommand((char*)"turnright");
}

void MainWindow::on_turnL_clicked()
{
    cont_->envoiCommand((char*)"turnleft");
}

void MainWindow::on_move_clicked()
{
    cont_->envoiCommand((char*)"move");
}

void MainWindow::on_down_clicked()
{
    cont_->envoiCommand((char*)"down");
}

void MainWindow::on_connect_clicked()
{
    cont_->connexion();
    ui->connect->setEnabled(false);
    ui->turnL->setEnabled(true);
    ui->turnR->setEnabled(true);
    ui->move->setEnabled(true);
    ui->shoot->setEnabled(true);
    ui->down->setEnabled(true);
    ui->quit->setEnabled(true);
}

void MainWindow::on_quit_clicked()
{
    cont_->envoiCommand((char*)"quit");
    ui->connect->setEnabled(true);
    ui->turnL->setEnabled(false);
    ui->turnR->setEnabled(false);
    ui->move->setEnabled(false);
    ui->shoot->setEnabled(false);
    ui->down->setEnabled(false);
    ui->quit->setEnabled(false);
}

// TODO Afficher la carte
// TODO Récupérer la position du joueur:
//  - Desactiver les boutons quand le personnage ne peut pas avancer dans la direction
//  - Afficher le joueur
// TODO Afficher le tresor sur la carte s'il est trouver
// TODO Fenêtre popup trésor trouver, tomber trou ou tomber wumpus
// TODO Afficher score lors du quit
// TODO Afficher les sensors
