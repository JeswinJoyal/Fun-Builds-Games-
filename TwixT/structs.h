#ifndef STRUCTS
#define STRUCTS

typedef struct{int x,y;}coords;

typedef struct{
    int p1group;
    int p2group;
    int player;
    int wonPlayer;
}playerData;

typedef struct coordLL{
    coords pos;
    struct coordLL* next;
}coordLL;

#endif