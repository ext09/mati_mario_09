#include <stdlib.h> // For some useful functions such as atexit :)
#include "SDL.h" // main SDL header
#include "SDL_image.h" // image library, if your only using BMP's, get ride of this.
#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480
#define true 1
#define false 0 //You might have to declaire True and False.
#define COLORKEY 255, 0, 255 //Your Transparent colour

SDL_Surface *screen; //This pointer will reference the backbuffer

//I have set the flag SDL_SWSURFACE for a window :)
int InitVideo()
{
    // Load SDL
    Uint32  flags = SDL_DOUBLEBUF | SDL_SWSURFACE;
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        fprintf(stderr, "Unable to initialize SDL: %s\n", SDL_GetError());
        return false;
    }
    atexit(SDL_Quit); // Clean it up nicely :)

    // fullscreen can be toggled at run time :) any you might want to change the flags with params?
    //set the main screen to SCREEN_WIDTHxSCREEN_HEIGHT with a colour depth of 16:
    screen = SDL_SetVideoMode(SCREEN_WIDTH, SCREEN_HEIGHT, 16, flags);
    if (screen == NULL)
    {
        fprintf(stderr, "Unable to set video mode: %s\n", SDL_GetError());
        return false;
    }
    return true;
}

//--------------------------- Drawing Stuff -------------------------
SDL_Surface* LoadImage(char *file, int *exitstate)
{
    SDL_Surface *tmp;
    tmp = IMG_Load(file);
    *exitstate = false;

    if (tmp == NULL)
    {
        fprintf(stderr, "Error: '%s' could not be opened: %s\n", file, IMG_GetError());
    }
    else
    {
        if (SDL_SetColorKey(tmp, SDL_SRCCOLORKEY | SDL_RLEACCEL, SDL_MapRGB(tmp->format, COLORKEY)) == -1)
            fprintf(stderr, "Warning: colorkey will not be used, reason: %s\n", SDL_GetError());
        *exitstate = true;
    }
    return tmp;
}

void DrawImage(SDL_Surface *srcimg, int sx, int sy, int sw, int sh, SDL_Surface *dstimg, int dx, int dy, int alpha)
{
    alpha = 255;
    if ((srcimg == NULL) || (alpha == 0)) return; //If theres no image, or its 100% transparent.
    SDL_Rect src, dst;

    src.x = sx;
    src.y = sy;
    src.w = sw;
    src.h = sh;
    dst.x = dx;
    dst.y = dy;
    dst.w = src.w;
    dst.h = src.h;
    if (alpha != 255) SDL_SetAlpha(srcimg, SDL_SRCALPHA, alpha);
    SDL_BlitSurface(srcimg, &src, dstimg, &dst);
}
//------------------------ The Game -----------------------

int main()
{
    int res = 0; //Results
    if (InitVideo() == false) return 1;
    SDL_Surface *tux = LoadImage("tux.bmp",res);
    if (res == false) return 1;
    DrawImage(tux, 0,0, tux->w, tux->h, screen, 10, 10, 128);
    SDL_Flip(screen); //Refresh the screen
    SDL_Delay(2500); //Wait a bit :)

    //cleanup
    SDL_FreeSurface(tux);
    return 0;
}
