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
    template<typename T>
    const T template_abs(const T &a)
    {
         return (a < 0) ? -a : a;
    }

    template<typename T>
    std::string valtostr(const T& val)
    {
        std::ostringstream sstream;
        sstream << val;
        return sstream.str();


    }
}
