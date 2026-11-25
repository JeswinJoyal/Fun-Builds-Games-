#ifndef CHECKS
#define CHECKS

#include "structs.h"

coords clickCheck( int x, int y);
int checkVictory(int arr[24][24],int player);
int checkIntersect(coords a1, coords a2, coords b1, coords b2);
int checkConnectivity(coordLL* coordll[24][24], coords a, coords b);

#endif