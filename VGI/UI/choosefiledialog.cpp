//
//
// choosefiledialog.cpp
//
// Implementation de la classe choosefiledialog.h
//
// LE Viet Man
// 14/05/10
//
//

#include "UI/choosefiledialog.h"
#include "ui_choosefiledialog.h"

//
// Construction
//
ChooseFileDialog::ChooseFileDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ChooseFileDialog)
{
    ui->setupUi(this);
}

//
// Destruction
//
ChooseFileDialog::~ChooseFileDialog()
{
    delete ui;
}

void ChooseFileDialog::changeEvent(QEvent *e)
{
    QDialog::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

//
// Afficher une dialog pour choisir un fichier
// quand l'utilisateur clique sur le bouton Browser
//
void ChooseFileDialog::on_btnBrowser_clicked()
{
    QDir dir;

    // ouvrir la dialog pour choisir un fichier
    QString fileName = QFileDialog::getOpenFileName(this,
                                tr("Choose a file VGI config"),
                                dir.homePath(),
                                tr("All Files (*);;VGI Config Files (*.vgi);;VML Config Files (*.vml)"));

    if (!fileName.isEmpty())
        ui->txtPath->setText(fileName); // mettre le chemin au textbox
}

//
// Prendre le chemin du fichier vgi choisi par l'utilisateur
//
QString ChooseFileDialog::getPathVGI()
{
    if (ui->txtPath->text().length() == 0)
        return 0;
    return ui->txtPath->text();
}
