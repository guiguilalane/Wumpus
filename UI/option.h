#ifndef OPTION_H
#define OPTION_H

#include <QDialog>

namespace Ui {
class Option;
}

class Option : public QDialog
{
    Q_OBJECT
    
public:
    explicit Option(QWidget *parent = 0);
    ~Option();

    void putValeur(QString ad, QString p);

signals:
    void newValeur(QString * ad, QString * p);
    
private slots:
    void on_buttonBox_accepted();

private:
    Ui::Option *ui;
};

#endif // OPTION_H
