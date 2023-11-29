/*
    fichier jeu.h
    but: gérer les prototypes des fonctions du jeu */

#ifndef MENU_H
#define MENU_H

void jouer(SDL_Surface *ecran, SDL_Surface *imageDeFond, SDL_Surface *image[], SDL_Surface *gagner, SDL_Surface *cestPlus, SDL_Surface *cestMoins, SDL_Surface *tempsEcoule, int *nombreImage, int *rejouer, FMOD_SYSTEM *system, FMOD_SOUND *ticTac);
void chronometre(int *decompte, int *tempsPrecedent, FMOD_SYSTEM *system, FMOD_SOUND *ticTac);
void UpdateEvents(Input* in, int *continuer, int *rejouer);


#endif




