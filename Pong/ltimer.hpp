//
//  ltimer.hpp
//  Pong
//
//  Created by Sun, Joy (Agoda) on 7/15/2559 BE.
//  Copyright © 2559 Xiayun Sun. All rights reserved.
//

#ifndef ltimer_hpp
#define ltimer_hpp

#include <SDL2/SDL.h>
#include <iostream>
#include <stdlib.h>
#include <string>

#endif /* ltimer_hpp */

class LTimer{
public:
    LTimer();
    
    void start();
    
    Uint32 getTicks();
    
private:
    Uint32 mStartTicks;
    
    bool mStarted;
};
