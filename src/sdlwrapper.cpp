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


    /* Keyboard functions */
    bool keyDown(int key) /* returns true while key is held down */
    {
        return (inkeys[key] != 0);
    }

    bool keyPressed(int key) //returns true only once until key is released
    {
        if(keypressed.find(key) == keypressed.end())
        {
            keypressed[key] = false;
        }

        if(inkeys[key])
        {
            if(keypressed[key] == false)
            {
                keypressed[key] = true;
                return true;
            }
        }
        else
        {
            keypressed[key] = false;
        }

        return false;
    }


}
