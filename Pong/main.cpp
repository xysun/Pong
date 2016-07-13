//
//  main.cpp
//  Pong
//
//  Created on 7/11/2016 BE.
//  Copyright © 2016 Xiayun Sun. All rights reserved.
//

#include <SDL2/SDL.h>
#include <iostream>
#include <stdlib.h>


const int SCREEN_WIDTH = 640;
const int SCREENT_HEIGHT = 480;

// starts up SDL and create window
bool init();

// free media and shutdown SDL
void close();

// window
SDL_Window* gWindow = NULL;

// window renderer
SDL_Renderer* gRenderer = NULL;


class LTimer{
public:
    LTimer();
    
    void start();
    
    Uint32 getTicks();
    
private:
    Uint32 mStartTicks;
    
    bool mStarted;
};

LTimer::LTimer(){
    mStarted = false;
    mStartTicks = 0;
}

void LTimer::start(){
    mStarted = true;
    mStartTicks = SDL_GetTicks();
}

Uint32 LTimer::getTicks(){
    Uint32 time = 0;
    if (mStarted) {
        time = SDL_GetTicks() - mStartTicks;
    }
    return time;
}

class Dot{
public:
    static const int DOT_WIDTH = 15;
    static const int DOT_HEIGHT = 15;
    
    static const int DOT_VEL = 5;
    
    static const int DOT_MOVE = 80; // move per 500 ticks
    
    Dot();
    
    void move();
    void render();
    
private:
    int mPosX, mPosY;
    
    int mVelX, mVelY;
};

Dot::Dot(){
    mPosX = SCREEN_WIDTH / 2;
    mPosY = SCREENT_HEIGHT / 2;
    
    // random initialize velocity in [0, DOT_VEL]
    srand (time(NULL));
    
    mVelX = (rand() % DOT_VEL) * (rand() % 2 ==1 ? -1:1);
    mVelY = (rand() % DOT_VEL) * (rand() % 2 ==1 ? -1:1);
    
    printf("mVelX: %d\n", mVelX);
    printf("mVelY: %d\n", mVelY);
}

void Dot::render(){
    SDL_Rect circleRect = {
        mPosX,
        mPosY,
        DOT_WIDTH,
        DOT_HEIGHT
    };
    SDL_SetRenderDrawColor(gRenderer, 0xFF, 0x00, 0x00, 0xFF);
    SDL_RenderFillRect(gRenderer, &circleRect);
}

void Dot::move(){

    mPosX += mVelX;
    
    if ((mPosX < 0) || (mPosX + DOT_WIDTH > SCREEN_WIDTH)) {
        // bounce off
        mPosX -= mVelX;
        mVelX = -1 * mVelX;
    }
    
    mPosY += mVelY;
    
    if ((mPosY < 0) || (mPosY + DOT_HEIGHT > SCREENT_HEIGHT)) {
        mPosY -= mVelY;
        mVelY = -1 * mVelY;
    }
}


bool init(){
    bool success = true;
    
    // initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("SDL could not initialize! SDL error: %s\n", SDL_GetError());
        success = false;
    }
    else{
        // create window
        gWindow = SDL_CreateWindow("Pong", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREENT_HEIGHT, SDL_WINDOW_SHOWN);
        
        if (gWindow == NULL) {
            printf("Window could not be created! SDL Error: %s\n", SDL_GetError());
            success = false;
        }
        else{
            // create renderer
            gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
            if (gRenderer == NULL){
                printf("Renderer could not be created! SDL Error: %s\n", SDL_GetError());
                success = false;
            }
            else{
                // initialize renderer color
                SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
            }
        }
    }
    
    return success;
}

void close(){
    SDL_DestroyRenderer(gRenderer);
    SDL_DestroyWindow(gWindow);
    gWindow = NULL;
    gRenderer = NULL;
    
    SDL_Quit();
}


int main(int argc, const char * argv[]) {
    
    if (!init()) {
        printf("Failed to initialize!\n");
    }
    else{
        bool quit = false;
        
        SDL_Event e;
        
        Dot dot;
        
        LTimer timer;
        timer.start();
        
        Uint32 t0 = timer.getTicks();
        
        while (!quit) {
            while (SDL_PollEvent(&e) != 0) {
                if (e.type == SDL_QUIT) {
                    quit = true;
                }
            }
            
            // clear screen
            SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
            SDL_RenderClear(gRenderer);
            
            dot.move();
            
            // render
            dot.render();
            
            // update screen
            SDL_RenderPresent(gRenderer);
        }
    }
    
    close();
    
    return 0;
}
