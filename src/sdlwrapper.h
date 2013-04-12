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

    
}
