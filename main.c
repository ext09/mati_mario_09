#include <stdio.h>
#include <stdlib.h>
#include <SDL/SDL.h>

void InitImages(void);
void DrawIMG(SDL_Surface *img, int x, int y);
void DrawIMG1(SDL_Surface *img, int x, int y, int w, int h, int sx, int sy);
void DrawIMG2(SDL_Surface *img, int x, int y, int w, int h, int sx, int sy);
void DrawBG(void);
void DrawScene(void);

SDL_Surface *back;
SDL_Surface *image;
SDL_Surface *box;
SDL_Surface *screen;

int xpos=0, ypos=200, xstep=0, ystep=0;

/* ------------------------------------------- */
int main(int argc, char *argv[])
{
    Uint8* keys;

    if ( SDL_Init(SDL_INIT_AUDIO|SDL_INIT_VIDEO) < 0 )
    {
        printf("Unable to init SDL: %s\n", SDL_GetError());
        exit(1);
    }

    atexit(SDL_Quit);

    SDL_WM_SetCaption(" qw ","qw");
    SDL_WM_SetIcon(SDL_LoadBMP("icon.bmp"), NULL);

    screen=SDL_SetVideoMode(640,480,32,SDL_HWSURFACE|SDL_DOUBLEBUF);
    if ( screen == NULL )
    {
        printf("Unable to set 640x480 video: %s\n", SDL_GetError());
        exit(1);
    }


    SDL_EnableKeyRepeat(1, SDL_DEFAULT_REPEAT_INTERVAL/5);
    InitImages();
    SDL_SetColorKey(image, SDL_SRCCOLORKEY, SDL_MapRGB(image->format,255,255,255));
    DrawBG();
    DrawScene();
    int done=0;
    while (done == 0)
    {
        SDL_Event event;
        while ( SDL_PollEvent(&event) )
        {
            if ( event.type == SDL_QUIT )
            {
                done = 1;
            }
            if ( event.type == SDL_KEYDOWN )
            {
                if ( event.key.keysym.sym == SDLK_ESCAPE )
                {
                    done = 1;
                }
                keys = SDL_GetKeyState(NULL);

                if (keys[SDLK_UP])
                {
                    ystep = -1;
                }
                if (keys[SDLK_DOWN])
                {
                    ystep = 1;
                }
                if (keys[SDLK_LEFT])
                {
                    xstep = -1;
                }
                if (keys[SDLK_RIGHT])
                {
                    xstep = 1;
                }
                if (keys[SDLK_SPACE])
                {
                    ystep = -5;
                }
                DrawScene();
            }
        }
    }

    return 0;

}

/* ------------------------------------------- */
void InitImages()
{
    back=SDL_LoadBMP("bg.bmp");
    image=SDL_LoadBMP("image.bmp");
    box=SDL_LoadBMP("box.bmp");
}

/* ------------------------------------------- */
void DrawIMG(SDL_Surface *img, int x, int y)
{

    SDL_Rect dest;
    dest.x = x;
    dest.y = y;

    SDL_BlitSurface(img, NULL, screen, &dest);

}

/* ------------------------------------------- */
void DrawIMG1(SDL_Surface *img, int x, int y, int w, int h, int sx, int sy)
{

    SDL_Rect dest;
    dest.x = x;
    dest.y = y;

    SDL_Rect src;
    src.x = sx;
    src.y = sy;
    src.w = w;
    src.h = h;

    SDL_BlitSurface(img, &src, screen, &dest);

}

/* ------------------------------------------- */
void DrawBG()
{

    DrawIMG(back, 0, 0);

}

/* ------------------------------------------- */
void DrawScene()
{
    DrawIMG1(back, xpos, ypos, 135, 135, xpos, ypos);
    xpos += xstep;
    ypos += ystep;
    DrawIMG(image, xpos, ypos);
    xstep = ystep = 0;
    DrawIMG1(box, 200, 200, 152, 152, 0, 0);
    SDL_Flip(screen);
}
