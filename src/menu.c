/* fichier menu.c
   but: g�rer le menu

   contient la fonction:
     -  gererMenu        */

#include <stdlib.h>             // On inclut les biblioth�ques
#include <stdio.h>              // base pour la programmation en C

#include <SDL/SDL.h>            // SDL
#include <SDL/SDL_image.h>      // G�rer tous les formats d'images: jpg, png, bmp ...
#include <SDL/SDL_ttf.h>        // Permet d'�crire du texte: decompte, prixTape
#include <fmodex/fmod.h>        // Jouer du son: tic tac
#include <math.h>               // Utiliser la fonction ceil() dans jeu.c pour prendre la partie entiere d'un nombre
#include <dirent.h>             // Parcourir les dossier dans image.c pour charger toutes les images du dossier

#ifndef WIN32
    #include <sys/types.h>      // Parcourir les dossier dans image.c pour charger toutes les images du dossier
#endif

#include "main.h"               // On inclut les fichier .h dit "headers" qui contiennent les prototypes des fonctions
#include "menu.h"               // Cela permet d'utiliser les fonctions des autres fichiers dans celui-ci
#include "jeu.h"                // pour rendre le code plus lisible
#include "image.h"


int gererMenu(SDL_Surface *ecran, SDL_Surface *menu1, SDL_Surface *menu2, int *rejouer)
{
    int continuer = 1, choixMenu = 1;

    SDL_Event event;    // Variable qui sert � g�rer les �v�nements
    SDL_Rect positionMenu;
    positionMenu.x = 0;
    positionMenu.y = 0;

    SDL_BlitSurface(menu1, NULL, ecran, &positionMenu); // On colle le menu1 � l'�cran

    while (continuer)      // On cr�e une boucle pour tester en permanence si il y a un �v�nement
    {
        SDL_PollEvent(&event);     // Chargement des �v�nements dans event
        switch(event.type)
        {
            case SDL_QUIT:      // Si on clique sur la croix de la fen�tre
                continuer = 0;
                *rejouer = 0;
                choixMenu = 0;
                break;
            case SDL_KEYDOWN:      // Si une touche est appuy�e
                switch(event.key.keysym.sym)
                {
                    case SDLK_RETURN:     // Si on appuie sur la touche entr�e
                        continuer = 0;
                        break;
                    case SDLK_UP:  // Fl�che du haut
                        SDL_BlitSurface(menu1, NULL, ecran, &positionMenu);  // On colle le menu1 � l'�cran
                        choixMenu = 1;
                        break;
                    case SDLK_DOWN:   // Fl�che du bas
                        SDL_BlitSurface(menu2, NULL, ecran, &positionMenu);   // On collele menu2 � l'�cran
                        choixMenu = 2;
                        break;
                }
                break;
        }

        SDL_Delay(30);       // On met une pause de 30 ms pour emp�cher que le jeu monopolise le processeur
        SDL_Flip(ecran);     // Mise � jour de l'�cran
    }

    return choixMenu;      // On renvoie le choix du menu
}





