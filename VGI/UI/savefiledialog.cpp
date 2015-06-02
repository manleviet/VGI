//
//
// savefiledialog.cpp
//
// Implementation de la classe savefiledialog.h
//
// LE Viet Man
// 18/06/10
//
//

#include "UI/savefiledialog.h"
#include "ui_savefiledialog.h"

//
// Construction
//
SaveFileDialog::SaveFileDialog(Util::VGI_VERSION version, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SaveFileDialog)
{
    ui->setupUi(this);
    this->version = version;

    if (version == Util::VGI20)
    {
        ui->rbtSimple->setChecked(true);
        ui->rbtComplet->setEnabled(false);
    }
}

//
// Destruction
//
SaveFileDialog::~SaveFileDialog()
{
    delete ui;
}

void SaveFileDialog::changeEvent(QEvent *e)
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
// Choisir un chemin
//
void SaveFileDialog::on_btnBrowse_clicked()
{
    QDir dir;

    // ouvrir la dialog pour choisir un fichier
    QString fileName = QFileDialog::getSaveFileName(this,
                                tr("Choose a file VGI config"),
                                dir.homePath(),
                                tr("All Files (*);;VGI Config Files (*.vgi);;VML Config Files (*.vml)"));

    if (ui->rbtTLP->isChecked())
    {
        fileName += ".tlp";
    }
    else
    {
        fileName += ".vml";
    }

    if (!fileName.isEmpty())
        ui->txtPath->setText(fileName); // mettre le chemin au textbox
}

//
// Le fichier enregistre est VML ?
// @result : true, false
//
bool SaveFileDialog::isVML()
{
    return ui->rbtVML->isChecked();
}

//
// Le fichier enregistrer est TLP ?
// @result : true, false
//
bool SaveFileDialog::isTLP()
{
    return ui->rbtTLP->isChecked();
}

//
// Enregistrer le simple graphe ?
// @result : true, false
//
bool SaveFileDialog::isSimple()
{
    return ui->rbtSimple->isChecked();
}

//
// Enregistrer le graphe complet
// @result : true, false
//
bool SaveFileDialog::isComplet()
{
    return ui->rbtComplet->isChecked();
}

//
// Prendre le chemin choisi
//
QString SaveFileDialog::getPath()
{
    if (ui->txtPath->text().length() == 0)
        return 0;
    return ui->txtPath->text();
}

void SaveFileDialog::on_rbtVML_toggled(bool checked)
{
    ui->rbtComplet->setEnabled(false);
    ui->rbtSimple->setEnabled(false);
    if (version == Util::VGI20)
    {
        ui->rbtSimple->setChecked(true);
        ui->rbtComplet->setEnabled(false);
    }

    if (ui->txtPath->text().length() == 0) return;

    if (checked)
    {
        QString path = ui->txtPath->text();

        ui->txtPath->setText(Util::changeExtOfFile(path, ".vml"));
    }
}

void SaveFileDialog::on_rbtTLP_toggled(bool checked)
{
    ui->rbtComplet->setEnabled(true);
    ui->rbtSimple->setEnabled(true);
    if (version == Util::VGI20)
    {
        ui->rbtSimple->setChecked(true);
        ui->rbtComplet->setEnabled(false);
    }

    if (ui->txtPath->text().length() == 0) return;

    if (checked)
    {
        QString path = ui->txtPath->text();

        ui->txtPath->setText(Util::changeExtOfFile(path, ".tlp"));
    }
}
