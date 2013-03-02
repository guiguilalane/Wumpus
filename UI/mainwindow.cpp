#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    cont_ = new Controleur();
    connect(cont_,SIGNAL(infoRecu()),this,SLOT(updateInfo()));

    ui->setupUi(this);

    // Désactiver le bouton connexion
    ui->connect->setEnabled(false);

    // Fenêtre pseudo
    pseudoDialog_ = new Pseudo(this);
    pseudoDialog_->show();
    connect(pseudoDialog_, SIGNAL(newPseudo(QString *)), this, SLOT(acceptPseudo(QString *)));

    // Initialization of the scene and its components.
    // TODO : A voir pour que la redimensionnement soit automatique
    scene_ = new QGraphicsScene(0,0,395,331,this);
    mapItem_ = new QGraphicsPixmapItem;
    characterItem_ = new QGraphicsPixmapItem;

    characterItem_->setPixmap(QPixmap(":/Pictures/Pictures/derriere.png").scaled(32,32));
    // A revoir position qui ne marche pas
    characterItem_->setPos(0,4*32);

    scene_->addItem(mapItem_);
    scene_->addItem(characterItem_);
    ui->view->setSceneRect(0,0,scene_->width(),scene_->height());

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
    mapItem_->setPixmap(renderer.createRendu()->pixmap().scaled(160,160));
}

void MainWindow::loadCharacter()
{
    // En fonction de l'orientation du personnage changer l'icone
    characterItem_->setPixmap(QPixmap(":/Pictures/Pictures/derriere.png").scaled(32,32));
    // Mettre pos
    //characterItem_->setPos(0,4);
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

void MainWindow::updateInfo()
{
    std::cout << "info reçu" << std::endl;
//    loadCharacter();
    // Changer le personnage et son emplacement
    // Afficher la carte si elle est trouvée
    // Lancer pop-up avec carte trouvée, tomber trou, wumpus tué, wumpus nous a tué
    // En fonction emplacement personnage désactivé les boutons
}

// TODO Afficher la carte
// TODO Récupérer la position du joueur:
//  - Desactiver les boutons quand le personnage ne peut pas avancer dans la direction
//  - Afficher le joueur
// TODO Afficher le tresor sur la carte s'il est trouver
// TODO Fenêtre popup trésor trouver, tomber trou ou tomber wumpus
// TODO Afficher score lors du quit
// TODO Afficher les sensors
