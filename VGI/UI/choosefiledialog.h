//
//
// choosefiledialog.h
//
// C'est une classe d'interface qui sert a choisir un fichier de configuration de VGI
//
// LE Viet Man
// 14/05/10
//
//

#ifndef CHOOSEFILEDIALOG_H
#define CHOOSEFILEDIALOG_H

#include <QDialog>
#include <QString>
#include <QFileDialog>

namespace Ui {
    class ChooseFileDialog;
}

class ChooseFileDialog : public QDialog {
    Q_OBJECT
public:
    ChooseFileDialog(QWidget *parent = 0);
    ~ChooseFileDialog();

    QString getPathVGI();

protected:
    void changeEvent(QEvent *e);

private:
    Ui::ChooseFileDialog *ui;

private slots:
    void on_btnBrowser_clicked();
};

#endif // CHOOSEFILEDIALOG_H
