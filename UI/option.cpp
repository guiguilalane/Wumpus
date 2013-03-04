#include "option.h"
#include "ui_option.h"

Option::Option(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Option)
{
    ui->setupUi(this);
}

Option::~Option()
{
    delete ui;
}

void Option::putValeur(QString ad, QString p)
{
    ui->lineEditAdresse->setText(ad);
    ui->lineEditPort->setText(p);
}

void Option::on_buttonBox_accepted()
{
    emit newValeur(new QString(ui->lineEditAdresse->text()), new QString(ui->lineEditPort->text()));
}
