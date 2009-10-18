#include <SDL/SDL.h>
#include <SDL/SDL_mixer.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_ttf.h>
#include <string.h>
#include <stdlib.h>

void  sound();
void clean_up();
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
    SDL_WM_SetIcon(SDL_LoadBMP("icon.bmp"), NULL);
    //If everything initialized fine
    return 1;
}

int load_files()
{
    //Load the background image
    background = load_image( "background.png" );
    //Open the font
    font = TTF_OpenFont( "menu.ttf", 19);
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
    return 1;
    }

int main_menu( int argc, char* args[] )
{

    int quit = 0;
    sound();
    Mix_Music *music_menu = NULL;
    music_menu = Mix_LoadMUS( "music_menu.mp3" );
    Mix_PlayMusic ( music_menu,-1 );
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

    m1 = TTF_RenderText_Solid( font, "M a i n  m e n u", textColor );
    m2 = TTF_RenderText_Solid( font, "L o a d   g a m e", textColor );
    m3 = TTF_RenderText_Solid( font, "S a v e   g a m e", textColor );
    m4 = TTF_RenderText_Solid( font, "E x i t", textColor );

    apply_surface( 0, 0, background, screen );
    apply_surface( 453, 241, m1, screen );
    apply_surface( 450, 271, m2, screen );
    apply_surface( 450, 301, m3, screen );
    apply_surface( 500, 331, m4, screen );

    //While the user hasn't quit
    while( quit == 0 )
    {
        //While there's an event to handle
        while( SDL_PollEvent( &event ) )
        {
            //If the user has Xed out the window
            if( event.type == SDL_QUIT || event.key.keysym.sym == SDLK_ESCAPE )
            {
            Mix_PauseMusic();
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
