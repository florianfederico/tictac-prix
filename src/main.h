
/* fichier main.h
   but: - contenir les param�tres g�n�raux du programme
        - contenir les prototypes des fonctions du fichier main.c

                                                            */

                // S�curit� pour n'inclure un header (fichier.h) qu'une seule fois
#ifndef MAIN_H  // Si la valeur MAIN_H n'est pas d�finie
#define MAIN_H  // On d�finie la valeur MAIN_H et on inclut le fichier

// Param�tre g�n�raux
#define LARGEUR_FENETRE 800
#define HAUTEUR_FENETRE 600

#define DECOMPTE 30 // Temps en seconde du decompte

// Prototypes de main.c
void appuyerSurTouche(int *rejouer);
void afficherErreur(SDL_Surface *ecran);

// Struture qui sert � mieux g�rer les �v�nements dans la fonction jouer
typedef struct Input Input;
struct Input
{
    char key[SDLK_LAST];
};

#endif


