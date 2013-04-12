#ifndef _sdlwrapper_h_included
#define _sdlwrapper_h_included

#include <SDL/SDL.h>
#include <vector>
#include <algorithm>
#include <string>
#include <sstream>
#include <iomanip>

namespace SDLwrapper
{

/*
 *   Templates
 *
 */

    template<typename T>
    const T template_abs(const T &abs)
    {
         return (abs < 0) ? -abs : abs;
    }

    template<typename T>
    std::string valtostr(const T& val)
    {
        std::ostringstream sstream;
        sstream << val;
        return sstream.str();
    }

    template<typename T>
    T strtoval(const std::string& s)
    {
        std:istringstream sstream(s);
        T val;
        sstream >> val;
        return val;
    }

    template<typename T>
    std::string valtostr(const T& val, int length, bool fixed = true)
    {
        std::ostringstream sstream;

        if(fixed)
        {
            sstream << std::fixed;
        }

        sstream << std::setprecision(length) << val;
        return sstream.str();
    }


/*
 *   Color structs
 *
 */

    struct ColorRGB8bit;

    //a color with 3 components: r, g and b
    struct ColorRGB
    {
        int r;
        int g;
        int b;

        ColorRGB(Uint8 r, Uint8 g, Uint8 b);
        ColorRGB(const ColorRGB8bit& color);
        ColorRGB();
    };

    ColorRGB operator+(const ColorRGB& color, const ColorRGB& color2);
    ColorRGB operator-(const ColorRGB& color, const ColorRGB& color2);
    ColorRGB operator*(const ColorRGB& color, int a);
    ColorRGB operator*(int a, const ColorRGB& color);
    ColorRGB operator/(const ColorRGB& color, int a);
    bool operator==(const ColorRGB& color, const ColorRGB& color2);
    bool operator!=(const ColorRGB& color, const ColorRGB& color2);

    static const ColorRGB RGB_Black    (  0,   0,   0);
    static const ColorRGB RGB_Red      (255,   0,   0);
    static const ColorRGB RGB_Green    (  0, 255,   0);
    static const ColorRGB RGB_Blue     (  0,   0, 255);
    static const ColorRGB RGB_Cyan     (  0, 255, 255);
    static const ColorRGB RGB_Magenta  (255,   0, 255);
    static const ColorRGB RGB_Yellow   (255, 255,   0);
    static const ColorRGB RGB_White    (255, 255, 255);
    static const ColorRGB RGB_Gray     (128, 128, 128);
    static const ColorRGB RGB_Grey     (192, 192, 192);
    static const ColorRGB RGB_Maroon   (128,   0,   0);
    static const ColorRGB RGB_Darkgreen(  0, 128,   0);
    static const ColorRGB RGB_Navy     (  0,   0, 128);
    static const ColorRGB RGB_Teal     (  0, 128, 128);
    static const ColorRGB RGB_Purple   (128,   0, 128);
    static const ColorRGB RGB_Olive    (128, 128,   0);

    //a color with 3 components: r, g and b
    struct ColorRGB8bit
    {
        Uint8 r;
        Uint8 g;
        Uint8 b;

        ColorRGB8bit(Uint8 r, Uint8 g, Uint8 b);
        ColorRGB8bit(const ColorRGB& color);
        ColorRGB8bit();
    };
  
    //a color with 3 components: h, s and l
    struct ColorHSL
    {
        int h;
        int s;
        int l;

        ColorHSL(Uint8 h, Uint8 s, Uint8 l);
        ColorHSL();
    };

    //a color with 3 components: h, s and v
    struct ColorHSV
    {
        int h;
        int s;
        int v;
  
        ColorHSV(Uint8 h, Uint8 s, Uint8 v);
        ColorHSV();
    };


/*
 *   Global variables
 *
 */

    extern int w;  /* width  */
    extern int h;  /* height */

/*
 *  Keyboard action listeners
 *
 */
    bool keyDown(int key);
    bool keyPressed(int key);

/*
 *  Functions for screen
 *
 */
    void screen(int width = 640, int height = 400, bool fullscreen = 0, const std::string& text = " ");
    void lock();
    void unlock();
    void redraw();
    void cls(const ColorRGB& color = RGB_Black);
    void pset(int x, int y, const ColorRGB& color);
    ColorRGB pget(intx, int y);
    void drawBuffer(Uint32 *buffer);
    bool onScreen(int x, int y);



}
