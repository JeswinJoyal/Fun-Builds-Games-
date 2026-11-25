#include "draw.h"

void drawBoard(SDL_Renderer* renderer,int arr[24][24],coordLL* coordll[24][24]){
    SDL_SetRenderDrawColor(renderer,0,0,0,255);
    SDL_Rect board = {399,299,492,492};
    SDL_RenderFillRect(renderer, &board);

    SDL_SetRenderDrawColor(renderer,190,190,190,255);
    board.x+=2,board.y+=2;
    SDL_RenderFillRect(renderer, &board);

    SDL_SetRenderDrawColor(renderer,255,0,0,255);
    SDL_Rect line = {432,324,428,4};
    SDL_RenderFillRect(renderer, &line);
    line.y+=440;
    SDL_RenderFillRect(renderer, &line);
    SDL_SetRenderDrawColor(renderer,0,0,0,255);
    line.x=424, line.y=332, line.h=428, line.w=4;
    SDL_RenderFillRect(renderer, &line);
    line.x+=440;
    SDL_RenderFillRect(renderer, &line);

    coords a;
    for(a.x=0;a.x<24;a.x++){
        for(a.y=0;a.y<24;a.y++){
            coordLL* traverse=coordll[a.y][a.x];
            while(traverse!=NULL){
                drawLine(renderer,a,traverse->pos,arr[a.y][a.x]);
                traverse=traverse->next;
            }

        }
    }

    SDL_Rect cells = {413,313,8,8};
    for(int i=0;i<24;i++){
        for(int j=0;j<24;j++){
            if(arr[i][j]==0)    SDL_SetRenderDrawColor(renderer,90,90,90,255);
            else if(arr[i][j]>1)    SDL_SetRenderDrawColor(renderer,255,0,0,255);
            else if(arr[i][j]<-1)    SDL_SetRenderDrawColor(renderer,0,0,0,255);
            else   SDL_SetRenderDrawColor(renderer,190,190,190,255);

            SDL_RenderFillRect(renderer, &cells);
            cells.x+=20;
        }
        cells.x=413;
        cells.y+=20;
    }

}

void drawPoint(SDL_Renderer *renderer, int i, int j, int player){
    SDL_SetRenderDrawColor(renderer, 127*(1+player), 0, 0, 255);
    SDL_Rect point = {413+j*20, 313+i*20, 8, 8};
    SDL_RenderFillRect(renderer, &point);
}

void drawLine(SDL_Renderer *renderer, coords a, coords b, int player){
    if(player>1)   SDL_SetRenderDrawColor(renderer,255,0,0,255);
    else    SDL_SetRenderDrawColor(renderer,0,0,0,255);
   for(int i=0;i<5;i++){
        SDL_RenderDrawLine(renderer, 414+a.x*20, 314+a.y*20+i, 415+b.x*20, 315+b.y*20+i);
    }
    return;
}