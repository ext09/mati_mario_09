#include <stdio.h>
#include <stdlib.h>
#include <SDL/SDL.h>


#define HEIGHT 10
#define WIDTH 13

void InitImages(void);
void DrawIMG(SDL_Surface *img, int x, int y);
void DrawIMG1(SDL_Surface *img, int x, int y, int w, int h, int sx, int sy);
void DrawBG(void);
void DrawScene(void);


int lev[HEIGHT][WIDTH];

SDL_Surface *sky;
SDL_Surface *earth;
SDL_Surface *wood;
SDL_Surface *screen;
SDL_Surface *back;
SDL_Surface *image1;
SDL_Surface *box;
SDL_Surface *screen;

int xpos=0, ypos=400, xstep=0, ystep=0;

    void level()
    {
    // int lev[HEIGHT][WIDTH];
    int  i,j;
    FILE *fp;
    if ((fp=fopen("lev.txt","r "))==NULL)
    {
        printf("Owubka co3danu9.\n");
        exit(666);
    }
    for (i=0;i<HEIGHT;i++)
    {
        for (j=0;j<WIDTH;j++)
        {
            fscanf(fp,"%1d",&lev[i][j]);
            //fprintf(fp, "%d",lev[i][j]);
        }
        fscanf(fp,"\n");
    }
    fclose(fp);
    //*******************************************output
    for (i=0;i<HEIGHT;i++)
    {
        for (j=0;j<WIDTH;j++)
        {
            printf("%2d",lev[i][j]);
        }
        putchar ('\n');
    }
    }
/* ------------------------------------------- */
int main(int argc, char *argv[])
{
    Uint8* keys;
    level();

    if ( SDL_Init(SDL_INIT_AUDIO|SDL_INIT_VIDEO) < 0 )
    {
        printf("Unable to init SDL: %s\n", SDL_GetError());
        exit(1);
    }

    atexit(SDL_Quit);

    SDL_WM_SetCaption("...","Mario");
    SDL_WM_SetIcon(SDL_LoadBMP("icon.bmp"), NULL);


    screen=SDL_SetVideoMode(640,480,32,SDL_HWSURFACE|SDL_DOUBLEBUF);
    if ( screen == NULL )
    {
        printf("Unable to set 640x480 video: %s\n", SDL_GetError());
        exit(1);
    }
    SDL_EnableKeyRepeat(1, SDL_DEFAULT_REPEAT_INTERVAL/6);
    InitImages();
    DrawBG();
    SDL_SetColorKey (image1, SDL_SRCCOLORKEY, SDL_MapRGB(image1->format,255,255,255));
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
    image1=SDL_LoadBMP("image.bmp");
    box=SDL_LoadBMP("box.bmp");
    sky=SDL_LoadBMP("sky.bmp");
    wood=SDL_LoadBMP("wood.bmp");
    earth=SDL_LoadBMP("earth.bmp");

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

    int i,j,z2=0,y2=0;
    for (i=0;i<HEIGHT;i++)
    {
        z2=0;
        for (j=0;j<WIDTH;j++)
        {
            if (lev[i][j] == 0)
            {
                DrawIMG1(sky, z2, y2, 50, 50,0,0);
                z2=z2+50;

            }
            if (lev[i][j] == 1)
            {
                DrawIMG1(earth, z2, y2, 50, 50,0,0);
                z2=z2+50;
            }
            if (lev[i][j] == 2)
            {
                DrawIMG1(wood, z2, y2, 50, 50, 0,0);
                z2=z2+50;
            }
        }
        y2=y2+50;
    }

    DrawIMG1(sky, xpos, ypos, 52, 52, xpos, ypos);
    xpos += xstep;
    ypos += ystep;
    DrawIMG(image1, xpos, ypos);
    xstep = ystep = 0;
//   DrawIMG1(box, 200, 340, 152, 152, 0, 0);
    SDL_Flip(screen);
}
