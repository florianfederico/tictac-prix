/*
    fichier image.c
    but: charger et libérer le menu et les images

    contient les fonctions:

       - chargerRessources
       - libererRessources

       - lireNombreImage
       - chargerImages

       - choisirImageAléatoire
       - lireBonPrix
                                                            */

#include <stdlib.h>             // On inclut les bibliothèques
#include <stdio.h>              // base pour la programmation en C

#include <SDL/SDL.h>            // SDL
#include <SDL/SDL_image.h>      // Gérer tous les formats d'images: jpg, png, bmp ...
#include <SDL/SDL_ttf.h>        // Permet d'écrire du texte: decompte, prixTape
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


void chargerRessources(SDL_Surface **menu1, SDL_Surface **menu2, SDL_Surface **imageDeFond, SDL_Surface **gagner, SDL_Surface **cestPlus, SDL_Surface **cestMoins, SDL_Surface **info, SDL_Surface **tempsEcoule, int *rejouer)
{
    // Chargement des images du dossier ressources

    *menu1 = IMG_Load("ressources/menu1.jpg");
    *menu2 = IMG_Load("ressources/menu2.jpg");
    *imageDeFond = IMG_Load("ressources/image_de_fond.jpg");
    *gagner = IMG_Load("ressources/gagne.jpg");
    *cestPlus = IMG_Load("ressources/plus.jpg");
    *cestMoins = IMG_Load("ressources/moins.jpg");
    *info = IMG_Load("ressources/info.jpg");
    *tempsEcoule = IMG_Load("ressources/temps_ecoule.jpg");

    // On teste si les ressources ont bien été chargées
    if(*menu1 == NULL || *menu2 == NULL || *imageDeFond == NULL || *gagner == NULL || *cestPlus == NULL || *cestMoins == NULL || *info == NULL || *tempsEcoule == NULL)
    {
        // Si une ressources n'a pas été chargée, message d'erreur dans le fichier stderr.txt et arrêt du jeu
        fprintf(stderr, "Erreur de chargement des ressources : %s\n", SDL_GetError());
        *rejouer = 0;
    }

}


void libererRessources(SDL_Surface **menu1, SDL_Surface **menu2, SDL_Surface **imageDeFond, SDL_Surface **gagner, SDL_Surface **cestPlus, SDL_Surface **cestMoins, SDL_Surface **info, SDL_Surface **tempsEcoule)
{
    // On libère les images du dossier ressources
    SDL_FreeSurface(*menu1);
    SDL_FreeSurface(*menu2);
    SDL_FreeSurface(*imageDeFond);
    SDL_FreeSurface(*gagner);
    SDL_FreeSurface(*cestPlus);
    SDL_FreeSurface(*cestMoins);
    SDL_FreeSurface(*info);
    SDL_FreeSurface(*tempsEcoule);
}


int lireNombreImage(int *rejouer)
{
    int i = 0, nombreImage = 0;

    DIR* dossier = NULL;             // pointeur qui contiendra la direction vers le dossier
    struct dirent* fichierLu = NULL; // Déclaration d'un pointeur qui contiendra les fichiers du dossier
    dossier = opendir("images/");    // Ouverture du dossier 'images'

    if (dossier == NULL)             // Si l'ouverture a échoué
    {
        *rejouer = 0;                // On arrête le jeu
    }

    while (i != 2)                    // On lit les 2 premiers fichiers caché du dossier sans les récupérer
    {                                 // pour déplacer le curseur sur les images
        fichierLu = readdir(dossier);
        i++;
    }

    i = 0;

    while((fichierLu = readdir(dossier)) != NULL) // Tant qu'il y a des images à lire
    {
        nombreImage = nombreImage + 1;
    }

    closedir(dossier);  // Fermeture du dossier 'image'
    return nombreImage; // On renvoie le nombre d'images à la fonction principale
}


void chargerImages(SDL_Surface *image[], int *rejouer)
{
    int i = 0;

    DIR* dossier = NULL;             // Pointeur qui contiendra la direction vers le dossier
    char cheminImage[100] = "";      // Variable qui contiendra le chemin vers l'image
    struct dirent* fichierLu = NULL; // Déclaration d'un pointeur qui contiendra les fichiers du dossier
    dossier = opendir("images/");    // Ouverture du dossier 'image'

    if (dossier == NULL)             // Si l'ouverture a échouée
    {
        *rejouer = 0;                // Arrêt du jeu
    }


    while (i != 2)                    // On lit les 2 premiers fichiers caché du dossier sans les récupérer
    {                                 // pour déplacer le curseur sur les images
        fichierLu = readdir(dossier);
        i++;
    }
    i = 0;

    while((fichierLu = readdir(dossier)) != NULL) // Tant qu'il y a des images à lire
    {
        sprintf(cheminImage, "images/%s", fichierLu->d_name); // Ecriture du chemin vers l'image dans la variable cheminImage
        image[i] = IMG_Load(cheminImage);                     // Chargement de l'image dans le tableau
        i++;
    }

    closedir(dossier); // Fermeture du dossier image
}


int choisirImageAleatoire(SDL_Surface *ecran, SDL_Surface *image[], SDL_Rect *positionImage, int *nombreImage)
{
    int nombreAleatoire = 0;
    srand(time(NULL)); // On charge un nombre dit "aléatoire" qui est générer par la fonction rand (random -> hasard)
    nombreAleatoire = (rand() % ((*nombreImage - 1) - 0 + 1)) + 0;

    positionImage->x = ecran->w / 2 - image[nombreAleatoire]->w / 2; // On centre l'image sur l'écran
    positionImage->y = ecran->h / 2 - image[nombreAleatoire]->h / 2;

    return nombreAleatoire;
}


int lireBonPrix(int nombreAleatoire, int *continuer, int *rejouer)
{
    int bonPrix = 0;

    FILE* fichier = fopen("ressources/prix.txt", "r"); // Ouverture du fichier prix.txt en mode lecture
    if (fichier != NULL)                               // Si l'ouverture s'est bien déroulée
    {
        int i = 0, caractereActuel = 0;
        while(i < nombreAleatoire)        // On positionne le curseur à la ligne contenant le prix de l'image
        {
            caractereActuel = fgetc(fichier); // On lit le caractère devant le curseur pour le tester et faire avancer le curseur

            if(caractereActuel == '\n')       // Si le caractèreActuel est un saut de ligne
            {
                i++;
            }
        }

        i = 0;
        while(i < 11)  // On déplace le curseur de 11 caractère sur la ligne pour sauter "image = " et lire uniquement le prix
        {
            caractereActuel = fgetc(fichier);
            i++;
        }

        fscanf(fichier, "%d", &bonPrix);  // On lit le prix et on le stocke dans bonPrix
        fclose(fichier);                  // Fermeture du fichier prix.txt
    }
    else // Sinon
    {
        fprintf(stderr, "Impossible d'ouvrir le fichier prix.txt\n");
        *continuer = 0;
        *rejouer = 0;
    }

    return bonPrix;
}





