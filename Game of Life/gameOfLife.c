#include<stdio.h>
#include<SDL2/SDL.h>
#include<SDL2/SDL_ttf.h>

void writeAtPos(SDL_Renderer* renderer, char text[], int posX, int posY,TTF_Font* font,SDL_Color* textColor){
    SDL_Rect textRect;
    SDL_Surface* textSurface = TTF_RenderText_Blended(font, text, *textColor);            //maybe make these function return on error;
    if(textSurface==NULL)   return;
    SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);      //maybe make these function return -1 on error;
    textRect.x = posX;  
    textRect.y = posY;  
    textRect.w = textSurface->w;
    textRect.h = textSurface->h;
    
    SDL_RenderCopy(renderer, textTexture, NULL, &textRect);

    SDL_FreeSurface(textSurface);
    SDL_DestroyTexture(textTexture);

}

int main(){
    int windowLength=1900,windowHeight=1000;
    int cell[2][102][182]={},cur=0,neighbours;
    int mouseclick=0,mousex,mousey,play=0;
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window *window = SDL_CreateWindow("GAME OF LIFE",SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,windowLength,windowHeight,SDL_WINDOW_SHOWN);
    SDL_Renderer *renderer = SDL_CreateRenderer(window,-1,SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    TTF_Init();
    TTF_Font* font = TTF_OpenFont("font.ttf", 28);
    SDL_Color textColor = { 0, 0, 0, 255 }; 
    
    int running = 1;
    SDL_Event event;

    while(running){
        while(SDL_PollEvent(&event)){
            if(event.type==SDL_QUIT) running = 0;
            else if(event.type==SDL_MOUSEBUTTONDOWN && event.button.button==SDL_BUTTON_LEFT){
                mousex = event.button.x;
                mousey = event.button.y;
                if(mousex>10 && mousex<91 && mousey>=100 && mousey<150)    play=1-play;
                else if(!play && mousex>10 && mousex<91 && mousey>=200 && mousey<250){//clear function
                    for(int i=1;i<=100;i++)
                        for(int j=1;j<=180;j++){
                            cell[0][i][j]=0;
                            cell[1][i][j]=0;
                        }
                }
                else if(!play && mousex>=100 && mousex<1500 && mousey>=0 && mousey<1000){
                    cell[cur][mousey/10+1][(mousex-90)/10]=1-cell[cur][mousey/10+1][(mousex-90)/10];  
                }
            }
        }
        
        SDL_SetRenderDrawColor(renderer,0,0,0,255);
        SDL_RenderClear(renderer);
        SDL_SetRenderDrawColor(renderer,50,50,50,255);
        SDL_Rect sidebar={0,0,100,windowHeight};
        SDL_RenderFillRect(renderer,&sidebar);
        SDL_Rect button={10,100,80,50};
        SDL_SetRenderDrawColor(renderer,100,100,100,255);
        SDL_RenderFillRect(renderer,&button);
        button.y+=100;
        SDL_RenderFillRect(renderer,&button);
        SDL_SetRenderDrawColor(renderer,255,255,255,255);
        if(!play)    writeAtPos(renderer,"Play",10,110,font,&textColor);
        else    writeAtPos(renderer,"Edit",10,110,font,&textColor);
        writeAtPos(renderer,"Clear",10,210,font,&textColor);
        
        //displaying the grid
        SDL_Rect cellRect={100,0,10,10};
        for(int i=1;i<=180;i++){
            for(int j=1;j<=100;j++){
                SDL_SetRenderDrawColor(renderer,cell[cur][j][i]*255,cell[cur][j][i]*255,cell[cur][j][i]*255,255);
                SDL_RenderFillRect(renderer,&cellRect);
                cellRect.y+=10;
            }
            cellRect.x+=10;
            cellRect.y=0;
        }   
        
        SDL_RenderPresent(renderer);
        
        if(play){
            for(int i=1;i<=100;i++){
                for(int j=1;j<=180;j++){
                    neighbours=cell[cur][i-1][j-1]+cell[cur][i][j-1]+cell[cur][i+1][j-1]+cell[cur][i+1][j]+cell[cur][i+1][j+1]+cell[cur][i][j+1]+cell[cur][i-1][j+1]+cell[cur][i-1][j];
                    if(neighbours==3)    cell[1-cur][i][j]=1;
                    else if(neighbours==2 && cell[cur][i][j])  cell[1-cur][i][j]=1;
                    else    cell[1-cur][i][j]=0;
                }
            }
        cur=1-cur;
        SDL_Delay(750);
        }

    }
    
    TTF_CloseFont(font);
    TTF_Quit();
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}