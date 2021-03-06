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
    {1,1,1,1,1,1,1,1,1,1,1,1,5,5,5,5,5,5,5,5,5,5,5,5,5},
    {1,0,0,0,0,0,0,0,0,0,0,1,5,0,0,0,0,0,0,0,0,0,0,0,5},
    {1,0,0,0,0,0,0,0,0,0,0,1,5,0,0,0,0,0,0,0,0,0,0,0,5},
    {1,1,1,1,1,1,1,1,1,0,1,1,5,5,5,5,0,5,5,5,5,5,5,5,5},
    {1,3,3,3,3,3,3,3,3,0,3,4,4,4,4,4,0,4,4,4,4,4,4,4,4},
    {3,0,0,0,0,0,0,0,0,0,0,0,0,0,0,4,0,4,0,0,0,0,0,0,4},
    {3,0,0,0,0,0,0,0,3,0,3,4,0,0,0,4,0,0,0,0,0,0,0,0,4},
    {3,0,9,9,9,9,0,0,3,0,4,4,0,0,0,4,0,4,0,0,0,0,0,0,4},
    {3,0,0,0,0,0,0,0,3,0,3,4,0,0,0,4,0,4,0,0,0,0,0,0,4},
    {3,0,0,0,0,0,0,0,3,0,4,4,0,0,0,4,0,4,4,4,4,4,4,4,4},
    {3,3,3,3,3,3,3,3,3,0,3,4,0,0,0,0,0,4,3,3,3,3,3,3,3},
    {3,0,0,0,0,0,0,0,3,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,3},
    {3,0,0,0,0,0,0,0,3,0,3,1,1,0,1,1,1,1,0,0,0,0,0,0,3},
    {3,0,0,0,0,0,0,0,0,0,4,1,0,0,0,0,0,1,0,0,0,0,0,0,3},
    {3,0,0,0,0,0,0,0,3,0,3,1,0,0,0,0,0,1,0,0,0,0,0,0,3},
    {3,0,0,0,0,0,0,0,3,0,4,1,0,0,0,0,0,1,0,0,0,0,0,0,3},
    {3,3,3,3,3,3,3,3,3,0,3,1,0,0,0,0,0,1,0,0,0,0,0,0,3},
    {4,3,4,3,4,3,4,3,4,0,4,1,0,0,0,0,0,1,0,0,0,0,0,0,3},
    {3,0,0,0,0,0,0,0,0,0,3,1,1,0,1,1,1,1,0,3,3,3,3,3,3},
    {4,0,0,0,0,0,0,0,0,0,4,2,5,0,2,5,5,2,0,5,2,5,5,2,5},
    {3,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,5},
    {4,0,0,0,0,0,0,0,0,0,4,2,0,0,0,0,0,0,0,0,0,0,0,0,5},
    {3,4,3,4,3,4,3,4,3,4,3,5,5,5,2,5,5,2,5,5,2,5,5,2,5},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
};

//Original test map
/*int worldMap[mapWidth][mapHeight]=
{
    {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
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
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
};
*/

int main()
{
    double xPos = 1.1;
    double yPos = 1.1;
    double xDir = -1;
    double yDir = 0;
    double xPlane = 0;
    double yPlane = .66;

    double time = 0;
    double oldTime = 0;

    screen(512, 384, 0, "Raycasting Demo");

    while(!done())
    {
        for(int x=0; x<w; x++)
        {
            /* ray position and direction */
            double cameraX = 2*x / double(w)-1;
            double rayPosX = xPos;
            double rayPosY = yPos;
            double rayDirX = xDir + xPlane * cameraX;
            double rayDirY = yDir + yPlane * cameraX;
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
                case 2:
                       color = RGB_Blue;
                       break;
                case 3:
                       color = RGB_Green;
                       break;
                case 4:
                       color = RGB_Purple;
                       break;
                case 5:
                       color = RGB_White;
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
        double rotationSpeed = frameTime *3.0; /* radians per second */
        readKeys();

        if(keyDown(SDLK_UP))
        {
            if(worldMap[int(xPos + xDir * moveSpeed)][int(yPos)] == false)
            {
                xPos += xDir * moveSpeed;
            }

            if(worldMap[int(xPos)][int(yPos + yDir * moveSpeed)] == false)
            {
                yPos += yDir * moveSpeed;
            }
        }
        if(keyDown(SDLK_DOWN))
        {
            if(worldMap[int(xPos - xDir * moveSpeed)][int(yPos)] == false)
            {
                xPos -= xDir * moveSpeed;
            }

            if(worldMap[int(xPos)][int(yPos - yDir * moveSpeed)] == false)
            {
                yPos -= yDir * moveSpeed;
            }
        }
        if(keyDown(SDLK_RIGHT))
        {
            double oldXDir = xDir;
            xDir = xDir*cos(-rotationSpeed) - yDir*sin(-rotationSpeed);
            yDir = oldXDir*sin(-rotationSpeed) + yDir*cos(-rotationSpeed);
            double oldXPlane = xPlane;
            xPlane = xPlane*cos(-rotationSpeed) - yPlane*sin(-rotationSpeed);
            yPlane = oldXPlane*sin(-rotationSpeed) + yPlane*cos(-rotationSpeed);
        }
        if(keyDown(SDLK_LEFT))
        {
            double oldXDir = xDir;
            xDir = xDir*cos(rotationSpeed) - yDir*sin(rotationSpeed);
            yDir = oldXDir*sin(rotationSpeed) + yDir*cos(rotationSpeed);
            double oldXPlane = xPlane;
            xPlane = xPlane*cos(rotationSpeed) - yPlane*sin(rotationSpeed);
            yPlane = oldXPlane*sin(rotationSpeed) + yPlane*cos(rotationSpeed);
        }
    }

    return 0;
}
