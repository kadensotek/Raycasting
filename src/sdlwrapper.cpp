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
 *  Keyboard action listeners
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

/*
 *  2D Shapes
 *
 */
    //Fast horizontal line from (x1,y) to (x2,y), with rgb color
    bool horLine(int y, int x1, int x2, const ColorRGB& color)
    {
        /* Swap x1 and x2; x1 must be leftmost endpoint */
        if(x2 < x1)
        {
            x1 += x2;
            x2 = x1 - x2;
            x1 -= x2;
        }

        /* no single point of the line is on screen */
        if(x2 < 0 || x1 >= w || y < 0 || y >= h)
        {
            return 0;
        }

        if(x1 < 0)
        {
            x1 = 0;
        }

        if(x2 >= w)
        {
            x2 = w - 1;
        }
  
        Uint32 colorSDL = SDL_MapRGB(scr->format, color.r, color.g, color.b);
        Uint32* bufp;
        bufp = (Uint32*)scr->pixels + y * scr->pitch / 4 + x1;

        for(int x = x1; x <= x2; x++)
        {
            *bufp = colorSDL;
            bufp++;
        }

        return 1;
    }

    //Fast vertical line from (x,y1) to (x,y2), with rgb color
    bool verLine(int x, int y1, int y2, const ColorRGB& color)
    {
        /* Swa[ y1 and y2 */
        if(y2 < y1)
        {
            y1 += y2; y2 = y1 - y2; y1 -= y2;
        }

        if(y2 < 0 || y1 >= h  || x < 0 || x >= w)
        {
            return 0;
        }

        if(y1 < 0)
        {
            y1 = 0;
        }

        if(y2 >= w)
        {
            y2 = h - 1;
        }
   
        Uint32 colorSDL = SDL_MapRGB(scr->format, color.r, color.g, color.b);
        Uint32* bufp;
  
        bufp = (Uint32*)scr->pixels + y1 * scr->pitch / 4 + x;

        for(int y = y1; y <= y2; y++)
        {
            *bufp = colorSDL;
            bufp += scr->pitch / 4;
        }

        return 1;
    }

    //Bresenham line from (x1,y1) to (x2,y2) with rgb color
    bool drawLine(int x1, int y1, int x2, int y2, const ColorRGB& color)
    {
        if(x1 < 0 || x1 > w - 1 || x2 < 0 || x2 > w - 1 || y1 < 0 || y1 > h - 1 || y2 < 0 || y2 > h - 1)
        {
            return 0;
        }
  
        int deltax = std::abs(x2 - x1); //The difference between the x's
        int deltay = std::abs(y2 - y1); //The difference between the y's
        int x = x1; //Start x off at the first pixel
        int y = y1; //Start y off at the first pixel
        int xinc1, xinc2, yinc1, yinc2, den, num, numadd, numpixels, curpixel;

        if(x2 >= x1) //The x-values are increasing
        {
            xinc1 = 1;
            xinc2 = 1;
        }
        else //The x-values are decreasing
        {
            xinc1 = -1;
            xinc2 = -1;
        }

        if(y2 >= y1) //The y-values are increasing
        {
            yinc1 = 1;
            yinc2 = 1;
        }
        else //The y-values are decreasing
        {
            yinc1 = -1;
            yinc2 = -1;
        }

        if(deltax >= deltay) //There is at least one x-value for every y-value
        {
            xinc1 = 0; //Don't change the x when numerator >= denominator
            yinc2 = 0; //Don't change the y for every iteration
            den = deltax;
            num = deltax / 2;
            numadd = deltay;
            numpixels = deltax; //There are more x-values than y-values
        }
        else //There is at least one y-value for every x-value
        {
            xinc2 = 0; //Don't change the x for every iteration
            yinc1 = 0; //Don't change the y when numerator >= denominator
            den = deltay;
            num = deltay / 2;
            numadd = deltax;
            numpixels = deltay; //There are more y-values than x-values
        }

        for (curpixel = 0; curpixel <= numpixels; curpixel++)
        {
            pset(x % w, y % h, color);  //Draw the current pixel
            num += numadd;  //Increase the numerator by the top of the fraction

            if (num >= den) //Check if numerator >= denominator
            {
                num -= den; //Calculate the new numerator value
                x += xinc1; //Change the x as appropriate
                y += yinc1; //Change the y as appropriate
            }

            x += xinc2; //Change the x as appropriate
            y += yinc2; //Change the y as appropriate
        }
  
        return 1;
    }

    //Bresenham circle with center at (xc,yc) with radius and red green blue color
    bool drawCircle(int xc, int yc, int radius, const ColorRGB& color)
    {
        if(xc - radius < 0 || xc + radius >= w || yc - radius < 0 || yc + radius >= h) return 0;

        int x = 0;
        int y = radius;
        int p = 3 - (radius << 1);
        int a, b, c, d, e, f, g, h;

        while (x <= y)
        {
            a = xc + x; //8 pixels can be calculated at once thanks to the symmetry
            b = yc + y;
            c = xc - x;
            d = yc - y;
            e = xc + y;
            f = yc + x;
            g = xc - y;
            h = yc - x;
            pset(a, b, color);
            pset(c, d, color);
            pset(e, f, color);
            pset(g, f, color);

            if(x > 0) //avoid drawing pixels at same position as the other ones
            {
                pset(a, d, color);
                pset(c, b, color);
                pset(e, h, color);
                pset(g, h, color);
            }

            if(p < 0)
            {
                p += (x++ << 2) + 6;
            }
            else
            {
                p += ((x++ - y--) << 2) + 10;
            }
        }
  
        return 1;
    }

    //Filled bresenham circle with center at (xc,yc) with radius and red green blue color
    bool drawDisk(int xc, int yc, int radius, const ColorRGB& color)
    {
        //every single pixel outside screen, so don't waste time on it
        if(xc + radius < 0 || xc - radius >= w || yc + radius < 0 || yc - radius >= h)
        {
            return 0;
        }

        int x = 0;
        int y = radius;
        int p = 3 - (radius << 1);
        int a, b, c, d, e, f, g, h;
        int pb = yc + radius + 1, pd = yc + radius + 1; //previous values: to avoid drawing horizontal lines multiple times

        while (x <= y)
        {
            // write data
            a = xc + x;
            b = yc + y;
            c = xc - x;
            d = yc - y;
            e = xc + y;
            f = yc + x;
            g = xc - y;
            h = yc - x;

            if(b != pb)
            {
                horLine(b, a, c, color);
            }

            if(d != pd)
            {
                horLine(d, a, c, color);
            }

            if(f != b)
            {
                horLine(f, e, g, color);
            }

            if(h != d && h != f)
            {
                horLine(h, e, g, color);
            }

            pb = b;
            pd = d;

            if(p < 0)
            {
                p += (x++ << 2) + 6;
            }
            else
            {
                p += ((x++ - y--) << 2) + 10;
            }
        }
  
        return 1;
    }

    //Rectangle with corners (x1,y1) and (x2,y2) and rgb color
    bool drawRect(int x1, int y1, int x2, int y2, const ColorRGB& color)
    {
        if(x1 < 0 || x1 > w - 1 || x2 < 0 || x2 > w - 1 || y1 < 0 || y1 > h - 1 || y2 < 0 || y2 > h - 1) return 0;
        SDL_Rect rec;
        rec.x = x1;
        rec.y = y1;
        rec.w = x2 - x1 + 1;
        rec.h = y2 - y1 + 1;
        Uint32 colorSDL = SDL_MapRGB(scr->format, color.r, color.g, color.b);
        SDL_FillRect(scr, &rec, colorSDL);  //SDL's ability to draw a hardware rectangle is used for now
        return 1;
    }

    //Functions for clipping a 2D line to the screen, which is the rectangle (0,0)-(w,h)
    //This is the Cohen-Sutherland Clipping Algorithm
    //Each of 9 regions gets an outcode, based on if it's at the top, bottom, left or right of the screen
    // 1001 1000 1010  9 8 10
    // 0001 0000 0010  1 0 2
    // 0101 0100 0110  5 4 6
    //int findregion returns which of the 9 regions a point is in, void clipline does the actual clipping
    int findRegion(int x, int y)
    {
        int code=0;

        if(y >= h)
        {
            code |= 1; //top
        }
        else if( y < 0)
        {
            code |= 2; //bottom
        }

        if(x >= w)
        {
            code |= 4; //right
        }
        else if ( x < 0)
        {
            code |= 8; //left
        }

        return(code);
    }

    bool clipLine(int x1, int y1, int x2, int y2, int & x3, int & y3, int & x4, int & y4)
    {
        int code1, code2, codeout;
        bool accept = 0, done=0;
        code1 = findRegion(x1, y1); //the region outcodes for the endpoints
        code2 = findRegion(x2, y2);

        do  //In theory, this can never end up in an infinite loop, it'll always come in one of the trivial cases eventually
        {
            if(!(code1 | code2))
            {
                accept = done = 1;  //accept because both endpoints are in screen or on the border, trivial accept
            {
            else if(code1 & code2)
            {
                done = 1; //the line isn't visible on screen, trivial reject
            }
            else  //if no trivial reject or accept, continue the loop
            {
                int x, y;
                codeout = code1 ? code1 : code2;

                if(codeout & 1) //top
                {
                    x = x1 + (x2 - x1) * (h - y1) / (y2 - y1);
                    y = h - 1;
                }
                else if(codeout & 2) //bottom
                {
                    x = x1 + (x2 - x1) * -y1 / (y2 - y1);
                    y = 0;
                }
                else if(codeout & 4) //right
                {
                    y = y1 + (y2 - y1) * (w - x1) / (x2 - x1);
                    x = w - 1;
                }
                else //left
                {
                    y = y1 + (y2 - y1) * -x1 / (x2 - x1);
                    x = 0;
                }

                if(codeout == code1) //first endpoint was clipped
                {
                    x1 = x; y1 = y;
                    code1 = findRegion(x1, y1);
                }
                else //second endpoint was clipped
                {
                    x2 = x; y2 = y;
                    code2 = findRegion(x2, y2);
                }
            }
        } while(done == 0);

        if(accept)
        {
            x3 = x1;
            x4 = x2;
            y3 = y1;
            y4 = y2;
            return 1;
        }
        else
        {
            x3 = x4 = y3 = y4 = 0;
            return 0;
        }
    }

/*
 *  Color structs
 *
 */
    ColorRGB::ColorRGB(Uint8 r, Uint8 g, Uint8 b)
    {
        this->r = r;
        this->g = g;
        this->b = b;
    }

    ColorRGB::ColorRGB(const ColorRGB8bit& color)
    {
        this->r = color.r;
        this->g = color.g;
        this->b = color.b;
    }

    ColorRGB::ColorRGB()
    {
        this->r = 0;
        this->g = 0;
        this->b = 0;
    }

    ColorRGB8bit::ColorRGB8bit(Uint8 r, Uint8 g, Uint8 b)
    {
        this->r = r;
        this->g = g;
        this->b = b;
    }

    ColorRGB8bit::ColorRGB8bit(const ColorRGB& color)
    {
        this->r = color.r;
        this->g = color.g;
        this->b = color.b;
    }

    ColorRGB8bit::ColorRGB8bit()
    {
        this->r = 0;
        this->g = 0;
        this->b = 0;
    }

    //Add two colors
    ColorRGB operator+(const ColorRGB& color, const ColorRGB& color2)
    {
        ColorRGB c;
        c.r = color.r + color2.r;
        c.g = color.g + color2.g;
        c.b = color.b + color2.b;
        return c;
    }

    //Subtract two colors
    ColorRGB operator-(const ColorRGB& color, const ColorRGB& color2)
    {
        ColorRGB c;
        c.r = color.r - color2.r;
        c.g = color.g - color2.g;
        c.b = color.b - color2.b;
        return c;
    }

    //Multiplies a color with an integer
    ColorRGB operator*(const ColorRGB& color, int a)
    {
        ColorRGB c;
        c.r = color.r * a;
        c.g = color.g * a;
        c.b = color.b * a;
        return c;
    }

    //Multiplies a color with an integer
    ColorRGB operator*(int a, const ColorRGB& color)
    {
        ColorRGB c;
        c.r = color.r * a;
        c.g = color.g * a;
        c.b = color.b * a;
        return c;
    }

    //Divides a color through an integer
    ColorRGB operator/(const ColorRGB& color, int a)
    {
        if(a == 0) return color;
        ColorRGB c;
        c.r = color.r / a;
        c.g = color.g / a;
        c.b = color.b / a;
        return c;
    }

    //Are both colors equal?
    bool operator==(const ColorRGB& color, const ColorRGB& color2)
    {
        return(color.r == color2.r && color.g == color2.g && color.b == color2.b);
    }

    //Are both colors not equal?
    bool operator!=(const ColorRGB& color, const ColorRGB& color2)
    {
        return(!(color.r == color2.r && color.g == color2.g && color.b == color2.b));
    }

    ColorHSL::ColorHSL(Uint8 h, Uint8 s, Uint8 l)
    {
        this->h = h;
        this->s = s;
        this->l = l;
    }

    ColorHSL::ColorHSL()
    {
        this->h = 0;
        this->s = 0;
        this->l = 0;
    }

    ColorHSV::ColorHSV(Uint8 h, Uint8 s, Uint8 v)
    {
        this->h = h;
        this->s = s;
        this->v = v;
    }

    ColorHSV::ColorHSV()
    {
        this->h = 0;
        this->s = 0;
        this->v = 0;
    }







}
