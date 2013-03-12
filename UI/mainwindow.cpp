#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    cont_ = new Controleur();
    connect(cont_,SIGNAL(infoRecu(fromServer *, dispatchStruct *)),this,SLOT(updateInfo(fromServer *, dispatchStruct *)));
    connect(cont_,SIGNAL(initMap(fromServer *)),this,SLOT(initialisation(fromServer*)));
    connect(cont_,SIGNAL(clearMap()),this,SLOT(clearScene()));
    connect(qApp,SIGNAL(lastWindowClosed()),this,SLOT(quitWindow()));

    pseudoRenseigne_ = false;
    treasureDisplay_ = false;

    ui->setupUi(this);

    // Désactivation des boutons sauf celui de connexion
    ui->connect->setEnabled(true);
    ui->turnL->setEnabled(false);
    ui->turnR->setEnabled(false);
    ui->move->setEnabled(false);
    ui->shoot->setEnabled(false);
    ui->down->setEnabled(false);
    ui->quit->setEnabled(false);

    // Initialization of the scene and its components.
    // TODO : A voir pour que la redimensionnement soit automatique
    scene_ = new QGraphicsScene();
    mapItem_ = new QGraphicsPixmapItem;
    characterItem_ = new QGraphicsPixmapItem;
    treasureItem_ = new QGraphicsPixmapItem;
    ui->view->setScene(scene_);

    ui->widgetInd->setVisible(false);
    ui->widgetScore->setVisible(false);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete cont_;
}

void MainWindow::loadCharacter(fromServer * s)
{
    // On change l'image du personnage selon la direction dans laquelle il regarde
    bool boutonMoveActif = true;
    QString dir = ":/Pictures/Pictures/derriere.png";
    if (s->dir == 0){
        dir = ":/Pictures/Pictures/derriere.png";
        if (s->playerPosY == 0){
            boutonMoveActif = false;
        }
    }
    else if (s->dir == 2){
        dir = ":/Pictures/Pictures/face.png";
        if (s->playerPosY == 4){
            boutonMoveActif = false;
        }
    }
    else if (s->dir == 1){
        dir = ":/Pictures/Pictures/right.png";
        if (s->playerPosX == 4){
            boutonMoveActif = false;
        }
    }
    else if (s->dir == 3){
        dir = ":/Pictures/Pictures/left.png";
        if (s->playerPosX == 0){
            boutonMoveActif = false;
        }
    }
    characterItem_->setPixmap(QPixmap(dir).scaled(32,32));
    characterItem_->setZValue(3);
    // On repositionne le personnage sur la carte
    characterItem_->setPos(s->playerPosX*33+1,s->playerPosY*33+1);
    // En fonction emplacement personnage désactivé les boutons
    ui->move->setEnabled(boutonMoveActif);
    // Si le personnage est sur l'échelle et qu'il a trouvé le trésor
    ui->down->setEnabled(s->playerPosX==0 && s->playerPosY==4 && s->tresureFind);
}

void MainWindow::acceptPseudo(QString* pseudo)
{
    delete pseudoDialog_;
    cont_->envoiPseudo(pseudo);
    pseudo_ = pseudo;
    pseudoRenseigne_ = true;
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
    // Connexion
    int r = cont_->connexion();

    if (r < 0)
    {
        // Afficher message
        QMessageBox msg;
        msg.setWindowTitle("Information");
        msg.setStandardButtons(QMessageBox::Ok);
        msg.setIcon(QMessageBox::Critical);
        msg.setText("<center> Connexion au serveur impossible ! </center>");
        msg.exec();
    }
    else
    {
        // Envoi du pseudo
        if (!pseudoRenseigne_){
            pseudoDialog_ = new Pseudo(this);
            pseudoDialog_->show();
            connect(pseudoDialog_, SIGNAL(newPseudo(QString *)), this, SLOT(acceptPseudo(QString *)));
        }
        else
        {
            cont_->envoiPseudo(pseudo_);
        }

        ui->statusBar->setStatusTip("Vous venez d'entrer dans le temple de la mort. Vous n'en ressortirez pas vivant !!!");

        initialisation(&cont_->server);
        ui->widgetInd->setVisible(true);
        ui->widgetScore->setVisible(true);
    }
}

void MainWindow::initialisation(fromServer *s)
{
    // Display the map and composant
    mapItem_->setPixmap(QPixmap(":/Pictures/Pictures/carte.png").scaled(166,166));
    mapItem_->setPos(0,0);
    characterItem_->setPixmap(QPixmap(":/Pictures/Pictures/derriere.png").scaled(32,32));
    characterItem_->setZValue(3);
    characterItem_->setPos(s->playerPosX*33+1,s->playerPosY*33+1);
    scene_->addItem(mapItem_);
    scene_->addItem(characterItem_);

    // TODO Add initialisation of sensor sans qu'auncune action soit effectuée
    ui->treasure->setVisible(s->besideTresure);
    ui->hole->setVisible(s->besideHole);
    ui->wumpus->setVisible(s->besideWumpus);

    // Booléean des popup initialisée à false
    popupWK_ = false;
    popupWF_ = false;
    popupTF_ = false;
    popupH_ = false;

    // Activation des boutons ou non
    ui->connect->setEnabled(false);
    ui->turnL->setEnabled(true);
    ui->turnR->setEnabled(true);
    ui->move->setEnabled(true);
    ui->shoot->setEnabled(true);
    ui->down->setEnabled(false);
    ui->quit->setEnabled(true);
    ui->option->setEnabled(false);
}

void MainWindow::clearScene()
{
    // On vide la scene
    scene_->removeItem(mapItem_);
    scene_->removeItem(characterItem_);
    if(treasureDisplay_){
        scene_->removeItem(treasureItem_);
        treasureDisplay_ = false;
    }
}

void MainWindow::quitWindow()
{
    if (cont_->getConnect_()){
        cont_->envoiCommand((char*)"quit");
    }
}

void MainWindow::on_quit_clicked()
{
    cont_->envoiCommand((char*)"quit");

    clearScene();

    // Désactivation des boutons
    ui->connect->setEnabled(true);
    ui->turnL->setEnabled(false);
    ui->turnR->setEnabled(false);
    ui->move->setEnabled(false);
    ui->shoot->setEnabled(false);
    ui->down->setEnabled(false);
    ui->quit->setEnabled(false);
    ui->option->setEnabled(true);

    ui->statusBar->setStatusTip("");
    ui->widgetInd->setVisible(false);
    ui->widgetScore->setVisible(false);
}

void MainWindow::updateInfo(fromServer * s, dispatchStruct *d)
{
    // Fenêtre message
    QMessageBox msg;
    msg.setWindowTitle("Information");
    msg.setStandardButtons(QMessageBox::Ok);
    ui->score->setText(QString::number(s->score));
    if (d->type == 2)
    {
        if (strcmp(d->structure,(char*)"down")==0){
            msg.setText("<center> Un autre joueur a trouvé le trésor et à changer d'étage. <br/> Vous allez vous aussi passer à l'étage inférieur </center>");
        }
        else if(strcmp(d->structure,(char*)"allDead") == 0){
            msg.setText("<center> Tout le monde est mort ou est tombé dans un trou. <br/> Un changement d'étage va avoir lieu </center>");
        }
        msg.exec();

        clearScene();
        initialisation(s);
    }
    else
    {
        loadCharacter(s);
        if (s->fallInHole && !popupH_){
            enableButtonPlay();
            popupH_ = true;
            msg.setText("<center> Vous venez de tomber dans le trou ! Vous perdez 30 points ! <br/> Attender qu'un changement d'étage soit déclenché ! </center>");
            msg.setIconPixmap(QPixmap(":/Pictures/Pictures/hole.jpg").scaled(135,186));
            msg.exec();
        }
        if (s->wumpusFind && !popupWF_){
            enableButtonPlay();
            popupWF_ = true;
            msg.setText("<center> Vous venez de rencontrer le Wumpus ! <br/> Vous en êtes pas sortis vivant et vous perdez 50 points ! <br/> Attender qu'un changement d'étage soit déclenché !  </center>");
            msg.setIconPixmap(QPixmap(":/Pictures/Pictures/wumpusColor.png").scaled(135,186));
            msg.exec();
        }
        if (s->tresureFind && !popupTF_){
            msg.setText("<center> Vous venez de trouver le trésor ! <br/> Gagnez vos 100 points en accédant le premier à l'echelle ! </center>");
            msg.setIconPixmap(QPixmap(":/Pictures/Pictures/treasure.png").scaled(143,130));
            msg.exec();
            treasureDisplay_ = true;
            treasureItem_->setPixmap(QPixmap(":/Pictures/Pictures/treasureCarte.png").scaled(32,32));
            treasureItem_->setPos(s->tresurePosX*33+1,s->tresurePosY*33+1);
            scene_->addItem(treasureItem_);
            popupTF_ = true;
        }
        if (s->wumpusKill && !popupWK_){
            msg.setText("<center> Vous venez de tuer le Wumpus ! <br/> Félicitation, vous gagnez 20 points ! </center>");
            msg.setIconPixmap(QPixmap(":/Pictures/Pictures/wumpusColor.png").scaled(143,130));
            msg.exec();
            popupWK_ = true;
        }
        // On affiche les senseurs sur l'IHM
        // TODO Récupérer les bonnes valeurs
        ui->treasure->setVisible(s->besideTresure);
        ui->hole->setVisible(s->besideHole);
        ui->wumpus->setVisible(s->besideWumpus);
    }
    // TODO Afficher les scores de tous les joueurs + PopUp lors du quit ?
}

// TODO Faire une fonction d'initialisation des sensor sans que la 1ère commande soit effectué (à priori, effectué)

void MainWindow::enableButtonPlay()
{
    ui->turnL->setEnabled(false);
    ui->turnR->setEnabled(false);
    ui->move->setEnabled(false);
    ui->shoot->setEnabled(false);
    ui->down->setEnabled(false);
}

void MainWindow::on_option_clicked()
{
    QString ad = cont_->getAdresse();
    QString p = cont_->getPort();
    optionDialog_ = new Option(this);
    optionDialog_->putValeur(ad, p);
    optionDialog_->show();
    connect(optionDialog_, SIGNAL(newValeur(QString *, QString *)), this, SLOT(acceptValeur(QString *, QString *)));
}

void MainWindow::acceptValeur(QString* ad, QString * p)
{
    cont_->envoiValeurConnexion(ad->toStdString().c_str(), p->toInt());
    delete optionDialog_;
}
