#include <SDL2/SDL.h>
#include <stdio.h>
#include<time.h>
#include<math.h>

void drawSnake(SDL_Renderer *ren, int gridHeight, int gridLength,int screenArray[12][18], int size){
    for(int i=0;i<gridHeight;i++)
        for(int j=0;j<gridLength;j++)
            if(screenArray[i+1][j+1]!=0){
                SDL_Rect snake = {j*size,i*size, size, size};
                SDL_RenderFillRect(ren, &snake);
            }
}


int main(){
    int screen[10+2][16+2] = {{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}};
    int windowLength=800,windowHeight=500;
    int snakeHeadX=5,snakeHeadY=5,snakeTailX=4,snakeTailY=5,tempTailDir,dir=1,rndStore=time(0)%10000,gridHeight=10,gridLength=16,size=windowLength/gridLength,foodY=7,foodX=13;
    screen[snakeHeadY][snakeHeadX]=1;
    screen[snakeTailY][snakeTailX]=1;
    srand(time(0));


    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        printf("SDL_Init Error: %s\n", SDL_GetError());
        return 1;
    }
    SDL_Window *win = SDL_CreateWindow("Snake!", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,windowLength, windowHeight, SDL_WINDOW_SHOWN);
    SDL_Renderer *ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    int running = 1;
    int playing=1;
    SDL_Event e;

    while (running) {
        if(playing){
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {running = 0;}
            if (e.type == SDL_MOUSEBUTTONDOWN && e.button.button == SDL_BUTTON_LEFT ){

            }
            if(e.type == SDL_KEYDOWN){
             if(e.key.keysym.sym == SDLK_DOWN && screen[snakeHeadY+1][snakeHeadX]!=-2)    dir=2;
             if(e.key.keysym.sym == SDLK_UP && screen[snakeHeadY-1][snakeHeadX]!=2)    dir=-2;
             if(e.key.keysym.sym == SDLK_LEFT && screen[snakeHeadY][snakeHeadX-1]!=1)    dir=-1;
             if(e.key.keysym.sym == SDLK_RIGHT && screen[snakeHeadY][snakeHeadX+1]!=-1)    dir=1;
            }
        }
        //updating snake

        screen[snakeHeadY][snakeHeadX]=dir;
        snakeHeadX+=dir%2;
        snakeHeadY+=dir/2;
        //any non zero value is ok inside this
        /* -->removing the tail==>Ideas-
        1)every part has info to next part, start from head and go to tail
        2)every part has info to next and previous part 
        3)every part has info to last part,but the tail coords are stored and the previos part is found using a search on the four sides
        4)Final/Best idea- every part has info about coords of next part. Neck store heads coords, tail stores the previos parts coord,etc
        */
        if(foodX!=snakeHeadX || foodY!=snakeHeadY){
            //Moving Snakes tail
            tempTailDir=screen[snakeTailY][snakeTailX];
            screen[snakeTailY][snakeTailX]=0;
            snakeTailX+=tempTailDir%2;
            snakeTailY+=tempTailDir/2;
               
            //Restarting if game over
            if(screen[snakeHeadY][snakeHeadX]!=0){
                printf("You lost");
                    for(int i=1;i<11;i++)
                       for(int j=1;j<17;j++)
                            screen[i][j]=0;
                    snakeHeadX=5,snakeHeadY=5,snakeTailX=4,snakeTailY=5,dir=1,foodY=7,foodX=13,rndStore=time(0)%10000;
                    screen[snakeHeadY][snakeHeadX]=1;
                    screen[snakeTailY][snakeTailX]=1;
            }

            screen[snakeHeadY][snakeHeadX]=1;

        }
        else{
            screen[snakeHeadY][snakeHeadX]=1;
            //updating food location and also not updating tail
            do{
                srand(rndStore);
                rndStore=rand()%160;
            }while(screen[rndStore%10+1][rndStore/10+1]!=0);
            foodY=rndStore%10+1;
            foodX=rndStore/10+1;
        }


        SDL_SetRenderDrawColor(ren, 0, 0, 0, 255);
        SDL_RenderClear(ren); //Clearing screen

        //drawing snake
        SDL_SetRenderDrawColor(ren, 255, 255, 255, 255);
        drawSnake(ren,gridHeight,gridLength,screen,size);
        //drawing food
        SDL_SetRenderDrawColor(ren, 255, 0, 0, 255);
        SDL_Rect snake = {(foodX-1)*size,(foodY-1)*size, size, size};
        SDL_RenderFillRect(ren, &snake);

        SDL_RenderPresent(ren);

        SDL_Delay(250);
    }
    else{
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {running = 0;}
            if (e.type == SDL_MOUSEBUTTONDOWN && e.button.button == SDL_BUTTON_LEFT ){

            }
            if(e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_KP_ENTER){
                playing=1;            
            }
        }
        SDL_SetRenderDrawColor(ren, 0, 0, 0, 255);
        SDL_RenderClear(ren); //Clearing screen
        
        SDL_RenderPresent(ren);

        SDL_Delay(250);

    }
}
    SDL_DestroyRenderer(ren);
    SDL_DestroyWindow(win);
    SDL_Quit();

    return 0;
}