#ifndef PSEUDO_H
#define PSEUDO_H

#include <QDialog>

namespace Ui {
class Pseudo;
}

class Pseudo : public QDialog
{
    Q_OBJECT
    
public:
    explicit Pseudo(QWidget *parent = 0);
    ~Pseudo();

signals:
    void newPseudo(QString * pseudo);
    
private slots:
    void on_buttonBox_accepted();

private:
    Ui::Pseudo *ui;
};

#endif // PSEUDO_H
