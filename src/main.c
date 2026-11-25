#include <SDL.h>
#include <stdio.h>
#include <stdbool.h>

#define mapWidth 24
#define mapHeight 24
#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480

// Example map
int worldMap[mapHeight][mapWidth] = {
  {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,2,2,2,2,2,0,0,0,0,3,0,3,0,3,0,0,0,1},
  {1,0,0,0,0,0,2,0,0,0,2,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,2,0,0,0,2,0,0,0,0,3,0,0,0,3,0,0,0,1},
  {1,0,0,0,0,0,2,0,0,0,2,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,2,2,0,2,2,0,0,0,0,3,0,3,0,3,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,4,4,4,4,4,4,4,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,4,0,4,0,0,0,0,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,4,0,0,0,0,0,0,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,4,0,4,0,0,0,0,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,4,0,4,4,4,4,4,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,4,4,4,4,4,4,4,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}
};

// Starts up SDL and creates window
bool init() 
{
    
}
// Loads media
bool loadMedia() 
{

}
// Frees media and shuts down SDL
bool quit() 
{

}

int main(int argc, char** args) {
    // Player poistion
    double posX = 22;
    double posY = 12;

    // Direction vector
    double dirX = -1, dirY = 0;

    // Plane vector 
    // (We use FOV that equals to 66 degree, FOV = 2 * arctan(|plane|/|dir|))
    double planeX = 0, planeY = 0.66;

    double oldTime = 0; // time of previous frame
    double time = 0; // time of current frame


    SDL_Window* window = NULL;

    SDL_Surface* screenSurface = NULL;

    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        printf("SDL couldn't initialize! SDL_Error: %s\n", SDL_GetError());
    } 
    else 
    {
        window = SDL_CreateWindow("RaycastEngine", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
        if (window == NULL) {
            printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
        }

        else 
        {
            screenSurface = SDL_GetWindowSurface(window);

            // NULL - to fill entire surface
            SDL_FillRect(screenSurface, NULL, SDL_MapRGB(screenSurface->format, 0xFF, 0xFF, 0xFF ));

            SDL_UpdateWindowSurface(window);

            SDL_Event event;
            bool quit = false;

            while (!quit)
            {
                while (SDL_PollEvent(&event))
                {
                    if (event.type == SDL_QUIT)
                    {
                        quit = true;
                    }
                }
                
                // Calculating ray position and direction for each x-stripe on computer screen
                for (int x = 0; x < SCREEN_WIDTH; ++x) {
                    double cameraX = 2 * x / (double)SCREEN_WIDTH - 1;
                    double rayDirX = dirX + planeX * cameraX;
                    double rayDirY = dirY + planeY * cameraX;  
                    
                    // Which box of the map we are in
                    int mapX = (int)posX;
                    int mapY = (int)posY;

                    // Distance from start position of the ray to the first x-side (y-side)
                    double sideDistX;
                    double sideDistY;

                    double deltaDistX = (rayDirX == 0) ? 1e30 : sqrt(1 + (rayDirY * rayDirY) / (rayDirX * rayDirX));
                    double deltaDistY = (rayDirY == 0) ? 1e30 : sqrt(1 + (rayDirX * rayDirX) / (rayDirY * rayDirY));

                    int stepX;
                    int stepY;

                    int hit = 0;
                    int side;

                    // Before DDA algorithm, we need to 



                }

            }

        }
    }

    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}