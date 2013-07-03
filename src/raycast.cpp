#include <cmath>
#include <string>
#include <vector>
#include <iostream>

#include "sdlwrapper.h"
using namespace SDLwrapper;

#define screenWidth 640
#define screenHeight 480
#define texWidth 64
#define texHeight 64
#define mapWidth 10
#define mapHeight 10 

int worldMap[mapWidth][mapHeight]=
{
    {1,1,1,1,1,1,1,1,1,1}
    {1,0,0,0,0,0,0,0,0,1}
    {1,0,0,0,0,0,0,0,0,1}
    {1,0,0,0,0,0,0,0,0,1}
    {1,0,0,0,0,0,0,0,0,1}
    {1,0,0,0,0,0,0,0,0,1}
    {1,0,0,0,0,0,0,0,0,1}
    {1,0,0,0,0,0,0,0,0,1}
    {1,0,0,0,0,0,0,0,0,1}
    {1,1,1,1,1,1,1,1,1,1}
}

int main()
{
    double posX = 22;
    double posY = 12;
    double dirX = -1;
    double dirY = 0;
    double planeX = 0;
    double planeY = .66;

    double time = 0;
    double oldTime = 0;

    screen(512, 384, 0, "Raycaster");

    while(!done())
    {
        /* ray position and direction */
        double cameraX = 2*x / double(w)-1;
        double rayPosX = posX;
        double rayPosY = posY;
        double rayDirX = dirX + planeX * cameraX;
        double rayDirY = dirY + planeY * cameraX;
        int mapX = int(rayPosX);
        int mapY = int(rayPosY);

        /* used to figure out how far next X or Y side is from current position */
        double sideDistX;
        double sideDistY;

        /* length of ray between one X or Y side to next X or Y side */
        double deltaDistX = sqrt(1 + (rayDirY * rayDirY) / (rayDirX * rayDirX));
        double deltaDistY = sqrt(1 + (rayDirX * rayDirX) / (rayDirY * rayDirY));
        double perpWallDist;

        /* handles timing/FPS */
        oldTime = time;
        time = getTicks();
        double frameTime = (time - oldTime) / 1000.0; /* time for frame in seconds */
        print(1.0 / frameTime);  /* prints FPS counter  */
        redraw();
        cls();
    }

    return 0;
}
