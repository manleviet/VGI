//
//
// savefiledialog.h
//
// C'est une classe d'interface qui sert a choisir
// le chemin du fichier enregistre
//
// LE Viet Man
// 18/06/10
//
//

#ifndef SAVEFILEDIALOG_H
#define SAVEFILEDIALOG_H

#include <QDialog>
#include <QString>
#include <QFileDialog>
#include "util.h"

namespace Ui {
    class SaveFileDialog;
}

class SaveFileDialog : public QDialog {
    Q_OBJECT
public:
    SaveFileDialog(Util::VGI_VERSION version, QWidget *parent = 0);
    ~SaveFileDialog();

    bool isVML();
    bool isTLP();
    bool isSimple();
    bool isComplet();

    QString getPath();

protected:
    void changeEvent(QEvent *e);

private:
    Ui::SaveFileDialog *ui;
    Util::VGI_VERSION version;

private slots:
    void on_rbtTLP_toggled(bool checked);
    void on_rbtVML_toggled(bool checked);
    void on_btnBrowse_clicked();
};

#endif // SAVEFILEDIALOG_H
