//no gpt used, even for gui part(:])
//also download SDL2 library for running this
#include<stdio.h>
#include<SDL2/SDL.h>
#include "draw.h"
#include "twixtInterface.h"
#include "structs.h"

int main(){
    int mousex=0, mousey=0, mousedown = 0;
    int arr[24][24]={};
    coordLL* coordll[24][24]={0};
    playerData playerdata={2,-2,1};
    arr[0][0]=1,arr[0][23]=1,arr[23][0]=1,arr[23][23]=1;

    int windowHeight=1080, windowLength=1920;
    if(SDL_Init(SDL_INIT_VIDEO)!=0){
        printf("There was some error loading SDL_INIT_VIDEO:%s\n",SDL_GetError());
        return 1;
    } 

    SDL_Window *window = SDL_CreateWindow("TwixT", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, windowLength, windowHeight, SDL_WINDOW_SHOWN);
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    int running = 1;
    SDL_Event event;
    while(running){
        while(SDL_PollEvent(&event)){
            if(event.type == SDL_QUIT)  running=0;
            else if(event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT){
                mousex = event.button.x;
                mousey = event.button.y;
                mousedown = 1;
            }
        }

        if(mousedown){ 
            if(update(renderer,mousex,mousey,arr,coordll,&playerdata))
                playerdata.player*=-1;
            mousedown=0;
        }

        SDL_SetRenderDrawColor(renderer,255,255,255,255);   
        SDL_RenderClear(renderer);

        if(playerdata.wonPlayer){
            SDL_Rect winnerDisplay = {400,820,300,50};
            if(playerdata.wonPlayer==1)   SDL_SetRenderDrawColor(renderer,255,0,0,255);
            else    SDL_SetRenderDrawColor(renderer,0,0,0,255);
            SDL_RenderFillRect(renderer,&winnerDisplay);

            drawBoard(renderer,arr,coordll);
            SDL_RenderPresent(renderer);
            SDL_Delay(5000);
            restartGame(arr,coordll,&playerdata);
        }
        
        SDL_Rect playerDisplay = {600+playerdata.player*180,200,50,50};
        if(playerdata.player==1)   SDL_SetRenderDrawColor(renderer,255,0,0,255);
        else    SDL_SetRenderDrawColor(renderer,0,0,0,255);
        SDL_RenderFillRect(renderer,&playerDisplay);

        drawBoard(renderer,arr,coordll);
        SDL_RenderPresent(renderer);
        
        SDL_Delay(250);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}