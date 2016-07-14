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
#include <SDL2_mixer/SDL_mixer.h>
#include <SDL2_ttf/SDL_ttf.h>
#include <string>


const int SCREEN_WIDTH = 640;
const int SCREENT_HEIGHT = 480;

// starts up SDL and create window
bool init();

// free media and shutdown SDL
void close();

bool checkCollision(SDL_Rect a, SDL_Rect b);
bool loadMedia();

// window
SDL_Window* gWindow = NULL;

// window renderer
SDL_Renderer* gRenderer = NULL;

// sound effects
Mix_Music* gMusic = NULL;
Mix_Chunk* gHit = NULL;



bool checkCollision(SDL_Rect a, SDL_Rect b){
    
    int leftA, leftB;
    int rightA, rightB;
    int topA, topB;
    int bottomA, bottomB;
    
    leftA = a.x;
    leftB = b.x;
    rightA = a.x + a.w;
    rightB = b.x + b.w;
    topA = a.y;
    topB = b.y;
    bottomA = a.y + a.h;
    bottomB = b.y + b.h;
    
    if (bottomA <= topB) {
        return false;
    }
    
    if (topA >= bottomB) {
        return false;
    }
    
    if (rightA <= leftB) {
        return false;
    }
    
    if (leftA >= rightB) {
        return false;
    }
    
    return true;
    
}


class LTexture{
public:
    LTexture();
    ~LTexture();
    
    bool loadFromRenderedText(std::string textureText, SDL_Color textColor);
    
    void free();
    
    void render(int x, int y);
    
private:
    SDL_Texture* mTexture;
    int mWidth;
    int mHeignt;
};

// font
TTF_Font* gFont = NULL;
LTexture gTextTexture;

LTexture::LTexture(){
    mTexture = NULL;
    mWidth = 0;
    mHeignt = 0;
}

LTexture::~LTexture(){
    free();
}

void LTexture::free(){
    if (mTexture != NULL) {
        SDL_DestroyTexture(mTexture);
        mTexture = NULL;
        mWidth = 0;
        mHeignt = 0;
    }
}





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

class Wall{
public:
    static const int WALL_WIDTH = 200;
    static const int WALL_HEIGHT = 20;
    
    static const int WALL_VEL = 5;
    
    Wall();
    
    void render();
    void handleEvent(SDL_Event& e);
    void move();
    SDL_Rect getCollider(){return mCollider;};
    
private:
    int mPosX, mPosY;
    int mVelX, mVelY;
    SDL_Rect mCollider;
    
};

Wall::Wall(){
    mPosX = 10;
    mPosY = SCREENT_HEIGHT - 2 * WALL_HEIGHT;
    
    mVelX = 0;
    mVelY = 0;
    
    mCollider.w = WALL_WIDTH;
    mCollider.h = WALL_HEIGHT;
    
}

void Wall::render(){
    SDL_Rect wallRect = {
        mPosX,
        mPosY,
        WALL_WIDTH,
        WALL_HEIGHT
    };
    SDL_SetRenderDrawColor(gRenderer, 0x00, 0x00, 0x00, 0x00);
    SDL_RenderDrawRect(gRenderer, &wallRect);
}

void Wall::handleEvent(SDL_Event& e){
    if (e.type == SDL_KEYDOWN && e.key.repeat == 0) {
        switch (e.key.keysym.sym) {
            case SDLK_LEFT: mVelX -= WALL_VEL; break;
            case SDLK_RIGHT: mVelX += WALL_VEL; break;
        }
    }
    
    else if (e.type == SDL_KEYUP && e.key.repeat == 0){
        switch (e.key.keysym.sym) {
            case SDLK_LEFT: mVelX += WALL_VEL; break;
            case SDLK_RIGHT: mVelX -= WALL_VEL; break;
        }
    }
}

void Wall::move(){
    mPosX += mVelX;
    
    if ((mPosX < 0) || (mPosX + WALL_WIDTH > SCREEN_WIDTH)) {
        mPosX -= mVelX;
    }
    
    mCollider.x = mPosX;
    
    mPosY += mVelY;
    
    if ((mPosY < 0) || (mPosY + WALL_HEIGHT > SCREENT_HEIGHT)) {
        mPosY -= mVelY;
    }
    
    mCollider.y = mPosY;
    
}


class Dot{
public:
    static const int DOT_WIDTH = 15;
    static const int DOT_HEIGHT = 15;
    
    static const int DOT_VEL = 5;
    
    Dot();
    
    void move(Wall& wall);
    void render();
    
private:
    int mPosX, mPosY;
    
    int mVelX, mVelY;
    
    SDL_Rect mCollider;
    
};



Dot::Dot(){
    mPosX = SCREEN_WIDTH / 2;
    mPosY = SCREENT_HEIGHT / 2;
    
    mCollider.w = DOT_WIDTH;
    mCollider.h = DOT_HEIGHT;
    
    
    // random initialize velocity in [0, DOT_VEL]
    srand (time(NULL));
    
    mVelX = (rand() % DOT_VEL) * (rand() % 2 ==1 ? -1:1);
    mVelY = (rand() % DOT_VEL) * (rand() % 2 ==1 ? -1:1);
    
    while (mVelY == 0) {
        mVelY = (rand() % DOT_VEL) * (rand() % 2 ==1 ? -1:1);// make sure we always have Y velocity, otherwise will never collide
    }
    
    while (mVelX == 0) {
        mVelX = (rand() % DOT_VEL) * (rand() % 2 ==1 ? -1:1);// make sure we're not purely on Y axis
    }
    
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

void Dot::move(Wall& wall){
    
    mPosX += mVelX;
    
    if ((mPosX < 0) || (mPosX + DOT_WIDTH > SCREEN_WIDTH)) {
        // bounce off
        mPosX -= 1 * mVelX;
        mVelX = -1 * mVelX;
        Mix_PlayChannel(-1, gHit, 0);
    }
    
    mCollider.x = mPosX;
    
    mPosY += mVelY;
    
    // if collide with wall, change Y (no need to change X)
    if ((mPosY < 0) || (mPosY + DOT_HEIGHT > SCREENT_HEIGHT)){
        mPosY -= 1 * mVelY;
        mVelY = -1 * mVelY;
        Mix_PlayChannel(-1, gHit, 0);
    }
    
    if (checkCollision(mCollider, wall.getCollider())) {
        mPosY -= 2 * mVelY;
        mVelY = -1 * mVelY;
        // play sound
        Mix_PlayChannel(-1, gHit, 0);
    }
    
    mCollider.y = mPosY;
    
}


bool init(){
    bool success = true;
    
    // initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0) {
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
            
            // initialize SDL_mixer
            if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
                printf("SDL mixer could not initialize! SDL mixer error: %s\n", Mix_GetError());
                success = false;
            }
        }
    }
    
    return success;
}

bool loadMedia(){
    bool success = true;
    
    // load music
    gMusic = Mix_LoadMUS("media/beat.wav");
    if (gMusic == NULL) {
        printf("failed to load beat music! SDL error: %s\n", Mix_GetError());
        success = false;
    }
    
    gHit = Mix_LoadWAV("media/scratch.wav");
    if (gMusic == NULL) {
        printf("failed to load scratch effect! SDL error: %s\n", Mix_GetError());
        success = false;
    }

    
    return success;
}

void close(){
    
    Mix_FreeChunk(gHit);
    gHit = NULL;
    Mix_FreeMusic(gMusic);
    gMusic = NULL;
    
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
        
        if (!loadMedia()) {
            printf("Failed to load media!\n");
        }
        else{
            bool quit = false;
            
            SDL_Event e;
            
            Dot dot;
            Wall wall;
            
            LTimer timer;
            timer.start();
            
            // play music
            Mix_PlayMusic(gMusic, -1);
            
            Uint32 t0 = timer.getTicks();
            
            while (!quit) {
                while (SDL_PollEvent(&e) != 0) {
                    if (e.type == SDL_QUIT) {
                        quit = true;
                    }
                    
                    wall.handleEvent(e);
                }
                
                // clear screen
                SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
                SDL_RenderClear(gRenderer);
                
                dot.move(wall);
                wall.move();
                
                // render
                dot.render();
                wall.render();
                
                // update screen
                SDL_RenderPresent(gRenderer);
            }
            
        }
        
    }
    
    close();
    
    return 0;
}
