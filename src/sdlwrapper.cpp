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

/*
 *   Variables
 *
 */
    int w; //screen width
    int h; //screen height

    std::map<int, bool> keypressed; //if key is pressed and released
    SDL_Surface* scr; //SDL surface
    Uint8* inkeys;
    SDL_Event event = {0};


/*
 *  Keyboard functions
 *
 */
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

/*
 *   Basic screen functions
 *
 */

    /* initializes SDL */
    /* fullscreen = 0 for window; fullscreen = 1 for fullscreen */
    void screen(int width, int height, bool fullscreen, const std::string& text)
    {
        int colorDepth = 32;
        w = width;
        h = height;

        if(SDL_Init(SDL_INIT_EVERYTHING) < 0)
        {
            printf("Unable to init SDL: %s\n", SDL_GetError());
            SDL_Quit();
            std::exit(1);
        }

        std::atexit(SDL_Quit);

        if(fullscreen)
        {
            scr = SDL_SetVideoMode(width, height, colorDepth, SDL_SWSURFACE | SDL_FULLSCREEN);
            lock();
        }
        else
        {
            scr = SDL_SetVideoMode(width, height, colorDepth, SDL_HWSURFACE | SDL_HWPALETTE);
        }

        if(scr == NULL)
        {
            printf("Unable to set video: %s\n", SDL_GetError());
            SDL_Quit();
            std::exit(1);
        }

        SDL_WM_SetCaption(text.c_str(), NULL);

        SDL_EnableUNICODE(1); //for the text input things
    }

    //Locks the screen
    void lock()
    {
        if(SDL_MUSTLOCK(scr))
        if(SDL_LockSurface(scr) < 0)
        return;
    }

    //Unlocks the screen
    void unlock()
    {
        if(SDL_MUSTLOCK(scr))
        {
            SDL_UnlockSurface(scr);
        }
    }

    //Updates the screen.
    void redraw()
    {
        SDL_UpdateRect(scr, 0, 0, 0, 0);
    }

    //Blacks out screen
    void cls(const ColorRGB& color)
    {
        SDL_FillRect(scr, NULL, 65536 * color.r + 256 * color.g + color.b);
    }

    //Puts an RGB color pixel at position x,y
    void pset(int x, int y, const ColorRGB& color)
    {
        if(x < 0 || y < 0 || x >= w || y >= h) return;
        Uint32 colorSDL = SDL_MapRGB(scr->format, color.r, color.g, color.b);
        Uint32* bufp;
        bufp = (Uint32*)scr->pixels + y * scr->pitch / 4 + x;
        *bufp = colorSDL;
    }

    //Gets RGB color of pixel at position x,y
    ColorRGB pget(int x, int y)
    {
        if(x < 0 || y < 0 || x >= w || y >= h) return RGB_Black;
        Uint32* bufp;
        bufp = (Uint32*)scr->pixels + y * scr->pitch / 4 + x;
        Uint32 colorSDL = *bufp;
        ColorRGB8bit colorRGB;
        SDL_GetRGB(colorSDL, scr->format, &colorRGB.r, &colorRGB.g, &colorRGB.b);
        return ColorRGB(colorRGB);
    }

    //Draws a buffer of pixels to the screen
    void drawBuffer(Uint32* buffer)
    {
        Uint32* bufp;
        bufp = (Uint32*)scr->pixels;
  
        for(int y = 0; y < h; y++)
        {
            for(int x = 0; x < w; x++)
            {
                *bufp=buffer[h * x + y];
                bufp++;
            }

            bufp += scr->pitch / 4;
            bufp -= w;
        }
    }

    void getScreenBuffer(std::vector<Uint32>& buffer)
    {
        Uint32* bufp;
        bufp = (Uint32*)scr->pixels;

        buffer.resize(w * h);
  
        for(int y = 0; y < h; y++)
        {
            for(int x = 0; x < w; x++)
            {
                buffer[h * x + y] = *bufp;
                bufp++;
            }

            bufp += scr->pitch / 4;
            bufp -= w;
        }
    }

    bool onScreen(int x, int y)
    {
        return (x >= 0 && y >= 0 && x < w && y < h);
    }

/*
 *  Non-graphical functions
 *
 */
    void sleep()
    {
        int done = 0;

        while(done == 0)
        {
            while(SDL_PollEvent(&event))
            {
                if(event.type == SDL_QUIT)
                {
                    end();
                }

                if(event.type == SDL_KEYDOWN)
                {
                    done = 1;
                }
            }

            SDL_Delay(5); //so it consumes less processing power
        }
    }

    void waitFrame(double oldTime, double frameDuration) //in seconds
    {
        float time = getTime();

        while(time - oldTime < frameDuration)
        {
            time = getTime();
            SDL_PollEvent(&event);

            if(event.type == SDL_QUIT)
            {
                end();
            }

            inkeys = SDL_GetKeyState(NULL);

            if(inkeys[SDLK_ESCAPE])
            {
                end();
            }

            SDL_Delay(5); //so it consumes less processing power
        }
    }

    bool done(bool quit_if_esc, bool delay) //delay makes CPU have some free time, use once per frame to avoid 100% usage of a CPU core
    {
        if(delay)
        {
            SDL_Delay(5);
        }

        int done = 0;

        if(!SDL_PollEvent(&event))
        {
            return 0;
        }

        readKeys();

        if(quit_if_esc && inkeys[SDLK_ESCAPE])
        {
            done = 1;
        }

        if(event.type == SDL_QUIT)
        {
            done = 1;
        }

        return done;
    }

    void end()
    {
        SDL_Quit();
        std::exit(1);
    }

    void readKeys()
    {
        inkeys = SDL_GetKeyState(NULL);
    }

    void getMouseState(int& mouseX, int& mouseY, bool& LMB, bool& RMB)
    {
        Uint8 mouseState = SDL_GetMouseState(&mouseX, &mouseY);
  
        if(mouseState & 1)
        {
            LMB = true; 
        }
        else
        {
            LMB = false;
        }

        if(mouseState & 4)
        {
            RMB = true;
        }
        else
        {
            RMB = false;
        }
    }  

    //Returns the time since program start in milliseconds
    unsigned long getTicks()
    {
        return SDL_GetTicks();
    }





}
