#ifndef DRAW
#define DRAW
#include<stdio.h>
#include<SDL2/SDL.h>
#include"structs.h"

void drawBoard(SDL_Renderer* renderer,int arr[24][24],coordLL* coordll[24][24]);
void drawPoint(SDL_Renderer *renderer, int i, int j, int player);
void drawLine(SDL_Renderer *renderer, coords a, coords b,int player);

#endif