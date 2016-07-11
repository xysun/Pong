//
//  main.cpp
//  Pong
//
//  Created by Sun, Joy (Agoda) on 7/11/2559 BE.
//  Copyright © 2559 Xiayun Sun. All rights reserved.
//

#include <SDL2/SDL.h>
#include <iostream>


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
            gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED);
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
        
        while (!quit) {
            while (SDL_PollEvent(&e) != 0) {
                if (e.type == SDL_QUIT) {
                    quit = true;
                }
            }
            
            // clear screen
            SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
            SDL_RenderClear(gRenderer);
            
            // TODO:  draw small red rect as bouncing circle
            SDL_Rect circleRect = {
                SCREEN_WIDTH / 2,
                SCREENT_HEIGHT / 2,
                15,
                15
            };
            SDL_SetRenderDrawColor(gRenderer, 0xFF, 0x00, 0x00, 0xFF);
            SDL_RenderFillRect(gRenderer, &circleRect);
            
            // update screen
            SDL_RenderPresent(gRenderer);
        }
    }
    
    close();
    
    return 0;
}
