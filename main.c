#include <stdio.h>
#include <stdlib.h>
#include <SDL/SDL.h>
#include <SDL/SDL_mixer.h>

#define HEIGHT 10
#define WIDTH 13

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


int lev[HEIGHT][WIDTH];
// image
SDL_Surface *sky;
SDL_Surface *earth;
SDL_Surface *wood;
SDL_Surface *screen;
SDL_Surface *image1;
// sound
Mix_Music *music = NULL;//The sound effects that will be used
Mix_Chunk *run = NULL;

/* ------------------------------------------- */
int xpos=0, ypos=400, xstep=0, ystep=0;
/* ------------------------------------------- */
/* ----------------MAIN----------------------- */
/* ------------------------------------------- */
int main(int argc, char *argv[])
{
    printf ("\t\nREADY\n");
//   Uint8* keys;
    sound();
    level();
    LoadMusic();
    Mix_VolumeMusic(90);
    Mix_VolumeChunk(run,45);
    if ( SDL_Init(SDL_INIT_AUDIO|SDL_INIT_VIDEO) < 0 )
    {
        printf("Unable to init SDL: %s\n", SDL_GetError());
        exit(1);
    }
    atexit(SDL_Quit);

    SDL_WM_SetCaption("...","Mario");
    SDL_WM_SetIcon(SDL_LoadBMP("icon.bmp"), NULL);
    screen=SDL_SetVideoMode(640,480,16,SDL_HWSURFACE|SDL_DOUBLEBUF|SDL_FULLSCREEN);
    if ( screen == NULL )
    {
        printf("Unable to set 640x480 video: %s\n", SDL_GetError());
        exit(1);
    }
    SDL_EnableKeyRepeat(1, SDL_DEFAULT_REPEAT_INTERVAL/7);
    InitImages();
    DrawBG();
    SDL_SetColorKey (image1, SDL_SRCCOLORKEY, SDL_MapRGB(image1->format,255,255,255));
    DrawScene();
    int done=0;
    Mix_PlayMusic( music,-1 );
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

                //   Mix_ResumeMusic();

                if ( event.key.keysym.sym == SDLK_ESCAPE )
                {
                    done = 1;
                }
                // keys = SDL_GetKeyState(NULL);

                move();

                //Mix_PauseMusic();
                DrawScene();
            }
        }
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
    DrawIMG(image1, xpos, ypos);
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




//new//////////////////////////////////////

void move()
{
    int i,j;
    Uint8* keys;
    i=ypos/50;
    j=xpos/50;
    keys = SDL_GetKeyState(NULL);
    //1
    if (((lev[i-1][j-1]==0)||(lev[i-1][j-1]==1)||(lev[i-1][j-1]==2))
            &&((lev[i-1][j]==1)||(lev[i-1][j]==2))
            &&((lev[i-1][j+1]==0)||(lev[i-1][j+1]==1)||(lev[i-1][j+1]==2))
            &&((lev[i+1][j-1]==0)||(lev[i+1][j-1]==1)||(lev[i+1][j-1]==2))
            &&((lev[i+1][j]==1)||(lev[i+1][j]==2))
            &&((lev[i+1][j+1]==0)||(lev[i+1][j+1]==1)||(lev[i+1][j+1]==2))
            &&(lev[i][j-1]==0)
            &&(lev[i][j+1]==0))
    {
        if (keys[SDLK_DOWN])
        {
            ystep = 0;
        }
        if (keys[SDLK_LEFT])
        {
            xstep = -2;
            Mix_PlayChannel(2,run,0);
        }

        if (keys[SDLK_RIGHT])
        {
            xstep = 2;
            Mix_PlayChannel(2,run,0);
        }
        if (keys[SDLK_SPACE])
        {
            ystep = 0;

        }

    }
//2
    if (((lev[i-1][j-1]==0)||(lev[i-1][j-1]==1)||(lev[i-1][j-1]==2))
            &&((lev[i][j-1]==1)||(lev[i][j-1]==2))
            &&((lev[i+1][j-1]==0)||(lev[i+1][j-1]==1)||(lev[i+1][j-1]==2))
            &&((lev[i-1][j+1]==0)||(lev[i-1][j+1]==1)||(lev[i-1][j+1]==2))
            &&((lev[i][j+1]==1)||(lev[i][j+1]==2))
            &&((lev[i+1][j+1]==0)||(lev[i+1][j+1]==1)||(lev[i+1][j+1]==2))
            &&(lev[i-1][j]==0)
            &&(lev[i+1][j]==0))
    {
        if (keys[SDLK_DOWN])
        {
            ystep = 2;
        }
        if (keys[SDLK_LEFT])
        {
            xstep = 0;
            Mix_PlayChannel(2,run,0);
        }

        if (keys[SDLK_RIGHT])
        {
            xstep = 0;
            Mix_PlayChannel(2,run,0);
        }
        if (keys[SDLK_SPACE])
        {
            ystep = -5;

        }
    }
//3
    if (((lev[i-1][j-1]==0)||(lev[i-1][j-1]==1)||(lev[i-1][j-1]==2))
            //
            &&((lev[i-1][j]==1)||(lev[i-1][j]==2))
            //
            &&((lev[i-1][j+1]==0)||(lev[i-1][j+1]==1)||(lev[i-1][j+1]==2))
            &&((lev[i+1][j-1]==0)||(lev[i+1][j-1]==1)||(lev[i+1][j-1]==2))
            &&((lev[i+1][j+1]==0)||(lev[i+1][j+1]==1)||(lev[i+1][j+1]==2))
            &&(lev[i][j-1]==0)
            &&(lev[i+1][j]==0)
            &&(lev[i][j+1]==0))
    {
        if (keys[SDLK_DOWN])
        {
            ystep = 2;
        }
        if (keys[SDLK_LEFT])
        {
            xstep = -2;
            Mix_PlayChannel(2,run,0);
        }

        if (keys[SDLK_RIGHT])
        {
            xstep = 2;
            Mix_PlayChannel(2,run,0);
        }
        if (keys[SDLK_SPACE])
        {
            ystep = 0;

        }
    }
//4
    if (((lev[i-1][j+1]==0)||(lev[i-1][j+1]==1)||(lev[i-1][j+1]==2))
    &&((lev[i][j+1]==1)||(lev[i][j+1]==2))
    &&((lev[i+1][j+1]==0)||(lev[i+1][j+1]==1)||(lev[i+1][j+1]==2))
    &&((lev[i-1][j-1]==0)||(lev[i-1][j-1]==1)||(lev[i-1][j-1]==2))
    &&((lev[i+1][j-1]==0)||(lev[i+1][j-1]==1)||(lev[i+1][j-1]==2))
    &&(lev[i-1][j]==0)
    &&(lev[i][j-1]==0)
    &&(lev[i+1][j]==0))
    {
        if (keys[SDLK_DOWN])
        {
            ystep = 2;
        }
        if (keys[SDLK_LEFT])
        {
            xstep = -2;
            Mix_PlayChannel(2,run,0);
        }

        if (keys[SDLK_RIGHT])
        {
            xstep = 0;
            Mix_PlayChannel(2,run,0);
        }
        if (keys[SDLK_SPACE])
        {
            ystep = -5;

        }
    }
//5
    if (((lev[i-1][j-1]==0)||(lev[i-1][j-1]==1)||(lev[i-1][j-1]==2))
            &&((lev[i-1][j+1]==0)||(lev[i-1][j+1]==1)||(lev[i-1][j+1]==2))
            &&((lev[i+1][j-1]==0)||(lev[i+1][j-1]==1)||(lev[i+1][j-1]==2))
            //
            &&((lev[i+1][j]==1)||(lev[i+1][j]==2))
            //
            &&((lev[i+1][j+1]==0)||(lev[i+1][j+1]==1)||(lev[i+1][j+1]==2))
            &&(lev[i][j-1]==0)
            &&(lev[i-1][j]==0)
            &&(lev[i][j+1]==0))
    {
        if (keys[SDLK_DOWN])
        {
            ystep = 0;
        }
        if (keys[SDLK_LEFT])
        {
            xstep = -2;
            Mix_PlayChannel(2,run,0);
        }

        if (keys[SDLK_RIGHT])
        {
            xstep = 2;
            Mix_PlayChannel(2,run,0);
        }
        if (keys[SDLK_SPACE])
        {
            ystep = -5;

        }
    }
//6
    if (((lev[i-1][j-1]==0)||(lev[i-1][j-1]==1)||(lev[i-1][j-1]==2))
            //
            &&((lev[i][j-1]==1)||(lev[i][j-1]==2))
            //
            &&((lev[i+1][j-1]==0)||(lev[i+1][j-1]==1)||(lev[i+1][j-1]==2))
            &&((lev[i-1][j+1]==0)||(lev[i-1][j+1]==1)||(lev[i-1][j+1]==2))
            &&((lev[i+1][j+1]==0)||(lev[i+1][j+1]==1)||(lev[i+1][j+1]==2))
            &&(lev[i-1][j]==0)
            &&(lev[i][j+1]==0)
            &&(lev[i+1][j]==0))
    {
        if (keys[SDLK_DOWN])
        {
            ystep = 2;
        }
        if (keys[SDLK_LEFT])
        {
            xstep = 0;
            Mix_PlayChannel(2,run,0);
        }

        if (keys[SDLK_RIGHT])
        {
            xstep = 2;
            Mix_PlayChannel(2,run,0);
        }
        if (keys[SDLK_SPACE])
        {
            ystep = -5;

        }
    }
//7
    if ((lev[i][j-1]==0)
            &&(lev[i-1][j]==0)
            &&(lev[i][j+1]==0)
            &&(lev[i+1][j]==0))
           // &&((lev[i-1][j-1]==0)||(lev[i-1][j-1]==1)||(lev[i-1][j-1]==2))
            //&&((lev[i-1][j+1]==0)|(lev[i-1][j+1]==1)||(lev[i-1][j+1]==2))
            //&&((lev[i+1][j+1]==0)||(lev[i+1][j+1]==1)||(lev[i+1][j+1]==2))
            //&&((lev[i+1][j-1]==0)||(lev[i+1][j-1]==1)||(lev[i+1][j-1]==2)))
    {
        if (keys[SDLK_DOWN])
        {
            ystep = 2;
        }
        if (keys[SDLK_LEFT])
        {
            xstep = -2;
            Mix_PlayChannel(2,run,0);
        }

        if (keys[SDLK_RIGHT])
        {
            xstep = 2;
            Mix_PlayChannel(2,run,0);
        }
        if (keys[SDLK_SPACE])
        {
            ystep = -5;

        }
    }
//8
    if (((lev[i-1][j-1]==0)||(lev[i-1][j-1]==1)||(lev[i-1][j-1]==2))
            &&((lev[i-1][j]==1)||(lev[i-1][j]==2))
            &&((lev[i-1][j+1]==0)||(lev[i-1][j+1]==1)||(lev[i-1][j+1]==2))
            &&((lev[i+1][j-1]==0)||(lev[i+1][j-1]==1)||(lev[i+1][j-1]==2))
            &&((lev[i+1][j]==1)||(lev[i+1][j]==2))
            &&((lev[i+1][j+1]==0)||(lev[i+1][j+1]==1)||(lev[i+1][j+1]==2))
            &&((lev[i][j-1]==1)||(lev[i][j-1]==2))
            &&(lev[i][j+1]==0))
    {
        if (keys[SDLK_DOWN])
        {
            ystep = 0;
        }
        if (keys[SDLK_LEFT])
        {
            xstep = 0;
            Mix_PlayChannel(2,run,0);
        }

        if (keys[SDLK_RIGHT])
        {
            xstep = 2;
            Mix_PlayChannel(2,run,0);
        }
        if (keys[SDLK_SPACE])
        {
            ystep = 0;

        }
    }
//9
    if (((lev[i-1][j-1]==0)||(lev[i-1][j-1]==1)||(lev[i-1][j-1]==2))
            &&((lev[i][j-1]==1)||(lev[i][j-1]==2))
            &&((lev[i+1][j-1]==0)||(lev[i+1][j-1]==1)||(lev[i+1][j-1]==2))
            &&((lev[i-1][j+1]==0)||(lev[i-1][j+1]==1)||(lev[i-1][j+1]==2))
            &&((lev[i][j+1]==1)||(lev[i][j+1]==2))
            &&((lev[i+1][j+1]==0)||(lev[i+1][j+1]==1)||(lev[i+1][j+1]==2))
            &&(lev[i-1][j]==0)
            &&((lev[i+1][j]==1)||(lev[i+1][j]==2)))
    {
        if (keys[SDLK_DOWN])
        {
            ystep = 0;
        }
        if (keys[SDLK_LEFT])
        {
            xstep = 0;
            Mix_PlayChannel(2,run,0);
        }

        if (keys[SDLK_RIGHT])
        {
            xstep = 0;
            Mix_PlayChannel(2,run,0);
        }
        if (keys[SDLK_SPACE])
        {
            ystep = -5;

        }
    }
    //10
    if (((lev[i-1][j-1]==0)||(lev[i-1][j-1]==1)||(lev[i-1][j-1]==2))
            &&((lev[i-1][j]==1)||(lev[i-1][j]==2))
            &&((lev[i-1][j+1]==0)||(lev[i-1][j+1]==1)||(lev[i-1][j+1]==2))
            &&((lev[i+1][j-1]==0)||(lev[i+1][j-1]==1)||(lev[i+1][j-1]==2))
            &&((lev[i+1][j]==1)||(lev[i+1][j]==2))
            &&((lev[i+1][j+1]==0)||(lev[i+1][j+1]==1)||(lev[i+1][j+1]==2))
            &&(lev[i][j-1]==0)
            &&((lev[i][j+1]==1)||(lev[i][j+1]==2)))
    {
        if (keys[SDLK_DOWN])
        {
            ystep = 0;
        }
        if (keys[SDLK_LEFT])
        {
            xstep = -2;
            Mix_PlayChannel(2,run,0);
        }

        if (keys[SDLK_RIGHT])
        {
            xstep = 0;
            Mix_PlayChannel(2,run,0);
        }
        if (keys[SDLK_SPACE])
        {
            ystep = 0;

        }
    }
//11
    if (((lev[i-1][j-1]==0)||(lev[i-1][j-1]==1)||(lev[i-1][j-1]==2))
            &&((lev[i][j-1]==1)||(lev[i][j-1]==2))
            &&((lev[i+1][j-1]==0)||(lev[i+1][j-1]==1)||(lev[i+1][j-1]==2))
            &&((lev[i-1][j+1]==0)||(lev[i-1][j+1]==1)||(lev[i-1][j+1]==2))
            &&((lev[i][j+1]==1)||(lev[i][j+1]==2))
            &&((lev[i+1][j+1]==0)||(lev[i+1][j+1]==1)||(lev[i+1][j+1]==2))
            &&((lev[i-1][j]==1)||(lev[i-1][j]==2))
            &&(lev[i+1][j]==0))
    {
        if (keys[SDLK_DOWN])
        {
            ystep = 2;
        }
        if (keys[SDLK_LEFT])
        {
            xstep = 0;
            Mix_PlayChannel(2,run,0);
        }

        if (keys[SDLK_RIGHT])
        {
            xstep = 0;
            Mix_PlayChannel(2,run,0);
        }
        if (keys[SDLK_SPACE])
        {
            ystep = 0;

        }
    }
//12
    if (((lev[i-1][j-1]==0)||(lev[i-1][j-1]==1)||(lev[i-1][j-1]==2))
            &&((lev[i][j-1]==1)||(lev[i][j-1]==2))
            &&((lev[i+1][j-1]==0)||(lev[i+1][j-1]==1)||(lev[i+1][j-1]==2))
            &&((lev[i+1][j]==1)||(lev[i+1][j]==2))
            &&((lev[i+1][j+1]==0)||(lev[i+1][j+1]==1)||(lev[i+1][j+1]==2))
            &&(lev[i][j+1]==0)
            &&((lev[i-1][j+1]==0)||(lev[i-1][j+1]==1)||(lev[i-1][j+1]==2))
            &&(lev[i-1][j]==0))
    {
        if (keys[SDLK_DOWN])
        {
            ystep = 0;
        }
        if (keys[SDLK_LEFT])
        {
            xstep = 0;
            Mix_PlayChannel(2,run,0);
        }

        if (keys[SDLK_RIGHT])
        {
            xstep = 2;
            Mix_PlayChannel(2,run,0);
        }
        if (keys[SDLK_SPACE])
        {
            ystep = -5;

        }
    }
//13
    if (((lev[i-1][j-1]==0)||(lev[i-1][j-1]==1)||(lev[i-1][j-1]==2))
            &&(lev[i][j-1]==0)
            &&((lev[i+1][j-1]==0)||(lev[i+1][j-1]==1)||(lev[i+1][j-1]==2))
            &&((lev[i+1][j]==1)||(lev[i+1][j]==2))
            &&((lev[i+1][j+1]==0)||(lev[i+1][j+1]==1)||(lev[i+1][j+1]==2))
            &&((lev[i][j+1]==1)||(lev[i][j+1]==2))
            &&((lev[i-1][j+1]==0)||(lev[i-1][j+1]==1)||(lev[i-1][j+1]==2))
            &&(lev[i-1][j]==0))
    {
        if (keys[SDLK_DOWN])
        {
            ystep = 0;
        }
        if (keys[SDLK_LEFT])
        {
            xstep = -2;
            Mix_PlayChannel(2,run,0);
        }

        if (keys[SDLK_RIGHT])
        {
            xstep = 0;
            Mix_PlayChannel(2,run,0);
        }
        if (keys[SDLK_SPACE])
        {
            ystep = -5;

        }
    }
//14
    if (((lev[i-1][j-1]==0)||(lev[i-1][j-1]==1)||(lev[i-1][j-1]==2))
            &&((lev[i][j-1]==1)||(lev[i][j-1]==2))
            &&((lev[i+1][j-1]==0)||(lev[i+1][j-1]==1)||(lev[i+1][j-1]==2))
            &&(lev[i+1][j]==0)
            &&((lev[i+1][j+1]==0)||(lev[i+1][j+1]==1)||(lev[i+1][j+1]==2))
            &&(lev[i][j+1]==0)
            &&((lev[i-1][j+1]==0)||(lev[i-1][j+1]==1)||(lev[i-1][j+1]==2))
            &&((lev[i-1][j]==1)||(lev[i-1][j]==2)))
    {
        if (keys[SDLK_DOWN])
        {
            ystep = 2;
        }
        if (keys[SDLK_LEFT])
        {
            xstep = 0;
            Mix_PlayChannel(2,run,0);
        }

        if (keys[SDLK_RIGHT])
        {
            xstep = 2;
            Mix_PlayChannel(2,run,0);
        }
        if (keys[SDLK_SPACE])
        {
            ystep = 0;

        }
    }
    //15
    if (((lev[i-1][j-1]==0)||(lev[i-1][j-1]==1)||(lev[i-1][j-1]==2))
            &&(lev[i][j-1]==0)
            &&((lev[i+1][j-1]==0)||(lev[i+1][j-1]==1)||(lev[i+1][j-1]==2))
            &&((lev[i-1][j+1]==0)||(lev[i-1][j+1]==1)||(lev[i-1][j+1]==2))
            &&((lev[i][j+1]==1)||(lev[i][j+1]==2))
            &&((lev[i+1][j+1]==0)||(lev[i+1][j+1]==1)||(lev[i+1][j+1]==2))
            &&((lev[i-1][j]==1)||(lev[i-1][j]==2))
            &&(lev[i+1][j]==0))
    {
        if (keys[SDLK_DOWN])
        {
            ystep = 2;
        }
        if (keys[SDLK_LEFT])
        {
            xstep = -2;
            Mix_PlayChannel(2,run,0);
        }

        if (keys[SDLK_RIGHT])
        {
            xstep = 0;
            Mix_PlayChannel(2,run,0);
        }
        if (keys[SDLK_SPACE])
        {
            ystep = 0;

        }

    }
}







