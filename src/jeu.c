/*
    fichier jeu.c
    but: g�rer le jeu

    contient les fonctions:
      - jouer
      - chronom�tre
      - UpdateEvent
                                */

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


void jouer(SDL_Surface *ecran, SDL_Surface *imageDeFond, SDL_Surface *image[], SDL_Surface *gagner, SDL_Surface *cestPlus, SDL_Surface *cestMoins, SDL_Surface *tempsEcoule, int *nombreImage, int *rejouer, FMOD_SYSTEM *system, FMOD_SOUND *ticTac)
{

    TTF_Font *police = TTF_OpenFont("ressources/mriam.ttf", 38);  // Chargement de la police
    SDL_Color couleurNoire = {0, 0, 0};                           // Couleur de l'�criture

    int continuer = 1;
    int nombreAleatoire = 0;        // nombreAleatoire permet de choisir aleatoirement une image dans le tableau

    int decompte = DECOMPTE, tempsPrecedent = 0;             // DECOMPTE est d�finie dans main.h
    int bonPrix = 0, prixTape = 0, prixEntre = 0;

    char chiffreChrono[30] = "", chiffrePrixEntre[20] = "";  // Sert � �crire le decompte et le prix entr� � l'�cran

    SDL_Surface *chrono = NULL, *textePrix = NULL; // Surfaces qui seront remplies par chiffreChrono et chiffrePrixEntre


    //********************* INITIALISATION DE LA POSITION DES SURFACES A AFFICHER ********************

    SDL_Rect positionImage; // On l'initialise quand on connait la taille de l'image � afficher
    SDL_Rect positionEcran;
    positionEcran.x = 0;
    positionEcran.y = 0;

    SDL_Rect positionChrono;
    positionChrono.x = ecran->w - 100; // Largeur de l'�cran (800 px) - 100 px
    positionChrono.y = 50;

    SDL_Rect positionPrixTape;
    positionPrixTape.x = ecran->w / 2 - 100;
    positionPrixTape.y = ecran->h - 100;

    //******************* CHOIX D'UNE IMAGE ALEATOIRE ET LECTURE DU BON PRIX CORRESPONDANT ***************

    nombreAleatoire = choisirImageAleatoire(ecran, image, &positionImage, nombreImage); // Chargement de l'image al�atoire
    bonPrix = lireBonPrix(nombreAleatoire, &continuer, &rejouer);                       // On lit le bon prix dans prix.txt


    //*************************** DEBUT DE LA BOUCLE **************************************

    SDL_Event event;             // Contient les �v�nements du clavier
    Input in;                    // Structure d�finie dans main.h qui g�re mieux event pour forcer l'utilisateur � appuyer
                                 // plusieurs fois sur une touche pour r�p�ter l'�v�nement

    memset(&in,0,sizeof(in));    // On initialise tous les �v�nements possible � 0

    // Tant que prixEntre different de bonPrix   ET   decompte > 0   ET  continuer = 1
    while(prixEntre != bonPrix && decompte > 0 && continuer == 1)
    {
        UpdateEvents(&in, &continuer, rejouer); // On charge les �v�nements

        if(in.key[SDLK_ESCAPE]) // Si on appuye sur echap
        {
            continuer = 0;
        }

        if(in.key[SDLK_RETURN])    // Si on appuie sur la touche entr�e
        {
            prixEntre = prixTape;  // On copie prixTape dans prixEntre pour le tester avec bonPrix
            prixTape = 0;          // On r�initialise prixTape

            if(prixEntre < bonPrix) // Si le prix entr� est < au bon prix
            {
                SDL_BlitSurface(cestPlus, NULL, ecran, &positionEcran); // On colle l'image cestPlus � l'�cran
                SDL_Flip(ecran);                                        // On met � jour l'�cran

                SDL_Delay(1000);                   // On attend 1 sec pour laisser le temps au joueur de lire
                in.key[event.key.keysym.sym] = 0 ; // On annule les �v�nemtns qui ont eu lieu pendant la pause
            }
            else if(prixEntre > bonPrix)
            {
                SDL_BlitSurface(cestMoins, NULL, ecran, &positionEcran); // On colle l'image cestPlus � l'�cran
                SDL_Flip(ecran);                                        // On met � jour l'�cran

                SDL_Delay(1000);                   // On attend 1 sec pour laisser le temps au joueur de lire
                in.key[event.key.keysym.sym] = 0 ; // On annule les �v�nemtns qui ont eu lieu pendant la pause
            }
        }

        if(in.key[SDLK_BACKSPACE])  // Si on appuye sur la touche retour
        {
            prixTape = ceil(prixTape / 10);  // On divise le prixEntre par 10 et on prend sa partie enti�re
                                             // pour faire disparaitre le dernier chiffre entr�

            in.key[SDLK_BACKSPACE] = 0;      // On met � 0 cet �v�nement pour forcer le joueur � taper plusieurs fois
                                             // sur retour pour enlever des chiffres
        }

        if(prixTape*10 < 1000000000)         // Si le prixTape va d�passer 1.000.000.000, on ne fait rien
        {
            if(in.key[SDLK_KP0])            // Si on appuye sur 0
            {
                prixTape = prixTape*10 + 0;
                in.key[SDLK_KP0] = 0;       // On r�initialise l'�v�nements "touche 0" pour forcer l'utilisateur
                                            // � appuyer plusieurs fois pour entrer plusieurs 0
            }

            if(in.key[SDLK_KP1])
            {
                prixTape = prixTape*10 + 1;
                in.key[SDLK_KP1] = 0;
            }

            if(in.key[SDLK_KP2])
            {
                prixTape = prixTape*10 + 2;
                in.key[SDLK_KP2] = 0;
            }

            if(in.key[SDLK_KP3])
            {
                prixTape = prixTape*10 + 3;
                in.key[SDLK_KP3] = 0;
            }

            if(in.key[SDLK_KP4])
            {
                prixTape = prixTape*10 + 4;
                in.key[SDLK_KP4] = 0;
            }

            if(in.key[SDLK_KP5])
            {
                prixTape = prixTape*10 + 5;
                in.key[SDLK_KP5] = 0;
            }

            if(in.key[SDLK_KP6])
            {
                prixTape = prixTape*10 + 6;
                in.key[SDLK_KP6] = 0;
            }

            if(in.key[SDLK_KP7])
            {
                prixTape = prixTape*10 + 7;
                in.key[SDLK_KP7] = 0;
            }

            if(in.key[SDLK_KP8])
            {
                prixTape = prixTape*10 + 8;
                in.key[SDLK_KP8] = 0;
            }

            if(in.key[SDLK_KP9])
            {
                prixTape = prixTape*10 + 9;
                in.key[SDLK_KP9] = 0;
            }
        }


        // Gestion du temps et du son tic tac
        chronometre(&decompte, &tempsPrecedent, system, ticTac);

        // On �crit le temps dans la variable chiffreChrono de type char pour pouvoir l'�crire a l'ecran
        sprintf(chiffreChrono, "%d", decompte);

        // On �crit le prix qui est en train d'etre tap� dans chiffrePrixEntre de type char pour pouvoir l'�crire a l'ecran
        sprintf(chiffrePrixEntre, "%d", prixTape);

        // On remplit la surface chrono avec le nombre du chronom�tre
        chrono = TTF_RenderText_Blended(police, chiffreChrono, couleurNoire);

        // On remplit la surface texte avec le prix tap�
        textePrix = TTF_RenderText_Blended(police, chiffrePrixEntre, couleurNoire);

        SDL_BlitSurface(imageDeFond, NULL, ecran, &positionEcran);            // On colle l'image de fond
        SDL_BlitSurface(image[nombreAleatoire], NULL, ecran, &positionImage); // On colle l'image
        SDL_BlitSurface(chrono, NULL, ecran, &positionChrono);                // On colle le nombre du chrono
        SDL_BlitSurface(textePrix, NULL, ecran, &positionPrixTape);           // On colle le prixTap�

        SDL_Flip(ecran);                                                      // Mise � jour de l'�cran
    }

    //************************* FIN DE LA BOUCLE *********************************************

    if(prixEntre == bonPrix) // GAGNER
    {
        SDL_BlitSurface(gagner, NULL ,ecran, &positionEcran);  // On colle l'image gagner
        SDL_Flip(ecran);                                       // Mise � jour de l'�cran
        appuyerSurTouche(&rejouer);                            // On attend l'appuye d'une touche pour revenir au menu
    }

    if(decompte <= 0)       // TEMPS ECOULE
    {
        SDL_BlitSurface(tempsEcoule, NULL, ecran, &positionEcran); // On colle l'image tempsEcoule � l'�cran
        SDL_Flip(ecran);                                           // Mise � jour de l'�cran
        appuyerSurTouche(&rejouer);                                // On attend l'appuye d'une touche pour continuer
    }

    TTF_CloseFont(police);       // On lib�re la police
    SDL_FreeSurface(chrono);     // On lib�re la surface chrono
    SDL_FreeSurface(textePrix);  // On lib�re la surface textePrix

    return EXIT_SUCCESS;
}


void chronometre(int *decompte, int *tempsPrecedent, FMOD_SYSTEM *system, FMOD_SOUND *ticTac)
{


    int tempsActuel = SDL_GetTicks();    // On charge le temps qui s'est �coul� depuis le debut du jeu
    if (tempsActuel - *tempsPrecedent > 1000) // Si le temps est sup�rieur � 1000 ms
    {
        *tempsPrecedent = tempsActuel;
        *decompte = *decompte - 1;
        FMOD_System_PlaySound(system, FMOD_CHANNEL_FREE, ticTac, 0, NULL);
    }
    else
    {
        SDL_Delay(30); // On attend 30 ms pour ne pas surcharger le processeur
    }


}


void UpdateEvents(Input* in, int *continuer, int *rejouer)
{
    SDL_Event event;
    while(SDL_PollEvent(&event)) // On charge les evenements dans event
    {
        switch (event.type)
        {
        case SDL_QUIT:           // Si on appuye sur la croix de la fen�tre, on arr�te le jeu
            *continuer = 0;
            *rejouer = 0;
            break;
        case SDL_KEYDOWN:                     // Si une touche est appuy�e
            in->key[event.key.keysym.sym]=1;  // On met � 1 la structure in
            break;
        case SDL_KEYUP:                       // Si une touche est relach�e
            in->key[event.key.keysym.sym]=0;  // On met � 0 la structure in
            break;
        default:                 // Autre type d'�v�nements, on ne fait rien
            break;
        }
    }
}


