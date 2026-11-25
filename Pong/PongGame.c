#include <SDL2/SDL.h>
#include <stdio.h>
#include <math.h>
#include<stdlib.h>
#include<time.h>
// by drawing many small points inside a radius
void drawCircle(SDL_Renderer *ren, int centerX, int centerY, int radius) {
    for (int w = -radius; w <= radius; w++) {
        for (int h = -radius; h < radius; h++) {
            if (w*w + h*h <= radius*radius && w*w + h*h > (radius-3)*(radius-3) ) { // equation of circle: x²+y² <= r²
                SDL_RenderDrawPoint(ren, centerX + w, centerY + h);
            }
        }
    }
}

int newPoint(int oldPoint, int parameterValue,int maxPoints){
    int newPoints;
    newPoints = oldPoint + parameterValue; //equation of points
    if(newPoints < 0) //point limits
        newPoints = 0;
    else if(newPoints > maxPoints)
        newPoints = maxPoints;
    
    return newPoints;
}

void letterDraw(SDL_Renderer *ren, int xPos, int yPos,char letter){
    if(letter==72){
        for(int i=0;i<6;i++)
            for(int j=0;j<35;j++)
                SDL_RenderDrawPoint(ren, xPos + i, yPos + j);

        for(int i=0;i<6;i++)
            for(int j=0;j<35;j++)
                SDL_RenderDrawPoint(ren, xPos + i + 20, yPos + j);

        for(int i=6;i<20;i++)
            for(int j=15;j<20;j++)
                SDL_RenderDrawPoint(ren, xPos + i, yPos + j);
    }
    if(letter==73){
        for(int i=0;i<25;i++)
            for(int j=0;j<6;j++)
                SDL_RenderDrawPoint(ren, xPos + i, yPos + j);

        for(int i=10;i<16;i++)
            for(int j=6;j<29;j++)
                SDL_RenderDrawPoint(ren, xPos + i, yPos + j);

        for(int i=0;i<25;i++)
            for(int j=29;j<35;j++)
                SDL_RenderDrawPoint(ren, xPos + i, yPos + j);
    }
    
 
}

int main() {
    int mouseClick=0;
    int windowLength=640,windowHeight=480;
    int points=0;
    int vectX=0,vectY=0;
    int rad, podHeight=0, podLength=windowLength/6,podSpeed=10,ballSpeedQoutient=windowLength*7/600;
    // Initialize SDL video system
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        printf("SDL_Init Error: %s\n", SDL_GetError());
        return 1;
    }

    // Create a window (title, x-pos, y-pos, width, height, flags)
    SDL_Window *win = SDL_CreateWindow("Pong!",
                        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                        windowLength, windowHeight, SDL_WINDOW_SHOWN);

    // Create a renderer (accelerated = use GPU, vsync = match screen refresh)
    SDL_Renderer *ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    int running = 1;// game loop control flag
    SDL_Event e;// event object (stores info about input events)

    int circleX = -1, circleY = -1; // store circle position (invalid at start)

    while (running) {

        // Poll all events (keyboard, mouse, quit, etc.)
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
                running = 0; // if window close button clicked, quit
            }
            if (e.type == SDL_MOUSEBUTTONDOWN && e.button.button == SDL_BUTTON_LEFT ) {

                if(e.button.x > windowLength/2){
                    circleX = e.button.x; // update circleX and circleY with the mouse position
                    circleY = e.button.y;

                    srand(circleX*circleY*3 + time(0));
                    rad = rand()%100 + 1;

                //direction updating for circle movement
                    vectY=rad%ballSpeedQoutient-ballSpeedQoutient/2;
                    vectX=rad/ballSpeedQoutient%ballSpeedQoutient-ballSpeedQoutient/2;

                    // points = newPoint(points,rad,100);// dont need anymore?
                    // printf("%d\n", points);
                }
            }
            if(e.type == SDL_KEYDOWN){
                if(e.key.keysym.sym == SDLK_DOWN && podHeight < windowHeight-podLength)
                    podHeight+=podSpeed;
                else if(e.key.keysym.sym == SDLK_UP && podHeight > 0)
                    podHeight-=podSpeed;
            }
        }

        // Clear screen with black color such tha old items erased
        SDL_SetRenderDrawColor(ren, 0, 0, 0, 255);
        SDL_RenderClear(ren);

        //letter drawing
        // if(points>50){
        //     SDL_SetRenderDrawColor(ren, 255, 100, 0, 255);
        //     letterDraw(ren, circleX-36, circleY,'H');
        //     letterDraw(ren, circleX, circleY,'I');
        // }

        //reflection of the circle on boundaries
        if(circleX+rad > windowLength)
            vectX=-abs(vectX);
        else if(circleX-rad < 10 && circleX!=-1){
            if(podHeight <= circleY && podHeight + podLength > circleY ){
                points=newPoint(points,5,100);
                vectX=abs(vectX);
            }
            else{
                points=newPoint(points,-25,100);
                circleX=-1;
                circleY=-1;
            }
        }
        if(circleY+rad > windowHeight)
            vectY=-abs(vectY);
        else if(circleY-rad < 0 )
            vectY=abs(vectY);

        //drawing circle
        if (circleX != -1 && circleY != -1) {
            SDL_SetRenderDrawColor(ren, 0, 255, 0, 255); // green
            drawCircle(ren, circleX, circleY, rad);

        //Updating crcleX and circleYfor movement of circle
            circleX+=vectX;
            circleY+=vectY;
        }
        //pod
        SDL_Rect pod = {0, podHeight,10 , podLength};
        SDL_SetRenderDrawColor(ren, 255, 0, 0, 255); // red
        SDL_RenderFillRect(ren, &pod);
        //point bar       
        SDL_Rect pointBar = {10, 10, points*(windowLength-20)/100, 10};
        SDL_SetRenderDrawColor(ren, 0, 0, 255, 255); // blue
        SDL_RenderFillRect(ren, &pointBar);
        //centre line
        SDL_Rect centerLine = {windowLength/2, 0, 2, windowHeight};
        SDL_SetRenderDrawColor(ren, 255, 0, 0,255); // red
        SDL_RenderFillRect(ren, &centerLine);

        // Show what we drew
        SDL_RenderPresent(ren);

        SDL_Delay(16);
    }

    SDL_DestroyRenderer(ren);
    SDL_DestroyWindow(win);
    SDL_Quit();
    return 0;
}
