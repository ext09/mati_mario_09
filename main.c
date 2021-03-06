#include <stdio.h>
#include <stdlib.h>
#include <SDL/SDL.h>
#include <SDL/SDL_mixer.h>
#define HEIGHT 16
#define WIDTH 20


/* ------------------------------------------- */
/* ----------------animation------------------ */
/* ------------------------------------------- */
struct player
{
    int xpos,ypos,vx,vy,col,xpos1,ypos1;
    SDL_Surface* link;
    int start, stop;
};
/* ------------------------------------------- */
/* ----------------function------------------- */
/* ------------------------------------------- */
void InitImages(void);
void DrawIMG(SDL_Surface *img, int x, int y);
void DrawIMG1(SDL_Surface *img, int x, int y, int w, int h, int sx, int sy);
void DrawBG(void);
void DrawScene(void);
void level(void);
void sound(void);
void LoadMusic(void);
void clean_up(void);
void move(void);
int main_menu(void);
//
void move_monstrik(void);
//

int lev[HEIGHT][WIDTH];

// image

SDL_Surface *sky;
SDL_Surface *earth;
SDL_Surface *wood;
SDL_Surface *screen;
SDL_Surface *image1;
//
SDL_Surface *monstrik;
//
// sound
Mix_Music *music = NULL;//The sound effects that will be used
Mix_Music *music_menu = NULL;
Mix_Chunk *run = NULL;

/* ------------------------------------------- */
int xpos=0, ypos=650, xstep=0, ystep=0, ypos1=650, xpos1=700, xstep1=0, ystep1=0;

/* ------------------------------------------- */
/* ----------------MAIN----------------------- */
/* ------------------------------------------- */

int main(int argc, char *argv[])
{
    main_menu();
    printf ("\t\nREADY\n");
    sound();
    level();
    LoadMusic();
    Mix_VolumeMusic(120);
    Mix_VolumeChunk(run,45);
    if ( SDL_Init(SDL_INIT_AUDIO|SDL_INIT_VIDEO) < 0 )
    {
        printf("Unable to init SDL: %s\n", SDL_GetError());
        exit(1);
    }
    atexit(SDL_Quit);

    SDL_WM_SetCaption("Mario","Mario");
    SDL_WM_SetIcon(SDL_LoadBMP("icon.bmp"), NULL);
    screen=SDL_SetVideoMode(1000,768,16,SDL_HWSURFACE|SDL_DOUBLEBUF);
    if ( screen == NULL )
    {
        printf("Unable to set 640x480 video: %s\n", SDL_GetError());
        exit(1);
    }
    SDL_EnableKeyRepeat(1, SDL_DEFAULT_REPEAT_INTERVAL/5);
    InitImages();
    DrawBG();
    SDL_SetColorKey (image1, SDL_SRCCOLORKEY, SDL_MapRGB(image1->format,255,255,255));

    int done=0;
    Mix_PlayMusic( music,-1 );

    while (done == 0)
    {
        SDL_ShowCursor(SDL_DISABLE);
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
                move();


            }

        }
       move_monstrik();

            DrawScene();
    }
    clean_up();
    printf ("FINISH");
    return 0;

}
/* ------------------------------------------- */
/* ----------------MUSIC---------------------- */
/* ------------------------------------------- */

void  sound()
{
    Mix_GetMusicType(music);
    if ( Mix_OpenAudio( 44100, MIX_DEFAULT_FORMAT, 2, 8192 ) == -1 )
    {
        printf("sound not load ");
    }
}

void LoadMusic()
{
    music = Mix_LoadMUS( "music.mp3" );
    if ( music == NULL )
    {
        printf(" music not found");
    }
    run = Mix_LoadWAV( "high.wav" );
    if ( run == NULL )
    {
        printf(" music not found");
    }
}
/* ------------------------------------------- */
/* ----------------IMAGE---------------------- */
/* ------------------------------------------- */

void InitImages()
{
    image1=SDL_LoadBMP("image.bmp");
    sky=SDL_LoadBMP("sky.bmp");
    wood=SDL_LoadBMP("wood.bmp");
    earth=SDL_LoadBMP("earth.bmp");
    monstrik=SDL_LoadBMP("monstrik.bmp");

}
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
    DrawIMG(sky, 0, 0);
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

    DrawIMG1(sky, xpos, ypos, 50, 50, xpos, ypos);
    xpos += xstep;
    ypos += ystep;
    xpos1 += xstep1;
    ypos1 += ystep1;
    DrawIMG(image1, xpos, ypos);
    DrawIMG(monstrik, xpos1, ypos1);
    xstep = ystep = 0;


    SDL_Flip(screen);

}
/* ------------------------------------------- */
/* -----------------level--------------------- */
/* ------------------------------------------- */
void level()
{
    int  i,j;
    FILE *fp;
    if ((fp=fopen("lev.txt","r"))==NULL)
    {
        printf("Owubka co3danu9.\n");
        exit(666);
    }
    for (i=0;i<HEIGHT;i++)
    {
        for (j=0;j<WIDTH;j++)
        {
            fscanf(fp,"%1d",&lev[i][j]);
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
/* ----------------clean up------------------- */
/* ------------------------------------------- */
void clean_up()
{
    //Free the surfaces
    SDL_FreeSurface( screen );
    //Free the sound effects
    Mix_CloseAudio();
    Mix_FreeChunk(run);
    //Quit SDL
    SDL_Quit();
}

//MOVE------------------------------------
void move()
{
    int i,j,i1,j1;
    Uint8* keys;
    i1=ypos%50;
    j1=xpos%50;
    keys = SDL_GetKeyState(NULL);

    if (keys[SDLK_DOWN])
    {
        i=(ypos+1)/50;
        j=xpos/50;
        if (lev[i+1][j]==0
                &&(!j1||lev[i+1][j+1]==0))
            ystep = 2;
    }
    if (keys[SDLK_LEFT])
    {
        i=ypos/50;
        j=(xpos-1)/50;
        if (lev[i][j]==0
                &&(!i1||lev[i+1][j]==0)
                &&xpos!=0)
            xstep = -2;
        else
            Mix_PlayChannel(2,run,0);
    }

    if (keys[SDLK_RIGHT])
    {
        i=ypos/50;
        j=(xpos+1)/50;
        if (lev[i][j+1]==0
                &&(!i1||lev[i+1][j+1]==0)
                &&xpos!=940)
            xstep = 2;
        else
            Mix_PlayChannel(2,run,0);
    }
    if (keys[SDLK_SPACE])
    {
        i=(ypos-5)/50;
        j=xpos/50;
        if (lev[i][j]==0
                &&(!j1||lev[i][j+1]==0)
                &&ypos!=0)
            ystep = -5;
    }

}
void move_monstrik()
{
    int i;

    if(xpos1==700)
    {
        for (i=0;i<200;i++)
        {
            xstep1=1;
        }
    }
    if(xpos1==900)
    {
        for (i=0;i<200;i++)
        {
            xstep1=-1;
        }
    }
}
