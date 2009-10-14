#include <SDL/SDL.h>
#include <SDL/SDL_mixer.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_ttf.h>
#include <string.h>
#include <stdlib.h>
void  sound();
void clean_up();
void LoadMusic();
//Screen attributes
int SCREEN_WIDTH = 640;
int SCREEN_HEIGHT = 480;
int SCREEN_BPP = 32;

//The surfaces
SDL_Surface *background = NULL;
SDL_Surface *m1 = NULL;
SDL_Surface *m2 = NULL;
SDL_Surface *m4 = NULL;
SDL_Surface *m3 = NULL;
SDL_Surface *screen = NULL;

//The event structure
SDL_Event event;

//The font
TTF_Font *font = NULL;

//The color of the font
SDL_Color textColor = { 255,40,0};

SDL_Surface *load_image( char *filename )
{
    //The image that's loaded
    SDL_Surface* loadedImage = NULL;

    //The optimized surface that will be used
    SDL_Surface* optimizedImage = NULL;

    //Load the image
    loadedImage = IMG_Load( filename );

    //If the image loaded
    if( loadedImage != NULL )
    {
        //Create an optimized surface
        optimizedImage = SDL_DisplayFormat( loadedImage );

        //Free the old surface
        SDL_FreeSurface( loadedImage );

        //If the surface was optimized
        if( optimizedImage != NULL )
        {
            //Color key surface
            SDL_SetColorKey( optimizedImage, SDL_SRCCOLORKEY, SDL_MapRGB( optimizedImage->format, 0, 0xFF, 0xFF ) );
        }
    }

    //Return the optimized surface
    return optimizedImage;
}

void apply_surface( int x, int y, SDL_Surface* source, SDL_Surface* destination)
{
    //Holds offsets
    SDL_Rect offset;

    //Get offsets
    offset.x = x;
    offset.y = y;

    //Blit
    SDL_BlitSurface( source, NULL, destination, &offset );
}

int init()
{
    //Initialize all SDL subsystems
    if( SDL_Init( SDL_INIT_EVERYTHING ) == -1 )
    {
        return 0;
    }

    //Set up the screen
    screen = SDL_SetVideoMode( SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP, SDL_SWSURFACE );

    //If there was an error in setting up the screen
    if( screen == NULL )
    {
        return 0;
    }

    //Initialize SDL_ttf
    if( TTF_Init() == -1 )
    {
        return 0;
    }

    //Set the window caption
    SDL_WM_SetCaption( "MARIO MENU", NULL );

    //If everything initialized fine
    return 1;
}

int load_files()
{
    //Load the background image
    background = load_image( "background.png" );

    //Open the font
    font = TTF_OpenFont( "lazy.ttf", 19);
    //TTF_SetFontStyle("lazy.ttf",0x01);

    //If there was a problem in loading the background
    if( background == NULL )
    {
        return 0;
    }

    //If there was an error in loading the font
    if( font == NULL )
    {
        return 0;
    }

    //If everything loaded fine
    return 1;
}

int main_menu( int argc, char* args[] )
{
    //Quit flag
    int quit = 0;
    sound();
    LoadMusic();
   // Mix_PlayMusic ( music_menu,-1 );
    //The timer starting time
    Uint32 start = 0;

    //The timer start/stop flag
    int running = 0;

    //Initialize
    if( init() == 0 )
    {
        return 1;
    }

    //Load the files
    if( load_files() == 0 )
    {
        return 1;
    }

    //Generate the message surface
    m1 = TTF_RenderText_Solid( font, "Main menu", textColor );
    m2 = TTF_RenderText_Solid( font, "Load game", textColor );
    m3 = TTF_RenderText_Solid( font, "Save game", textColor );
    m4 = TTF_RenderText_Solid( font, "Exit", textColor );

    //Start the timer
    start = SDL_GetTicks();
    apply_surface( 0, 0, background, screen );
    apply_surface( 500, 200, m1, screen );
    apply_surface( 500, 230, m2, screen );
    apply_surface( 500, 260, m3, screen );
    apply_surface( 520, 290, m4, screen );

    //While the user hasn't quit
    while( quit == 0 )
    {
        //While there's an event to handle
        while( SDL_PollEvent( &event ) )
        {
            //If a key was pressed
            if( event.type == SDL_KEYDOWN )
            {
                //If s was pressed
                if( event.key.keysym.sym == SDLK_s )
                {
                    //If the timer is running
                    if( running == 1 )
                    {
                        //Stop the timer
                        running = 0;
                        start = 0;
                    }
                    else
                    {
                        //Start the timer
                        running = 1;
                        start = SDL_GetTicks();
                    }
                }
            }

            //If the user has Xed out the window
            if( event.type == SDL_QUIT )
            {
                //Quit the program
                quit = 1;
            }
        }

        //Apply the background

        //Update the screen
        if( SDL_Flip( screen ) == -1 )
        {
            return 1;
        }
    }

    //Clean up
    clean_up();

    return 0;
}
