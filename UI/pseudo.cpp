#include "pseudo.h"
#include "ui_pseudo.h"

Pseudo::Pseudo(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Pseudo)
{
    ui->setupUi(this);
}

Pseudo::~Pseudo()
{
    delete ui;
}

void Pseudo::on_buttonBox_accepted()
{
    emit newPseudo(new QString(ui->lineEditPseudo->text()));
}
