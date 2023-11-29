
/* fichier main.h
   but: - contenir les paramètres généraux du programme
        - contenir les prototypes des fonctions du fichier main.c

                                                            */

                // Sécurité pour n'inclure un header (fichier.h) qu'une seule fois
#ifndef MAIN_H  // Si la valeur MAIN_H n'est pas définie
#define MAIN_H  // On définie la valeur MAIN_H et on inclut le fichier

// Paramètre généraux
#define LARGEUR_FENETRE 800
#define HAUTEUR_FENETRE 600

#define DECOMPTE 30 // Temps en seconde du decompte

// Prototypes de main.c
void appuyerSurTouche(int *rejouer);
void afficherErreur(SDL_Surface *ecran);

// Struture qui sert à mieux gérer les évènements dans la fonction jouer
typedef struct Input Input;
struct Input
{
    char key[SDLK_LAST];
};

#endif


