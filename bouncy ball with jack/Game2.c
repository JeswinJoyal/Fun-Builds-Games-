#include <SDL2/SDL.h>
#include <stdio.h>
#include <math.h>
#include<stdlib.h>
#include<time.h>

typedef struct{float x;float y;}xnyValue;

void drawCircle(SDL_Renderer *ren, xnyValue centre, int radius) {
    for (int w = -radius; w <= radius; w++) {
        for (int h = -radius; h < radius; h++) {
            if (w*w + h*h <= radius*radius ) {
                SDL_RenderDrawPoint(ren, centre.x + w, centre.y + h);
            }
        }
    }
}
void motionUpdate(float *position, float *velocity, float acceleration){
    *position+=*velocity+acceleration/2;
    *velocity+=acceleration;
}
void bounce(float *position,float *velocity, float acceleration,float height, float e){//e=coefficient of restitution
    float v=sqrtf((*velocity)*(*velocity)+2*acceleration*height);
    float u=(e+1)*v-(*velocity)-1;
    *position+=height-(e*e*v*v-u*u)/2;
    *velocity=-u;
}
void cornerBounce(xnyValue corner, int ballRadius, xnyValue *ball, xnyValue *ballVelocity){
    if((corner.x-ball->x)*(corner.x-ball->x)+(corner.x-ball->x)*(corner.x-*ball->x)<ballRadius*ballRadius){
        float temp=sqrt((ball->x-corner.x)*(ball->x-corner.x)+(ball->y-corner.y)*(ball->y-corner.y));
        float incident[2], reflected[2], normal[2]={(ball->x-corner.x)/temp,(ball->y-corner.y)/temp};

    }
}

int main() {
    int windowLength=900,windowHeight=600;
    int ballRadius=30;
    xnyValue ballVelocity, ball, jackHatLeft, jackVelocity;
    int jackLength=20,jackHeight=50;
    jackHatLeft.x=300, jackHatLeft.y =0,jackVelocity.y=0,jackVelocity.x=0;
    ballVelocity.x=0, ballVelocity.y=10;
    ball.x = -1, ball.y = -1;
    float coefOfRestitution =1;
    int groundHeight= windowHeight - 30;

    // Initialize SDL video system
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        printf("SDL_Init Error: %s\n", SDL_GetError());
        return 1;
    }
    SDL_Window *win = SDL_CreateWindow("Game2",SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, windowLength, windowHeight, SDL_WINDOW_SHOWN);
    SDL_Renderer *ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    int running = 1;
    SDL_Event e;
    while (running) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
                running = 0;
            }
            if (e.type == SDL_MOUSEBUTTONDOWN && e.button.button == SDL_BUTTON_LEFT ) {
                    if(e.button.y<groundHeight-ballRadius){
                    ball.x = e.button.x;
                    ball.y = e.button.y;
                    ballVelocity.y=0;
                    }
            }
            if(e.type == SDL_KEYDOWN){
                if(e.key.keysym.sym == SDLK_UP)
                    if(jackHatLeft.y+jackHeight == groundHeight)    jackVelocity.y=-10;
                if(e.key.keysym.sym == SDLK_LEFT){    
                    jackHatLeft.x-=5;
                    jackVelocity.x=-5;
                }
                if(e.key.keysym.sym == SDLK_RIGHT){    
                    jackHatLeft.x+=5;
                    jackVelocity.x=5;
                }
            }
        }
        SDL_SetRenderDrawColor(ren, 0, 0, 0, 255);
        SDL_RenderClear(ren);

        if(ballVelocity.y==0)printf("%d",ball.y);
        //gravity of jack
            if(jackHatLeft.y+jackHeight+jackVelocity.y<=groundHeight){
                jackHatLeft.y+=jackVelocity.y;
                jackVelocity.y++;
            }
            else if(jackHatLeft.y+jackHeight<groundHeight) jackHatLeft.y=groundHeight-jackHeight;
            else    jackVelocity.y=0;
        
        //reflection of the circle on boundaries
            if(ball.x+ballRadius > windowLength)    ballVelocity.x=-abs(ballVelocity.x);
            else if(ball.x-ballRadius < 0)   ballVelocity.x=abs(ballVelocity.x);
            if(ball.y-ballRadius < 0 )    ballVelocity.y=abs(ballVelocity.y);
        //gravity of ball and its movement +//bouncing on ground and jackhat
            if(ball.y+ballRadius+ballVelocity.y>=jackHatLeft.y && ball.x > jackHatLeft.x && ball.x < jackHatLeft.x+jackLength)    bounce(&ball.y,&ballVelocity.y,1,jackHatLeft.y-ball.y-ballRadius,1);
            else if(ball.y+ballRadius+ballVelocity.y>=groundHeight)  bounce(&ball.y,&ballVelocity.y,1,groundHeight-ball.y-ballRadius,coefOfRestitution);
            else    motionUpdate(&ball.y,&ballVelocity.y,1);
            ball.x+=ballVelocity.x;
        //ball on jack
            if(ball.y+ballRadius>jackHatLeft.y && (jackHatLeft.x<=ball.x+ballRadius+ballVelocity.x && jackHatLeft.x+jackLength>ball.x-ballRadius+ballVelocity.x))
                ballVelocity.x=-ballVelocity.x+jackVelocity.x;

        //draw ground
            SDL_Rect ground = {0, groundHeight, windowLength, windowHeight - groundHeight};
            SDL_SetRenderDrawColor(ren, 0, 255, 0,255); // red
            SDL_RenderFillRect(ren, &ground);
        //draw jack the jumper
            SDL_Rect Jack = {jackHatLeft.x, jackHatLeft.y, jackLength, jackHeight};
            SDL_SetRenderDrawColor(ren, 255, 200, 0,255); // red
            SDL_RenderFillRect(ren, &Jack);
        //drawing ball
            SDL_SetRenderDrawColor(ren, 255, 0, 0, 255); // green
            drawCircle(ren, ball, ballRadius);
        // Show what we drew
         SDL_RenderPresent(ren);


        //updating variables
            jackVelocity.x=0;//bad method of jackVelocity.x just for ball collision on sides, change it later if possible

        SDL_Delay(22);// for delaying the game
    }

    SDL_DestroyRenderer(ren);
    SDL_DestroyWindow(win);
    SDL_Quit();
    return 0;
}

/*Gravity and bouce incorperation ideas
        1)approximate kinematic similar equations
            ballVelocity.y--;
            ball.y-=ballVelocity.y;
            int newHeight =(ballVelocity.y-groundHeight+ball.y+ballRadius+1)*coefOfRestitution;
            ballVelocity.y=-(ballVelocity.y-1)*coefOfRestitution;
            if(ballVelocity.y==-1 || ballVelocity.y==-2 )
                ballVelocity.y=0;
            ball.y = groundHeight-1 - newHeight - ballRadius;

            ballVelocity.y--;
            ball.y-=ballVelocity.y;
            int newHeight =(ballVelocity.y-jackHatLeft.y+ball.y+ballRadius+1) ;
            ballVelocity.y=-(ballVelocity.y-1);
            if(ballVelocity.y==-1 || ballVelocity.y==-2 )
                ballVelocity.y=0;
            ball.y = jackHatLeft.y-1 - newHeight - ballRadius;
        
        2)using time(0) or cosidering each loop execution as a fixed time length and using exact kinematic equations

        3)Redirect ball velocit on crossing boundary
                -inacurate but very simple to implement

            */