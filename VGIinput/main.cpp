//
//
// main.cpp
//
// Ce programme creer un fichier d'entree pour VGI
//
// Fichier sortie contenira :
//      - Premiere ligne : vgi1.0
//      - image tag : le chemin du fichier de configuration de la base d'images
//      - databaseMS tag : le chemin du repertoire de MS
//      - databaseSA tag : le chemin du repertoire de SA
//      - quelques commentaires commencees par "//"
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

char *Usage = (char*)"Usage: vgiinput [arguments]\n\
Creer un fichier d'entree pour VGI.\n\
Fichier sortie contenira :\n\
- Premiere ligne : vgi1.0\n\
- image tag : le chemin du fichier de configuration de la base d'images\n\
- databaseMS tag : le chemin du repertoire de MS\n\
- databaseSA tag : le chemin du repertoire de SA\n\
- quelques commentaires commencees par //\n\
Arguments:\n\
    -a : aide\n\
    -e <path> : le chemin et le nom du fichier sortie\n\
    -c <path> : le chemin du fichier de configuration\n\
    -m <path> : le chemin du repertoire MS\n\
    -s <path> : le chemin du repertoire SA\n\
Exemple: vgiinput -e /home/manleviet/data.vgi -c /home/manleviet/MRI/BW/db.conf\
-m /home/manleviet/MRI/database/MS -s /home/manleviet/MRI/database/SA\n\
Auteur : LE Viet Man\n\
Date   : 13/05/10\n";

//
// Prototypes des fonctions
//
void showUsage(bool);
char* concatString(const char *st1, const char *st2);
int saveVGIInput(const char* path_conf, const char* path_MS, const char* path_SA, const char* path_fileout);

int main(int argc, char *argv[])
{
    if (argc < 2) // manque les paramètres
    {
        showUsage(true); // afficher les informations d'aide
    }

    char* path_conf = 0; // chemin du fichier de configuration
    char* path_MS = 0; // chemin du repertoire MS
    char* path_SA = 0; // chemin du repertoire SA
    char* path_fileout = 0; // chemin du fichier sortie

    // cette boucle lit les arguments passés
    int c;
    while ((c = getopt(argc, argv, "ae:c:m:s:")) != -1)
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
                break;
            }
            case 'c':
            {
                extern char *optarg;
                path_conf = optarg;
                break;
            }
            case 'm':
            {
                extern char *optarg;
                path_MS = optarg;
                break;
            }
            case 's':
            {
                extern char *optarg;
                path_SA = optarg;
                break;
            }
            default:
                break;
        }
    }

    if (!saveVGIInput(path_conf, path_MS, path_SA, path_fileout)) return 0;
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
//      const char* path_conf : chemin du fichier de configuration
//      const char* path_MS : chemin du repertoire MS
//      const char* path_SA : chemin du repertoire SA
//      const char* path_fileout : chemin du fichier sortie
// @return :
//      0 : s'il y a une erreur
//      1 : si tout est bien
//
int saveVGIInput(const char* path_conf, const char* path_MS, const char* path_SA, const char* path_fileout)
{
    // ouvrir le fichier sortie
    ofstream outfile;
    if (path_fileout == 0)
        outfile.open((char*)"data.vgi");
    else
        outfile.open(path_fileout);

    if (!outfile.is_open())
    {
        cerr << "Il y a une erreur dans le chemin du fichier sortie." << endl;
        return 0;
    }

    // enregistrer les informations au fichier sortie
    outfile << "vgi1.0" << endl;
    outfile << "//" << endl;
    outfile << "// Fichier de configuration pour VGI" << endl;
    outfile << "//" << endl;
    outfile << "// chemin du fichier de configuration de la base d'images" << endl;
    outfile << "image " << path_conf << endl;
    outfile << "// chemin du repertoire de MS" << endl;
    outfile << "databaseMS " << path_MS << endl;
    outfile << "// chemin du repertoire de SA" << endl;
    outfile << "databaseSA " << path_SA << endl;
    outfile << "// dans les repertoire MS et SA, on doit contenir les fichiers suivants :" << endl;
    outfile << "// 1. igroups.data" << endl;
    outfile << "// 2. les fichiers data des images" << endl;

    outfile.close();

    return 1;
}
