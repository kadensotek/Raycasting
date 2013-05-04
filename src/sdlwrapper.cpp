#include "sdlwrapper.h"

#include <SDL/SDL.h>
#include <cstdlib>
#include <cmath>
#include <vector>
#include <map>
#include <iostream>
#include <fstream>

namespace sdlwrapper
{

    /* Variables */
    int w; //screen width
    int h; //screen height

    std::map<int, bool> keypressed; //if key is pressed and released
    SDL_Surface* scr; //SDL surface
    Uint8* inkeys;
    SDL_Event event = {0};



}
