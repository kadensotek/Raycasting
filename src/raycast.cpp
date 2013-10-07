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
#define mapWidth 25 
#define mapHeight 25 

int worldMap[mapWidth][mapHeight]=
{
    {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
};

int main()
{
    double posX = 1.1;
    double posY = 1.1;
    double dirX = -1;
    double dirY = 0;
    double planeX = 0;
    double planeY = .66;

    double time = 0;
    double oldTime = 0;

    screen(512, 384, 0, "Raycaster");

    while(!done())
    {
        for(int x=0; x<w; x++)
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

            /* what direction to step in x or y direction */
            int stepX;
            int stepY;

            int hit = 0;
            int side;  

            /* calculate step and sideDist */
            if(rayDirX<0)
            {
                stepX = -1;
                sideDistX = (rayPosX - mapX) * deltaDistX;
            }
            else
            {
                stepX = 1;
                sideDistX = (mapX + 1.0 - rayPosX) * deltaDistX;
            }

            if(rayDirY < 0)
            {
                stepY = -1;
                sideDistY = (rayPosY - mapY) * deltaDistY;
            }
            else
            {
                stepY = 1;
                sideDistY = (mapY + 1.0 - rayPosY) * deltaDistY;
            }

            /* digital differential analyzer algorithm */
            while(hit == 0)
            {
                if(sideDistX < sideDistY)
                {
                    sideDistX += deltaDistX;
                    mapX += stepX;
                    side = 0;
                }
                else
                {
                    sideDistY += deltaDistY;
                    mapY += stepY;
                    side = 1;
                }

                /* does ray intersect wall? */
                if(worldMap[mapX][mapY] > 0)
                {
                    hit = 1;
                }
            }

            /* distance projected on camera direction */
            if(side == 0)
            {
                perpWallDist = fabs((mapX - rayPosX + (1 - stepX) / 2) / rayDirX);
            }
            else
            {
                perpWallDist = fabs((mapY - rayPosY + (1 - stepY) / 2) / rayDirY);
            }

            /* calculate height of vertical line (gives illusion of distance) */
            int lineHeight = abs(int(h/perpWallDist));

            /* calculate start and end point of vertical line */
            int drawStart = -lineHeight/2 + h/2;

            if(drawStart < 0)
            {
                drawStart = 0;
            }

            int drawEnd = lineHeight/2 + h/2;

            if(drawEnd >= h)
            {
                drawEnd = h-1;
            }

            ColorRGB color;

            switch(worldMap[mapX][mapY])
            {
                case 1:
                       color = RGB_Red;
                       break;
                default:
                       color = RGB_Yellow;
                       break;
            }

            /* gives shadow to one side */
            if(side == 1)
            {
               color = color/2;
            }

            /* draw the vertical line */
            verLine(x, drawStart, drawEnd, color);
        }

        /* handles timing/FPS */
        oldTime = time;
        time = getTicks();
        double frameTime = (time - oldTime) / 1000.0; /* time for frame in seconds */
        print(1.0 / frameTime);  /* prints FPS counter  */
        redraw();
        cls();

        /* speed modifiers */
        double moveSpeed = frameTime*5.0; /* squares per second */
        double rotSpeed = frameTime *3.0; /* radians per second */
        readKeys();

        if(keyDown(SDLK_UP))
        {
            if(worldMap[int(posX + dirX * moveSpeed)][int(posY)] == false)
            {
                posX += dirX * moveSpeed;
            }

            if(worldMap[int(posX)][int(posY + dirY * moveSpeed)] == false)
            {
                posY += dirY * moveSpeed;
            }
        }
        if(keyDown(SDLK_DOWN))
        {
            if(worldMap[int(posX - dirX * moveSpeed)][int(posY)] == false)
            {
                posX -= dirX * moveSpeed;
            }

            if(worldMap[int(posX)][int(posY - dirY * moveSpeed)] == false)
            {
                posY -= dirY * moveSpeed;
            }
        }
        if(keyDown(SDLK_RIGHT))
        {
            double oldDirX = dirX;
            dirX = dirX*cos(-rotSpeed) - dirY*sin(-rotSpeed);
            dirY = oldDirX*sin(-rotSpeed) + dirY*cos(-rotSpeed);
            double oldPlaneX = planeX;
            planeX = planeX*cos(-rotSpeed) - planeY*sin(-rotSpeed);
            planeY = oldPlaneX*sin(-rotSpeed) + planeY*cos(-rotSpeed);
        }
        if(keyDown(SDLK_LEFT))
        {
            double oldDirX = dirX;
            dirX = dirX*cos(rotSpeed) - dirY*sin(rotSpeed);
            dirY = oldDirX*sin(rotSpeed) + dirY*cos(rotSpeed);
            double oldPlaneX = planeX;
            planeX = planeX*cos(rotSpeed) - planeY*sin(rotSpeed);
            planeY = oldPlaneX*sin(rotSpeed) + planeY*cos(rotSpeed);
        }
    }

    return 0;
}
