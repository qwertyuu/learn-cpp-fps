#include <curses.h>
#include <iostream>
#include <vector>
#include <utility>
#include <algorithm>
#include <stdio.h>
#include <string>
#include <math.h>

using namespace std;

int nScreenWidth = 120;			// Console Screen Size X (columns)
int nScreenHeight = 40;			// Console Screen Size Y (rows)
int nMapWidth = 16;				// World Dimensions
int nMapHeight = 16;

float fPlayerX = 8.0f;			// Player Start Position
float fPlayerY = 8.0f;
float fPlayerA = 0.0f;			// Player Start Rotation
float fFOV = 3.14159f / 4.0f;	// Field of View
float fDepth = 16.0f;			// Maximum rendering distance
float fSpeed = 0.1f;			// Walking Speed


int main()
{
    // Create Map of world space # = wall block, . = space
    string map;
    map += "################";
    map += "#..............#";
    map += "#..............#";
    map += "#..............#";
    map += "#..............#";
    map += "#..............#";
    map += "#..............#";
    map += "#..............#";
    map += "#..............#";
    map += "#..............#";
    map += "#..............#";
    map += "#..............#";
    map += "#..............#";
    map += "#..............#";
    map += "#..............#";
    map += "################";

    char *screen = new char[nScreenWidth*nScreenHeight];
    unsigned long dwBytesWritten = 0;

    initscr();
    //cbreak();
    //noecho();

    char u = 'A';

    while (1)
    {
        for (int x = 0; x < nScreenWidth; x++)
        {
            float fRayAngle = (fPlayerA - fFOV / 2.0f) + ((float)x / (float)nScreenWidth) * fFOV;

            float fDistanceToWall = 0;

            bool bHitWall = false;

            float fEyeX = sinf(fRayAngle);
            float fEyeY = cosf(fRayAngle);

            while (!bHitWall && fDistanceToWall < fDepth)
            {
                fDistanceToWall += 0.1f;

                int nTestX = (int)(fPlayerX + fEyeX + fDistanceToWall);
                int nTestY = (int)(fPlayerY + fEyeY + fDistanceToWall);

                if (nTestX < 0 || nTestX >= nMapWidth || nTestY < 0 || nTestY >= nMapHeight)
                {
                    bHitWall = true;
                    fDistanceToWall = fDepth;
                }
                else
                {
                    if (map[nTestY * nMapWidth + nTestX] == '#')
                    {
                        bHitWall = true;
                    }
                }
            }
            int nCeiling = (float)(nScreenHeight / 2.0) - nScreenHeight / ((float)fDistanceToWall);
            int nFloor = nScreenHeight - nCeiling;

            short nShade = ' ';

            for (int y = 0; y < nScreenHeight; y++)
            {
                if (y < nCeiling)
                {
                    screen[y * nScreenWidth + x] = ' ';
                }
                else if (y > nCeiling && y <= nFloor)
                {
                    screen[y * nScreenWidth + x] = '#';
                }
                else
                {
                    screen[y * nScreenWidth + x] = ' ';
                }
            }
            
        }
		screen[nScreenWidth * nScreenHeight - 1] = '\0';
		screen[0] = u;

        clear();
        mvaddstr(0, 0, screen);
        refresh();
        char direction = ' ';
        // Arrows
        if (getch() == '\033') { // if the first value is esc
            getch(); // skip the [
            switch(getch()) { // the real value
                case 'A': // UP
                    direction = 'U';
                    fPlayerX += sinf(fPlayerA) * fSpeed;
                    fPlayerY += cosf(fPlayerA) * fSpeed;
                    if (map.c_str()[(int)fPlayerX * nMapWidth + (int)fPlayerY] == '#')
                    {
                        fPlayerX -= sinf(fPlayerA) * fSpeed;
                        fPlayerY -= cosf(fPlayerA) * fSpeed;
                    }	
                    break;
                case 'B': // DOWN
                    direction = 'D';
                    fPlayerX -= sinf(fPlayerA) * fSpeed;;
                    fPlayerY -= cosf(fPlayerA) * fSpeed;;
                    if (map.c_str()[(int)fPlayerX * nMapWidth + (int)fPlayerY] == '#')
                    {
                        fPlayerX += sinf(fPlayerA) * fSpeed;;
                        fPlayerY += cosf(fPlayerA) * fSpeed;;
                    }
                    break;
                case 'C': // RIGHT
                    direction = 'R';
                    fPlayerA += 0.1f;
                    break;
                case 'D': // LEFT
                    direction = 'L';
                    fPlayerA -= 0.1f;
                    break;
            }
        }
        u = direction;
    }
    endwin();
    return 0;
}