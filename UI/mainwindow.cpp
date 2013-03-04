#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    cont_ = new Controleur();
    connect(cont_,SIGNAL(infoRecu(fromServer *)),this,SLOT(updateInfo(fromServer *)));

    ui->setupUi(this);

    // Désactiver le bouton connexion
    ui->connect->setEnabled(false);
    ui->down->setEnabled(false);

    // Fenêtre pseudo
    pseudoDialog_ = new Pseudo(this);
    pseudoDialog_->show();
    connect(pseudoDialog_, SIGNAL(newPseudo(QString *)), this, SLOT(acceptPseudo(QString *)));

    // Initialization of the scene and its components.
    // TODO : A voir pour que la redimensionnement soit automatique
    scene_ = new QGraphicsScene(0,0,395,331,this);
    mapItem_ = new QGraphicsPixmapItem;
    characterItem_ = new QGraphicsPixmapItem;
    treasureItem_ = new QGraphicsPixmapItem;

    characterItem_->setPixmap(QPixmap(":/Pictures/Pictures/derriere.png").scaled(32,32));
    // A revoir position qui ne marche pas
    characterItem_->setPos(0,4*32);

    scene_->addItem(mapItem_);
    scene_->addItem(characterItem_);
    ui->view->setSceneRect(0,0,scene_->width(),scene_->height());

//    mapLoader(QString("../Wumpus/Wumpus.tmx"));
}

MainWindow::~MainWindow()
{
    delete ui;
    delete cont_;
}

void MainWindow::mapLoader(QString file)
{
    MapRenderer renderer(GestionnaireMap::getInstance((char *)file.toStdString().c_str())->getMap());
    //    mapItem_->setPixmap(renderer.createRendu()->pixmap().scaled(160,160));
}

void MainWindow::loadCharacter(fromServer * s)
{
    // On change l'image du personnage selon la direction dans laquelle il regarde
    bool boutonMoveActif = true;
    QString dir;
    if (s->dir == 'n'){
        dir = ":/Pictures/Pictures/derriere.png";
        if (s->playerPosY == 0){
            boutonMoveActif = false;
        }
    }
    else if (s->dir == 's'){
        dir = ":/Pictures/Pictures/face.png";
        if (s->playerPosY == 4){
            boutonMoveActif = false;
        }
    }
    else if (s->dir == 'e'){
        dir = ":/Pictures/Pictures/left.png";
        if (s->playerPosX == 0){
            boutonMoveActif = false;
        }
    }
    else if (s->dir == 'o'){
        dir = ":/Pictures/Pictures/right.png";
        if (s->playerPosX == 4){
            boutonMoveActif = false;
        }
    }
    characterItem_->setPixmap(QPixmap(dir).scaled(32,32));
    // On repositionne le personnage sur la carte
    characterItem_->setPos(s->playerPosX,s->playerPosY);
    // En fonction emplacement personnage désactivé les boutons
    ui->move->setEnabled(boutonMoveActif);
    // Si le personnage est sur l'échelle et qu'il a trouvé le trésor
    if (s->playerPosX==0 && s->playerPosY==4 && s->tresureFind){
        ui->down->setEnabled(true);
    }
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
    ui->down->setEnabled(false);
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

void MainWindow::updateInfo(fromServer * s)
{
    loadCharacter(s);
    // Fenêtre message
    QMessageBox msg;
    msg.setWindowTitle("Information");
    msg.setStandardButtons(QMessageBox::Ok);
    if (s->fallInHole){
        msg.setText("<center> Vous venez de tomber dans le trou !</center>");
        msg.setIconPixmap(QPixmap(":/Pictures/Pictures/hole.jpg").scaled(135,186));
        msg.exec();
        // TODO : Quitter le nivo ?
    }
    if (s->wumpusFind){
        msg.setText("<center> Vous venez de rencontrer le Wumpus ! <br/> Vous en êtes pas sortis vivant ! </center>");
        msg.setIconPixmap(QPixmap(":/Pictures/Pictures/wumpusColor.png").scaled(135,186));
        msg.exec();
        // TODO : Quitter le nivo ?
    }
    // TODO A revoir les nb points
    if (s->tresureFind){
        msg.setText("<center> Vous venez de trouver le trésor ! <br/> Gagnez vos 100 points en accédant le premier à l'echelle ! </center>");
        msg.setIconPixmap(QPixmap(":/Pictures/Pictures/treasure.png").scaled(143,130));
        msg.exec();
        // TODO A vérifier s'il s'affiche au bon endroit
        treasureItem_->setPixmap(QPixmap(":/Pictures/Pictures/derriere.png").scaled(32,32));
        treasureItem_->setPos(s->tresurePosX,s->tresurePosY);
        scene_->addItem(treasureItem_);
    }
    if (s->wumpusKill){
        msg.setText("<center> Vous venez de tuer le Wumpus ! <br/> Félicitation, vous gagnez 20 points ! </center>");
        msg.setIconPixmap(QPixmap(":/Pictures/Pictures/wumpusColor.png").scaled(143,130));
        msg.exec();
    }
    // On affiche les senseurs sur l'IHM
    ui->treasure->setVisible(s->besideTresure);
    ui->hole->setVisible(s->besideHole);
    ui->wumpus->setVisible(s->besideWumpus);
    // TODO mettre a jour les scores --> Lors du quit dans une popup et tout le tps dans la fenêtre - Son score et celui de l'autre joueur
    // TODO quand un joueur change de nivo avertir tous les autres joueurs
}


// TODO Afficher la carte
// TODO Afficher score lors du quit et dans la case à coté ainsi que celui de tous les joueurs
// TODO Au lancement --> Adresse + port ? Comment on fait ? --> Argument, rentrer par l'utilisateur ???
// A vérifier:
// TODO Récupérer la position du joueur:
//  - Desactiver les boutons quand le personnage ne peut pas avancer dans la direction
//  - Afficher le joueur
//  - Dans le bon sens
// TODO Afficher le tresor sur la carte s'il est trouver
// TODO Fenêtre popup trésor trouver, tomber trou ou tomber wumpus
// TODO Afficher les sensors
