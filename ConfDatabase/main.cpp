//
//
// main.cpp
//
// Ce programme creer un fichier de configuration d'une base d'images
//
// Fichier sortie contenira :
//      - Premiere ligne : nombre d'images
//      - Autres lignes : les chemins des images
//
// LE Viet Man
// 13/05/10
//
//

#include <unistd.h>
#include <iostream>
#include <stdlib.h>
#include <sys/dir.h>
#include <string.h>
#include <fstream>

using namespace std;

char *Usage = (char*)"Usage: confdb [option] <chemin de la base d'images>\n\
Creer un fichier de configuration d'une base d'images.\n\
Fichier sortie contenira :\n\
- Premiere ligne : nombre d'images\n\
- Autres lignes : les chemins des images\n\
Arguments:\n\
    -a : aide\n\
    -e <path> : le chemin du fichier sortie\n\
Auteur : LE Viet Man\n\
Date   : 13/05/10\n";

//
// Prototypes des fonctions
//
void showUsage(bool);
char* concatString(const char *st1, const char *st2);
int saveConf(int count, char** path, const char* path_fileout);

int main(int argc, char *argv[])
{
    if (argc < 2) // manque les paramètres
    {
        showUsage(true); // afficher les informations d'aide
    }

    char* path_db = 0; // chemin de la base d'images
    char* path_fileout = 0; // chemin du fichier sortie

    // cette boucle lit les arguments passés
    int c;
    while ((c = getopt(argc, argv, "ae:")) != -1)
    {
        switch (c)
        {
            case 'a':
            {
                showUsage(false);
                break;
            }
            case 'e':
            {
                extern char *optarg;
                path_fileout = optarg;
                path_fileout = concatString(path_fileout, (char*)"/");
                break;
            }
            default:
                break;
        }
    }

    // lire le nom de fichier a la fin de la ligne de commande
    extern int optind;
    if (optind>=argc)
    {
        showUsage(true);
    }
    path_db = argv[optind];

    struct direct **files;

    int count = scandir(path_db, &files, NULL, NULL);

    // initier un array qui contenir les chemins
    char* path[count - 2];

    int j = -1;
    for (int i = 0; i < count; i++)
    {
        if ((strcmp(files[i]->d_name,(char*)".") != 0) &&
            (strcmp(files[i]->d_name, (char*)"..") != 0))
        {
            j++;
            path[j] = concatString(path_db, (char*)"/");
            path[j] = concatString(path[j], files[i]->d_name);
        }
    }

    // enregistrer les informations au fichier sortie
    if (!saveConf(count - 2, path, path_fileout)) return 0;
    cout << "Done" << endl;
    return 1;
}

//
// La fonction sert à afficher les informations d'aide de ce programme
// @param :
//	bool erreur : true, cad il y a une erreur
//                          la fonction exécutera la commande exit(1)
//                    false, cad il n'y a pas d'erreur
//                           la fonction exécutera la commande exit(0)
//
void showUsage(bool erreur)
{
    cerr << Usage << endl;
    if (erreur)
    {
        exit(1);
    }
    else
    {
        exit(0);
    }
}

//
// Joindre deux strings
// @param :
//      const char *st1 : string 1
//      const char *st2 : string 2
// @return : un string
//
char* concatString(const char *st1, const char *st2)
{
    char *st;
    st = (char *)malloc(sizeof(char)
                         * (strlen(st1) + strlen(st2) + 1));
    strcpy(st, st1);
    strcat(st, st2);
    return st;
}

//
// Enregistrer les informations au fichier sortie
// @param :
//      int count : nombre des fichiers
//      char** path : un array des chemins
//      const char* path_fileout : le chemin du fichier sortie
// @return :
//      0 : s'il y a une erreur
//      1 : si tout est bien
//
int saveConf(int count, char** path, const char* path_fileout)
{
    // creer le chemin du fichier sortie
    char* path_out;
    if (path_fileout == 0)
        path_out = (char*)"db.conf";
    else
        path_out = concatString(path_fileout, (char*)"db.conf");

    // ouvrir le fichier sortie
    ofstream outfile;
    outfile.open(path_out);

    if (!outfile.is_open())
    {
        cerr << "Il y a une erreur dans le chemin du fichier sortie." << endl;
        return 0;
    }

    // enregistrer les informations au fichier sortie
    outfile << count << endl;

    for (int i = 0; i < count; i++)
    {
        outfile << path[i] << endl;
    }

    outfile.close();

    return 1;
}
