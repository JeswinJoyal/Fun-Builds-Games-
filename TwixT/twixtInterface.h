#ifndef INTERFACE
#define INTERFACE

#include<stdio.h>
#include "draw.h"
#include "structs.h"
#include "checks.h"

// void updateLine(SDL_Renderer *renderer, coords ind, int arr[24][24],coordLL *coordLL[24][24]);
int update(SDL_Renderer* renderer, int x, int y, int arr[24][24], coordLL* coordll[24][24], playerData* playerdata);
void restartGame(int arr[24][24], coordLL* coordll[24][24], playerData* playerdata);

#endif