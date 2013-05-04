#ifndef _sdlwrapper_h_included
#define _sdlwrapper_h_included
#endif

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
        std::istringstream sstream(s);
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
    ColorRGB pget(int x, int y);
    void drawBuffer(Uint32 *buffer);
    bool onScreen(int x, int y);

/*
 *  Other functions
 *
 */
    void sleep();
    void waitFrame(double oldTime, double frameDuration); /* seconds */
    bool done(bool quit_if_esc = true, bool delay = true);
    void end();
    void readKeys();
    void getMouseState(int& mouseX, int& mouseY);
    void getMouseState(int& mouseX, int& mouseY, bool& LMB, bool& RMB);
    unsigned long getTicks(); /* in milliseconds */
    inline double getTime()
    {
         return getTicks() / 1000.0;
    }


/*
 *  2d shape functions
 *
 */
    bool horLine(int y, int x1, int x2, const ColorRGB& color);
    bool verLine(int x, int y1, int y2, const ColorRGB& color);
    bool drawLine(int x1, int y1, int x2, int y2, const ColorRGB& color);
    bool drawCircle(int xc, int yc, int radius, const ColorRGB& color);
    bool drawDisk(int xc, int yc, int radius, const ColorRGB& color);
    bool drawRect(int x1, int y1, int x2, int y2, const ColorRGB& color);
    bool clipLine(int x1,int y1,int x2, int y2, int & x3, int & y3, int & x4, int & y4);


/*
 *  Color conversion functions
 *
 */
    ColorHSL RGBtoHSL(const ColorRGB& colorRGB);
    ColorRGB HSLtoRGB(const ColorHSL& colorHSL);
    ColorHSV RGBtoHSV(const ColorRGB& colorRGB);
    ColorRGB HSVtoRGB(const ColorHSV& colorHSV);
    Uint32 RGBtoINT(const ColorRGB& colorRGB);
    ColorRGB INTtoRGB(Uint32 colorINT);


/*
 *  File functions
 *
 */
    void loadFile(std::vector<unsigned char>& buffer, const std::string& filename);
    void saveFile(const std::vector<unsigned char>& buffer, const std::string& filename);


/*
 *  Image functions
 *
 */
    int loadImage(std::vector<ColorRGB>& out, unsigned long& w, unsigned long& h, const std::string& filename);
    int loadImage(std::vector<Uint32>& out, unsigned long& w, unsigned long& h, const std::string& filename);
    int decodePNG(std::vector<unsigned char>& out_image_32bit, unsigned long& image_width, unsigned long& image_height, const unsigned char* in_png, unsigned long in_size);
    int decodePNG(std::vector<unsigned char>& out_image_32bit, unsigned long& image_width, unsigned long& image_height, const std::vector<unsigned char>& in_png);


/*
 *  Text functions
 *
 */
    extern bool font[256][8][8];
    void drawLetter(unsigned char n, int x, int y, const ColorRGB& color = RGB_White, bool bg = 0, const ColorRGB& color2 = RGB_Black);
    int printString(const std::string& text, int x = 0, int y = 0, const ColorRGB& color = RGB_White, bool bg = 0, const ColorRGB& color2 = RGB_Black, int forceLength = 0);

    //print something (string, int, float, ...)
    template<typename T>
    int print(const T& val, int x = 0, int y = 0, const ColorRGB& color = RGB_White, bool bg = 0, const ColorRGB& color2 = RGB_Black, int forceLength = 0)
    {
        std::string text = valtostr(val);
        return printString(text, x, y, color, bg, color2, forceLength);
    }

    //print some floating point number, this one allows printing floating point numbers with limited length
    template<typename T>
    int fprint(const T& val, int length, int x = 0, int y = 0, const ColorRGB& color = RGB_White, bool bg = 0, const ColorRGB& color2 = RGB_Black, int forceLength = 0)
    {
        std::string text = valtostr(val, length, true);
        return printString(text, x, y, color, bg, color2, forceLength);
    }

/*
 *  Text input functions
 *
 */
    Uint8 getInputCharacter();
    void getInputString(std::string& text, const std::string& message = "", bool clear = false, int x = 0, int y = 0, const ColorRGB& color = RGB_White, bool bg = 0, const ColorRGB& color2 = RGB_Black);

    template<typename T>
    T getInput(const std::string& message = "", bool clear = false, int x = 0, int y = 0, const ColorRGB& color = RGB_White, bool bg = 0, const ColorRGB& color2 = RGB_Black)
    {
        std::string text;
        getInputString(text, message, clear, x, y, color, bg, color2);
        return strtoval<T>(text);
    }
}
