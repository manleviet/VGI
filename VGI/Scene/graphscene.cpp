//
//
// graphscene.cpp
//
// Implementation de la classe graphscene.h
//
// LE Viet Man
// 10/06/10
//
//

#include "Scene/graphscene.h"

const int zValue = 10000;

//
// Construction
//
GraphScene::GraphScene()
{
    this->showCluster = false;
}

//
// Ajouter un nouveau graphe
// @param:
//      GraphImage *graph : un nouveau graphe
//
void GraphScene::addGraph(GraphImage *graph)
{
    this->graph = graph;

    // initier le premier niveau du graphe (les clusters)
    this->levels.clear();
    this->lines.clear();

    // initier le niveau 0
    LevelScene *level = new LevelScene(0);
    // mettre Image au Level
    // createLevel(Image *image, Level *level);
    // ex : createLevel(graph->getCluster(0), level);
    createLevel(graph->getCluster(0), level);

    level->setZValue(0 * zValue); // mettre le parametre zValue
    this->levels << level;
    showLevel(); // afficher les niveaux
}

//
// Changer la facons de voir les relations entre des images
// Voir la relation avec des autres images
// ou voir la relation avec des clusters
// @param:
//      bool show : true - voir la relation avec des clusters
//                  false - voir la relation avec des autres images
//
void GraphScene::setShowCluster(bool show)
{
    this->showCluster = show;
    // faire le changement
    if (show)
    { // voir la relation avec des clusters
        for (int i = 0; i < levels.length(); i++)
        {   // chercher les niveaux dont le focus est une image et
            // les elements sont aussi des images
            if (levels.at(i)->canChangeToClusters())
            {
                Image *focus = levels.at(i)->getFocus()->getImage();
                levels.at(i)->removeItems(this);
                createLevelCluster(focus, levels.at(i)); // recreer des elements du cluster
            }
        }
    }
    else
    { // voir la relation avec des images
        for (int i = 0; i < levels.length(); i++)
        {
            if (levels.at(i)->canChangeToImages())
            {
                Image *focus = levels.at(i)->getFocus()->getImage();
                levels.at(i)->removeItems(this);
                createLevelBetweenImage(focus, levels.at(i));
            }
        }
    }
    showLevel();
}

//
// Cette fonction resoudre l'evenement double-clique sur une image
// @param:
//      int id : l'indice du cluster
//      ImageScene *image : une image
//      ImageScene *focus : le focus du cluster
//
void GraphScene::doubleClickedImage(int id, ImageScene *image, ImageScene *focus)
{
    if ((levels.length() - 1) == id)
    { // si l'image cliquee est au dernier cluster
        if ((id != 0) && (image == focus)) return;
        // creer un nouveau niveau avec l'indice id+1 et le focus est image
        if (levels.length() == 8) return;

        // creer un nouveau niveau
        LevelScene *level = new LevelScene(id + 1);
        // creer un lien entre deux niveaux
        LineLevelScene *line = new LineLevelScene(levels.at(levels.length() - 1), level);

        // mettre les images au cluster
        createLevel(image->getImage(), level);

        level->setZValue((id + 1) * zValue);
        line->setZValue(level->zValue() + 1);

        levels.at(levels.length() - 1)->addLineLevel(line);
        level->addLineLevel(line);

        this->levels << level;
        this->lines << line;
    }
    else
    {
        // effacer des niveaux dont l'indice est plus que id
        while ((levels.length() - 1) > id)
        {
            LevelScene *level = levels.at(levels.length() - 1);
            disconnect(level, SIGNAL(doubleClickedImage(int,ImageScene*,ImageScene*)),
                       this, 0);
            level->removeItems(this);
            this->removeItem(level);
            levels.removeLast();
            this->removeItem(lines.at(lines.length() - 1));
            lines.removeLast();
        }

        // creer un nouveau niveau
        if (image != focus)
        {
            LevelScene *level = new LevelScene(id + 1);
            LineLevelScene *line = new LineLevelScene(levels.at(levels.length() - 1), level);

            // mettre les images au cluster
            createLevel(image->getImage(), level);

            level->setZValue((id + 1) * zValue);
            line->setZValue(level->zValue() + 1);

            levels.at(levels.length() - 1)->addLineLevel(line);
            level->addLineLevel(line);

            this->levels << level;
            this->lines << line;
        }
    }
    showLevel();
}

//
// Afficher les niveaux
//
void GraphScene::showLevel()
{
   layoutLevels(); // calculer les coordonnees des niveaux

   // afficher les niveaux
   for (int i = 0; i < levels.length(); i++)
   {
       LevelScene *level = levels.at(i);

       if (!level->isAdded())
       {
            this->connect(level, SIGNAL(doubleClickedImage(int,ImageScene*,ImageScene*)),
                          this, SLOT(doubleClickedImage(int,ImageScene*,ImageScene*)));

            addItem(level);
            level->setAdded(true);
       }

       // faire apparaitre les images cachees
       if (level->getId() == levels.length() - 1)
       {
            level->setVisible(true);
       }

       level->showLevel(this);
   }
   // afficher les lignes
   for (int i = 0; i < lines.length(); i++)
   {
       LineLevelScene *line = lines.at(i);

       if (!line->isAdded())
       {
           addItem(line);
           line->setAdded(true);
       }
   }
}

//
// Calculer les coordonnees des niveaux
// Implementation de l'algorithme spirale
// Auteur : LE Viet Man
//
void GraphScene::layoutLevels()
{
    qreal x = 0;
    qreal y = 0;
    qreal width = this->width();
    qreal height = this->height();

    if (levels.length() == 1)
    { // il y a seulement un niveau
        LevelScene *level = levels.at(levels.length() - 1);
        // la position du niveau est centre de l'ecran
        computePosLevel(level, Util::CENTRE_LEVEL, width, height, x, y);
    }
    else
    { // il y a plusieurs niveaux
        Util::TYPE_LEVEL type = Util::LEFT_LEVEL; // le type du niveau est le niveau a gauche
        for (int i = (levels.length() - 1); i >= 0; i--)
        { // traverser chaque niveau
            LevelScene *level = levels.at(i);
            // calculer les coordonnees des niveaux
            computePosLevel(level, type, width, height, x, y);
            // changer le type du niveau prochain
            type = Util::changeType(type);
        }
    }
}

//
// Calculer les coordonnees d'un niveau
// Note : dependre le type du niveau. Il y a cinq type :
// niveau au centre, niveau a gauche, niveau en bas, niveau a droit et niveau en haut
// @param:
//      LevelScene *level : un niveau
//      Util::TYPE_LEVEL type : le type du niveau
//      qreal &width : la largeur de la region dessinee
//      qreal &height : la hauteur de la region dessinee
//      qreal &x : la coordonnee x
//      qreal &y : la coordonnee y
//
void GraphScene::computePosLevel(LevelScene *level, Util::TYPE_LEVEL type,
                                 qreal &width, qreal &height,
                                 qreal &x, qreal &y)
{
    qreal newSize;
    qreal centreX;
    qreal centreY;

    switch(type)
    {
    case Util::CENTRE_LEVEL:
        {
            newSize = qMin(width, height) - 40;
            centreX = width / 2 + x;
            centreY = height / 2 + y;
            break;
        }
    case Util::LEFT_LEVEL:
        {
            width = width / 2;
            newSize = qMin(width, height) - 10;
            centreX = width / 2 + x;
            centreY = height / 2 + y;
            x = x + width;
            break;
        }
    case Util::BOTTOM_LEVEL:
        {
            height = height / 2;
            newSize = qMin(width, height) - 10;
            centreX = width / 2 + x;
            centreY = height / 2 + y + height;
            break;
        }
    case Util::RIGHT_LEVEL:
        {
            width = width / 2;
            newSize = qMin(width, height) - 10;
            centreX = width / 2 + x + width;
            centreY = height / 2 + y;
            break;
        }
    case Util::TOP_LEVEL:
        {
            height = height / 2;
            newSize = qMin(width, height) - 10;
            centreX = width / 2 + x;
            centreY = height / 2 + y;
            y = y + height;
            break;
        }
    }

    level->setRect(centreX - newSize/2,
                   centreY - newSize/2,
                   newSize,
                   newSize);
}

//
// Mettre les images au niveau
// @param:
//      Image *focus : le focus du niveau
//      LevelScene *level : un niveau
//
void GraphScene::createLevel(Image *focus, LevelScene *level)
{
    switch(level->getId())
    {
    case 0: // clusters
        createLevel0(focus, level);
        break;
    case 1: // subclusters ou images
        // le focus est un cluster
        if (focus->getQuantitySubCluters() != 0)
        {
            createLevelSubCluster(focus, level);
        }
        else
        {
            createLevelImage(focus, level);
        }
        break;
    case 2: // images
        // focus est un sous-cluster ou une image
        if (focus->getType() == Image::SUB_CLUSTER)
        {
            createLevelImage(focus, level);
        }
        else
        {
            // un niveau des images
            createLevelBetweenImage(focus, level);
        }
        break;
    case 3:
    case 4:
    case 5:
    case 6:
    case 7:
    case 8:
        if (showCluster)
        {
            if (focus->getType() == Image::IMAGE)
            {
                createLevelCluster(focus, level);
            }
            else
            {
                createLevelImage(focus, level);
            }
        }
        else
        {
            createLevelBetweenImage(focus, level);
        }
        break;
    }
}

//
// Initier le niveau des clusters
// @param:
//      Image *focus : le focus du niveau, ici il est un cluster
//      LevelScene *level : le niveau des clusters
//
void GraphScene::createLevel0(Image *focus, LevelScene *level)
{
    int idFocus = focus->getId();

    // prendre les clusters et mettre une liste imagesScene
    QList<ImageScene *> imagesScene;
    for (int i = 0; i < graph->getQuantityCluster(); i++)
    {
        Image *cluster = graph->getCluster(i);
        if (cluster->getQuantityChildrens() == 0) continue; // a l'exception des clusters qui n'ont pas des images

        ImageScene *imageScene = new ImageScene(cluster);
        imagesScene << imageScene;
    }

    // creer les liens entre des clusters
    QList<LineScene *> linesScene;
    for (int i = 0; i < graph->getQuantityLineClusters(); i++)
    {
        Line *line = graph->getLineCluster(i);

        // chercher start dans imagesScene
        ImageScene *start = searchImage(line->start(), imagesScene);

        // chercher end dans imagesScene
        ImageScene *end = searchImage(line->end(), imagesScene);

        LineScene *lineScene = new LineScene(start, end, line);
        start->addLine(lineScene);
        end->addLine(lineScene);

        linesScene << lineScene;
    }

    // mettre au niveau
    level->addFocus(idFocus);
    for (int i = 0; i < imagesScene.length(); i++)
    {
        level->addImage(imagesScene.at(i));
    }
    for (int i = 0; i < linesScene.length(); i++)
    {
        level->addLine(linesScene.at(i));
    }
}

//
// Initier un niveau des sous-clusters
// @param:
//      Image *focus : le focus est un cluster
//      LevelScene *level : le niveau des sous-clusters
//
void GraphScene::createLevelSubCluster(Image *focus, LevelScene *level)
{
    QList<ImageScene *> imagesScene;

    // mettre le focus a la liste imagesScene a la premiere position
    int idFocus = 0;
    ImageScene *imageFocus = new ImageScene(focus);
    imagesScene << imageFocus;

    // mettre les sous-clusters a la liste
    for (int i = 0; i < focus->getQuantitySubCluters(); i++)
    {
        Image *subcluster = focus->getSubCluster(i);
        if (subcluster->getQuantityChildrens() == 0) continue;

        ImageScene *imageScene = new ImageScene(subcluster);
        imagesScene << imageScene;
    }

    // creer les liens entre le focus et les sous-clusters
    QList<LineScene *> linesScene;
    int index = -1;
    for (int i = 0; i < focus->getQuantitySubCluters(); i++)
    {
        if (focus->getSubCluster(i)->getQuantityChildrens() == 0) continue;

        index++;
        Line *line = new Line(focus, focus->getSubCluster(i));
        line->setColor(Util::getColor(focus->getId() + 1));

        // chercher start dans imagesScene;
        ImageScene *start = imagesScene.at(0);

        // chercher end dans imagesScene
        ImageScene *end = imagesScene.at(index + 1);

        LineScene *lineScene = new LineScene(start, end, line);
        start->addLine(lineScene);
        end->addLine(lineScene);

        linesScene << lineScene;
    }

    // mettre au niveau
    level->addFocus(idFocus);
    for (int i = 0; i < imagesScene.length(); i++)
    {
        level->addImage(imagesScene.at(i));
    }
    for (int i = 0; i < linesScene.length(); i++)
    {
        level->addLine(linesScene.at(i));
    }
}

//
// Initier un niveau dont le focus est un cluster ou sous-cluster
// et les elements sont les images
// @param:
//      Image *focus : le focus
//      LevelScene *level : un niveau
//
void GraphScene::createLevelImage(Image *focus, LevelScene *level)
{
    QList<ImageScene *> imagesScene;

    // mettre le focus a la premiere position dans la liste imagesScene
    int idFocus = 0;
    ImageScene *imageFocus = new ImageScene(focus);
    imagesScene << imageFocus;

    // mettre les elements
    for (int i = 0; i < focus->getQuantityChildrens(); i++)
    {
        Image *image = focus->getChildren(i);

        ImageScene *imageScene = new ImageScene(image);
        imagesScene << imageScene;
    }

    // creer des liens entre le focus et les elements
    QList<LineScene *> linesScene;
    for (int i = 0; i < focus->getQuantityChildrens(); i++)
    {
        Line *line = new Line(focus, focus->getChildren(i));
        line->setColor(Util::getColor(focus->getId() + 1));

        // chercher start dans imagesScene;
        ImageScene *start = imagesScene.at(0);

        // chercher end dans imagesScene
        ImageScene *end = imagesScene.at(i + 1);

        LineScene *lineScene = new LineScene(start, end, line);
        start->addLine(lineScene);
        end->addLine(lineScene);

        linesScene << lineScene;
    }

    // mettre au niveau
    level->addFocus(idFocus);
    for (int i = 0; i < imagesScene.length(); i++)
    {
        level->addImage(imagesScene.at(i));
    }
    for (int i = 0; i < linesScene.length(); i++)
    {
        level->addLine(linesScene.at(i));
    }
}

//
// Initier un niveau dont le focus est une image
// et les elements sont les iamges
// @param:
//      Image *focus : le focus
//      LevelScene *level : un niveau
//
void GraphScene::createLevelBetweenImage(Image *focus, LevelScene *level)
{
    QList<ImageScene *> imagesScene;

    // mettre le focus a la premiere position dans la liste imagesScene
    int idFocus = 0;
    ImageScene *imageFocus = new ImageScene(focus);
    imagesScene << imageFocus;
    // mettre les elements
    for (int i = 0; i < focus->getQuantityNeighbors(); i++)
    {
        Image *image = focus->getNeighbor(i);

        ImageScene *imageScene = new ImageScene(image);
        imagesScene << imageScene;
    }

    // creer les liens
    QList<LineScene *> linesScene;
    for (int i = 0; i < focus->getQuantityLinesNeighbor(); i++)
    {
        Line *line = focus->getLineNeighbor(i);

        // chercher start dans imagesScene;
        ImageScene *start = searchImage(line->start(), imagesScene);

        // chercher end dans imagesScene
        ImageScene *end = searchImage(line->end(), imagesScene);

        LineScene *lineScene = new LineScene(start, end, line);
        start->addLine(lineScene);
        end->addLine(lineScene);

        linesScene << lineScene;
    }

    // mettre au niveau
    level->addFocus(idFocus);
    for (int i = 0; i < imagesScene.length(); i++)
    {
        level->addImage(imagesScene.at(i));
    }
    for (int i = 0; i < linesScene.length(); i++)
    {
        level->addLine(linesScene.at(i));
    }
}

//
// Initier un niveau dont le focus est une image
// et les elements sont les clusters
// @param:
//      Image *focus : le focus
//      LevelScene *level : un niveau
//
void GraphScene::createLevelCluster(Image *focus, LevelScene *level)
{
    QList<ImageScene *> imagesScene;

    // mettre le focus a la premiere position dans la liste imagesScene
    int idFocus = 0;
    ImageScene *imageFocus = new ImageScene(focus);
    imagesScene << imageFocus;
    // mettre les elements
    for (int i = 0; i < focus->getQuantityClusters(); i++)
    {
        Image *image = focus->getCluster(i);

        ImageScene *imageScene = new ImageScene(image);
        imagesScene << imageScene;
    }

    // creer les liens
    QList<LineScene *> linesScene;
    for (int i = 0; i < focus->getQuantityClusters(); i++)
    {
        Image *cluster = focus->getCluster(i);
        Line *line = new Line(focus, cluster);
        line->setColor(Util::getColor(cluster->getId() + 1));

        // chercher start dans imagesScene;
        ImageScene *start = imagesScene.at(0);

        // chercher end dans imagesScene
        ImageScene *end = imagesScene.at(i + 1);

        LineScene *lineScene = new LineScene(start, end, line);
        start->addLine(lineScene);
        end->addLine(lineScene);

        linesScene << lineScene;
    }

    // mettre au niveau
    level->addFocus(idFocus);
    for (int i = 0; i < imagesScene.length(); i++)
    {
        level->addImage(imagesScene.at(i));
    }
    for (int i = 0; i < linesScene.length(); i++)
    {
        level->addLine(linesScene.at(i));
    }
}

//
// Chercher une ImageScene entre des ImageScene en se basnat une Image
// Parce qu'une ImageScene contient une Image
// @param:
//      Image *image : une image
//      QList<ImageScene*> imagesScene : une liste des ImageScenes
//
ImageScene *GraphScene::searchImage(Image *image, QList<ImageScene *> imagesScene)
{
    for (int i = 0; i < imagesScene.length(); i++)
    {
        if (imagesScene.at(i)->hasImage(image))
            return imagesScene.at(i);
    }

    return NULL;
}
