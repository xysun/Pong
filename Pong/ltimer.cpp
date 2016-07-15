//
//  ltimer.cpp
//  Pong
//
//  Created by Sun, Joy (Agoda) on 7/15/2559 BE.
//  Copyright © 2559 Xiayun Sun. All rights reserved.
//

#include "ltimer.hpp"



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

