//
//
// levelscene.cpp
//
// Implementation de la classe levelscene.h
//
// LE Viet Man
// 10/06/10
//
//

#include "Scene/levelscene.h"

const qreal Pi = 3.14159265358979323846264338327950288419717;
const qreal TwoPi = 2.0 * Pi;

//
// Construction
//
LevelScene::LevelScene(int id, QGraphicsItem *parent)
    :QGraphicsEllipseItem(parent)
{
    this->id = id;
    this->added = false;
    this->idFocus = 0;
    this->lineLevel = 0;
}

int LevelScene::getId()
{
    return this->id;
}

bool LevelScene::isAdded()
{
    return this->added;
}

void LevelScene::setAdded(bool added)
{
    this->added = added;
}

bool LevelScene::canChangeToClusters()
{
    return ((images.at(idFocus)->getImage()->getType() == Image::IMAGE)
            && (images.at(0)->getImage()->getType() == Image::IMAGE)
            && (images.at(images.length() - 1)->getImage()->getType() == Image::IMAGE));
}

bool LevelScene::canChangeToImages()
{
    return ((images.at(idFocus)->getImage()->getType() == Image::IMAGE)
            && ((images.at(0)->getImage()->getType() == Image::CLUSTER)
                || (images.at(images.length() - 1)->getImage()->getType() == Image::CLUSTER)));
}

ImageScene *LevelScene::getFocus()
{
    return images.at(idFocus);
}

void LevelScene::addFocus(int id)
{
    this->idFocus = id;
}

//
// Ajouter une image au niveau
// @param:
//      ImageScene *image : une image
//
void LevelScene::addImage(ImageScene *image)
{
    this->images << image;
    connect(image, SIGNAL(clickedImage(ImageScene*)),
            this, SLOT(clickedImage(ImageScene*)));
    connect(image, SIGNAL(doubleClickedImage(ImageScene*)),
            this, SLOT(doubleClickedImage(ImageScene*)));
}

//
// Ajouter un lien au niveau
//
void LevelScene::addLine(LineScene *line)
{
    this->lines << line;
}

//
// Ajouter un lien entre deux niveau
//
void LevelScene::addLineLevel(LineLevelScene *lineLevel)
{
    this->lineLevel = lineLevel;
}

//
// Afficher le niveau
//
void LevelScene::showLevel(GraphScene *scene)
{
    doLayout(); // calculer les coordonnees des elements
    // mettre les images et les liens au scene
    for (int i = 0; i < images.length(); i++)
    {
        if (images.at(i)->isAdded()) continue;
        images.at(i)->setZValue(this->zValue() + i + 2);
        scene->addItem(images.at(i));
        images.at(i)->setAdded(true);
    }
    for (int i = 0; i < lines.length(); i++)
    {
        if (lines.at(i)->isAdded()) continue;
        lines.at(i)->setZValue(this->zValue() + 1);
        scene->addItem(lines.at(i));
        lines.at(i)->setAdded(true);
    }
}

//
// Effacer toutes les elements du niveau dans le scene
//
void LevelScene::removeItems(GraphScene *scene)
{
    for (int i = 0; i < images.length(); i++)
    {
        scene->removeItem(images.at(i));
    }
    this->images.clear();
    for (int i = 0; i < lines.length(); i++)
    {
        scene->removeItem(lines.at(i));
    }
    this->lines.clear();
}

//
// Override la fonction paint
//
void LevelScene::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->setRenderHint(QPainter::Antialiasing, true);
    QGraphicsEllipseItem::paint(painter, option, widget);
}

//
// Override la fonction itemChange pour mettre a jour les liens
//
QVariant LevelScene::itemChange(GraphicsItemChange change, const QVariant &value)
{
    Q_UNUSED(change);
    if (this->lineLevel != 0)
    {
        this->lineLevel->updatePosition();
    }
    return value;
}

//
// Resoudre l'evenement clique sur une image du niveau
// @param:
//      ImageScene *image : une image
//
void LevelScene::clickedImage(ImageScene *image)
{
    if (this->id != 0) return; // ne pas faire clique si ne pas etre le niveau 0

    // le niveau des clusters, faire changer le focus
    changeFocus(image);
}

//
// Resoudre l'evenement double clique sur une image du niveau
// @param:
//      ImageScene *image : une image
//
void LevelScene::doubleClickedImage(ImageScene *image)
{
    emit doubleClickedImage(id, image, images.at(idFocus));

    if (id == 0)
    {
        changeFocus(image);
    }
}

//
// Calculer les coordonnees pour le focus et les elements
// Implementation l'algorithme Radial
//
void LevelScene::doLayout()
{
    QRectF r = this->rect();

    if (images.length() == 1)
    {   // s'il y a seulement le focus
        qreal radii0 = r.width() / 3;
        // centre du niveau
        qreal xLevel = r.x() + (r.width() / 2);
        qreal yLevel = r.y() + (r.height() / 2);

        // avec le focus
        // scale la taille du focus
        ImageScene *focus = images.at(idFocus);
        QPixmap pixmap = focus->getImage()->getPixmap();
        qreal orgWidth = pixmap.width();
        qreal orgHeight = pixmap.height();

        qreal scale = radii0 / radiiImage(orgWidth, orgHeight);

        qreal newWidth = orgWidth * scale;
        qreal newHeight = orgHeight * scale;

        focus->setPixmap(pixmap.scaled(newWidth, newHeight));

        focus->setX(xLevel - (newWidth / 2));
        focus->setY(yLevel - (newHeight / 2));
    }
    else
    {   // il y a des elements
        // le rayon du noeud
        qreal radii0 = r.width() / 4;
        qreal radii1 = radii0 / 2;

        // le rayon du niveau 1
        qreal lRadii = radii0 + radii1;

        // calculer les positions angulaires
        int numImage = this->images.length() - 1;
        qreal mAngle = TwoPi / (numImage);

        qreal angle[this->images.length()];

        for (int i = 0; i < images.length(); i++)
        {
            angle[i] = i * mAngle;
        }

        // METTRE LES COORDONNEES AUX NOUEDS
        // centre du niveau
        qreal xLevel = r.x() + (r.width() / 2);
        qreal yLevel = r.y() + (r.height() / 2);

        // avec le focus
        // scale la taille du focus
        ImageScene *focus = images.at(idFocus);
        QPixmap pixmap = focus->getImage()->getPixmap();
        qreal orgWidth = pixmap.width();
        qreal orgHeight = pixmap.height();

        qreal scale = radii0 / radiiImage(orgWidth, orgHeight);

        qreal newWidth = orgWidth * scale;
        qreal newHeight = orgHeight * scale;

        focus->setPixmap(pixmap.scaled(newWidth, newHeight));

        focus->setX(xLevel - (newWidth / 2));
        focus->setY(yLevel - (newHeight / 2));

        // les autres images
        int countAngle = -1;
        for (int i = 0; i < images.length(); i++)
        {
            if (i == idFocus) continue;
            // scale
            ImageScene *image = images.at(i);
            QPixmap pixmap = image->getImage()->getPixmap();
            qreal orgWidth = pixmap.width();
            qreal orgHeight = pixmap.height();

            qreal scale = radii1 / radiiImage(orgWidth, orgHeight);
            qreal newWidth = orgWidth * scale;
            qreal newHeight = orgHeight * scale;

            image->setPixmap(pixmap.scaled(newWidth, newHeight));

            // mettre les coordonnees
            countAngle++;
            qreal newX = lRadii * cos(angle[countAngle]);
            qreal newY = lRadii * sin(angle[countAngle]);

            image->setX(xLevel + newX - (newWidth / 2));
            image->setY(yLevel + newY - (newHeight / 2));
            image->update();
        }

        this->update();
        for (int i = 0; i < lines.length(); i++)
        {
            this->lines.at(i)->addFocus(images.at(idFocus));
        }
    }
}

//
// Calculer la moitie de la diagonale d'une image
// @param:
//      qreal width : la largeur
//      qreal height : la hauteur
//
qreal LevelScene::radiiImage(qreal width, qreal height)
{
    return sqrt(width * width + height * height) / 2;
}

//
// Changer le focus
// @param:
//      ImageScene *image : un nouveau focus
//
void LevelScene::changeFocus(ImageScene *image)
{
    int index = images.indexOf(image);
    if (idFocus != index)
    {
        this->idFocus = index;
        doLayout();
    }
}
