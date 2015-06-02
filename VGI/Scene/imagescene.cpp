//
//
// imagescene.cpp
//
// Implimentation de la classe imagescene.h
//
// LE Viet Man
// 10/06/10
//
//

#include "Scene/imagescene.h"

// les constantes pour dessiner trois permiere images d'un cluster
#define THUMB_SIZE	(120)
#define THUMB_HALF_SIZE	(60)
#define THUMB_STEP1	(50)
#define THUMB_STEP2	(100)

//
// Construction
//
ImageScene::ImageScene(Image *image, QGraphicsItem *parent)
    : QGraphicsPixmapItem(parent)
{
    this->image = image;
    this->added = false;

    // si image est un cluster ou sous-cluster
    if ((this->image->getType() == Image::CLUSTER)
        || (this->image->getType() == Image::SUB_CLUSTER))
    {
        // dessiner trois premiere images
        QString temp = createImageCluster(this->image);

        if (!this->image->isPixmapLoaded())
        {
            this->image->setPath(temp); // remettre le chemin de l'image
            this->image->loadPixmap(); // charger l'image
        }
    }
    else
    {
        if (!this->image->isPixmapLoaded())
        {
            this->image->loadPixmap(); // charger l'image
        }
    }

    this->setPixmap(this->image->getPixmap());

    setFlags(QGraphicsItem::ItemIsSelectable);
    this->setAcceptHoverEvents(true);

    myPolygon << QPointF(-100, -100) << QPointF(100, -100) << QPointF(100, 100)
                    << QPointF(-100, 100) << QPointF(-100, -100);
    this->hovered = false;
}

//
// Prendre l'objet Image
// @return: un objet Image
//
Image *ImageScene::getImage()
{
    return this->image;
}

//
// Deja ajouter a l'ecran ?
// @return: true - deja, false - sinon
//
bool ImageScene::isAdded()
{
    return this->added;
}

void ImageScene::setAdded(bool added)
{
    this->added = added;
}

//
// Contenir une image ?
// @param:
//      Image *image : une image
// @return: true - oui, false : non
//
bool ImageScene::hasImage(Image *image)
{
    return (this->image == image);
}

//
// Mettre une lien
// @param :
//      LineScene *line : un lien
//
void ImageScene::addLine(LineScene *line)
{
    this->lines << line;
}

//
// Override la fonction paint pour dessiner une image sur l'ecran
//
void ImageScene::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    QGraphicsPixmapItem::paint(painter, option, widget);

    qreal size = this->pixmap().width() * 0.2;

    painter->setRenderHint(QPainter::Antialiasing, true); // beaute

    if (size < 10) return; // resoudre le cas ou la taille des elements est tres petites

    QRadialGradient redGradient(0.0, 0.0, 17.0, size - 3, size - 3);
    redGradient.setColorAt(0.0, QColor(0xe0, 0x84, 0x9b));
    redGradient.setColorAt(0.5, QColor(0xe9, 0x34, 0x43));
    redGradient.setColorAt(1.0, QColor(0xdc, 0x0c, 0x00));

    QRadialGradient blueGradient(0.0, 0.0, 17.0, size - 3, size - 3);
    blueGradient.setColorAt(0.0, QColor(0x6f, 0x9d, 0xe8));
    blueGradient.setColorAt(0.5, QColor(0x4a, 0x72, 0xbf));
    blueGradient.setColorAt(1.0, QColor(0x27, 0x47, 0x94));

    int x = 0;
    int y = 0;

    // dessiner l'indentite du cluster ou du subcluster ou d'image
    QString id;
    switch (this->image->getType())
    {
    case Image::CLUSTER:
    case Image::SUB_CLUSTER:
        id = QString("%1").arg(this->image->getId());
        break;
    case Image::IMAGE:
        id = QString("%1").arg(Util::getNameOfFile(this->image->getFilename()));
        break;
    }

    painter->setFont(QFont(painter->font().family(), 11, QFont::Bold));
    while ((size - painter->fontMetrics().width(id)) < 6) {
            int pointSize = painter->font().pointSize() - 1;
            int weight = (pointSize < 6) ? QFont::Normal : QFont::Bold;
            painter->setFont(QFont(painter->font().family(), painter->font().pointSize() - 1, weight));
    }

    QColor shadowColor(0x00, 0x00, 0x00, size);
    QBrush brush = QBrush(shadowColor);
    fillEllipse(painter, x + 1, y, size, brush);
    fillEllipse(painter, x - 1, y, size, brush);
    fillEllipse(painter, x, y + 1, size, brush);
    fillEllipse(painter, x, y - 1, size, brush);

    painter->setPen(QPen(Qt::white, 2));
    fillEllipse(painter, x, y, size - 3, QBrush(blueGradient));
    painter->drawEllipse(x, y, size - 3, size - 3);

    painter->setPen(QPen(Qt::white, 1));
    painter->drawText(x, y, size - 3, size - 3, Qt::AlignCenter, id);

    // dessiner la quantity des images du cluster ou du subcluster
    if (this->image->getType() != Image::IMAGE)
    {
        QString quantity = QString("%1").arg(this->image->getQuantityChildrens());
        x = this->pixmap().width() - size;

        painter->setFont(QFont(painter->font().family(), 11, QFont::Bold));
        while ((size - painter->fontMetrics().width(quantity)) < 6) {
                int pointSize = painter->font().pointSize() - 1;
                int weight = (pointSize < 6) ? QFont::Normal : QFont::Bold;
                painter->setFont(QFont(painter->font().family(), painter->font().pointSize() - 1, weight));
        }

        fillEllipse(painter, x + 1, y, size, brush);
        fillEllipse(painter, x - 1, y, size, brush);
        fillEllipse(painter, x, y + 1, size, brush);
        fillEllipse(painter, x, y - 1, size, brush);

        painter->setPen(QPen(Qt::white, 2));
        fillEllipse(painter, x, y, size - 3, QBrush(redGradient));
        painter->drawEllipse(x, y, size - 3, size - 3);

        painter->setPen(QPen(Qt::white, 1));
        painter->drawText(x, y, size - 3, size - 3, Qt::AlignCenter, quantity);
    }
    else
    {
        QString quantity = QString("%1").arg(this->image->getQuantityClusters());
        x = this->pixmap().width() - size;

        painter->setFont(QFont(painter->font().family(), 11, QFont::Bold));
        while ((size - painter->fontMetrics().width(quantity)) < 6) {
                int pointSize = painter->font().pointSize() - 1;
                int weight = (pointSize < 6) ? QFont::Normal : QFont::Bold;
                painter->setFont(QFont(painter->font().family(), painter->font().pointSize() - 1, weight));
        }

        fillEllipse(painter, x + 1, y, size, brush);
        fillEllipse(painter, x - 1, y, size, brush);
        fillEllipse(painter, x, y + 1, size, brush);
        fillEllipse(painter, x, y - 1, size, brush);

        painter->setPen(QPen(Qt::white, 2));
        fillEllipse(painter, x, y, size - 3, QBrush(redGradient));
        painter->drawEllipse(x, y, size - 3, size - 3);

        painter->setPen(QPen(Qt::white, 1));
        painter->drawText(x, y, size - 3, size - 3, Qt::AlignCenter, quantity);
    }
}

//
// Overridre la fonction mousePressEvent pour capturer l'evenement clique sur une image
//
void ImageScene::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if (this->hovered) hoverLeaveEvent(0);
    if (event->button() == Qt::RightButton)
        emit clickedImage(this);
    else
        emit doubleClickedImage(this);
}

//
// Override la fonction hoverEnterEvent pour elargir la taille de l'image
//
void ImageScene::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
    Q_UNUSED(event);
    if (this->hovered) return;
    this->hovered = true;
    // quand le curseur entrer l'image
    // stocker la valeur zValue
    old_zValue = this->zValue();
    this->setZValue(old_zValue * 2);
    // stocker la taille
    old_height = this->pixmap().height();
    old_width = this->pixmap().width();
    // elargir
    qreal new_height = old_height * 1.2;
    qreal new_width = old_width * 1.2;

    deltaX = (new_width - old_width) / 2;
    deltaY = (new_height - old_height) / 2;
    this->scale(1.2, 1.2);
    this->setPos(this->x() - deltaX, this->y() - deltaY);
    this->update();
}

//
// Override la fonction hoverLeaveEvent pour remettre la originale taille de l'image
//
void ImageScene::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    Q_UNUSED(event);
    if (!this->hovered) return;
    // quand le curseur quit cette objet
    this->setZValue(old_zValue);
    this->scale(1/1.2, 1/1.2);
    this->setPos(this->x() + deltaX, this->y() + deltaY);
    this->update();
    this->hovered = false;
}

//
// Override la fonction itemtChange pour mettre a jour les coordonnees des liens
//
QVariant ImageScene::itemChange(GraphicsItemChange change, const QVariant &value)
{
    if (change == QGraphicsItem::ItemPositionChange){
        foreach (LineScene *line, lines)
            line->updatePosition();
    }
    return value;
}

void ImageScene::fillEllipse(QPainter *p, int x, int y, int size, const QBrush &brush)
{
    QPainterPath path;
    path.addEllipse(x, y, size, size);
    p->fillPath(path, brush);
}

//
// Dessiner trois premiere images du cluster
// @param:
//      Image *image : image est un cluster ou un sous-cluster
//
QString ImageScene::createImageCluster(Image *image)
{
    QPixmap pixmap1, pixmap2, pixmap3;

    // charger trois premieres images
    pixmap1.load(image->getChildren(0)->getPath());
    if (image->getQuantityChildrens() > 1)
        pixmap2.load(image->getChildren(1)->getPath());
    if (image->getQuantityChildrens() > 2)
        pixmap3.load(image->getChildren(2)->getPath());

    // dessiner trois images sur une image dont le fond est transparent
    QImage threeImage;
    threeImage.load(Util::getPathImageTransparent());

    QPainter *painter = new QPainter();
    painter->begin(&threeImage);

    QColor shadowColor(0x00, 0x00, 0x00, 64);
    painter->translate(10, 10);

    drawImageShadow(painter, THUMB_STEP2, 0,   10, shadowColor);
    drawImage(painter, THUMB_STEP2,  0,   10, pixmap3);

    painter->translate(10, 10);

    drawImageShadow(painter, THUMB_STEP1, 0, -15, shadowColor);
    drawImage(painter, THUMB_STEP1,  0, -15, pixmap2);

    drawImageShadow(painter,  0, 0,  15, shadowColor);
    drawImage(painter,  0,  0,  15, pixmap1);

    painter->end();

    // enregistrer sur un fichier png
    QString path = Util::getPathDirImages();
    if (image->getType() == Image::CLUSTER)
    {
        path += QString("/%1%2.png").arg("cluster").arg(image->getId());
    }
    else
    {
        path += QString("/%1%2.png").arg("subcluster").arg(image->getId());
    }

    threeImage.save(path);

    return path;
}

//
// Dessiner une image
//
void ImageScene::drawImage(QPainter *painter, int x, int y, int angle, const QPixmap& pixmap)
{
    painter->save();
    painter->translate(x, y);
    painter->translate(THUMB_HALF_SIZE, THUMB_HALF_SIZE);
    painter->rotate(angle);
    painter->translate(-THUMB_HALF_SIZE, -THUMB_HALF_SIZE);
    painter->drawPixmap(0, 0, THUMB_SIZE, THUMB_SIZE, pixmap);
    painter->restore();
}

//
// Dessiner l'ombre de l'image
//
void ImageScene::drawImageShadow(QPainter *painter, int x, int y, int angle, const QBrush& color)
{
    drawShadow(painter, x + 2, y,  angle, color);
    drawShadow(painter, x - 2, y,  angle, color);
    drawShadow(painter, x, y + 2,  angle, color);
    drawShadow(painter, x, y - 2,  angle, color);
}

void ImageScene::drawShadow(QPainter *painter, int x, int y, int angle, const QBrush& brush)
{
    painter->save();
    painter->translate(x, y);
    painter->translate(THUMB_HALF_SIZE, THUMB_HALF_SIZE);
    painter->rotate(angle);
    painter->translate(-THUMB_HALF_SIZE, -THUMB_HALF_SIZE);
    painter->fillRect(0, 0, THUMB_SIZE, THUMB_SIZE, brush);
    painter->restore();
}
