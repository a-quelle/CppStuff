#include "Plotter.h"
#include <fstream>
#include <iostream>
static const Uint32 RED = 0xFF0000; 
static const Uint32 GREEN =  0x00FF00; 
static const Uint32 BLUE =  0x0000FF; 
static const Uint32 ALPHA= 0xFF000000;
extern bool quit;

using namespace std;

Plotter& Plotter::getInstance()
{
    static Plotter plotter;
    return plotter;
}

Plotter::Plotter(){
    if( !initSDL() )
    {
        printf( "Failed to initialize!\n" );
        exit(EXIT_FAILURE);
    }
    image = SDL_CreateRGBSurface(0, 800, 800, 32, RED, GREEN, BLUE, ALPHA);
    imageData = (Uint32*) (image->pixels);
}

Plotter::~Plotter(){
    //Deallocate surface
    SDL_FreeSurface( image );
    image = NULL;

    //Destroy window
    SDL_DestroyWindow( gWindow );
    gWindow = NULL;
    gScreenSurface = NULL;
    //Quit SDL subsystems
    SDL_Quit();
}

void Plotter::handleEvents()
{
    while( SDL_PollEvent( &e ) != 0 )
    {
        //User requests quit
        if( e.type == SDL_QUIT )
        {
            quit = true;
            cout << "Quitting main loop." << endl;
        }
        if( e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_d )
        {
            fDist += 5;
            cout << "fDist is now: " << fDist << "\nRedrawing image." << endl;
            draw = true;
        }
        if( e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_a )
        {
            fDist -= 5;
            cout << "fDist is now: " << fDist << "\nRedrawing image." << endl;
            draw = true;
        }
        if( e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_s )
        {
            cout << "fDist is now: " << fDist << "\nRedrawing image." << endl;
            draw = true;
        }
    }
}

void Plotter::drawPlot(NeuralNet& neuralNet){
    vector<double> inputs(3);
    vector<double> outputs(3);
    neuralNet.lock.lock();
    for (int lDist = 0; lDist < 800; lDist++)
    {
        for (int rDist = 0; rDist < 800; rDist++)
        {
            inputs[0] = 0.01 * (180 - 0.2*lDist);
            inputs[1] = 0.01 * fDist;
            inputs[2] = 0.01 * (20 + 0.2*rDist);
            outputs = neuralNet.processInput(inputs);
            if (outputs[0] > outputs[1] && outputs[0] > outputs[2])
                imageData[800 * lDist + rDist] = RED + ALPHA;
            if (outputs[1] > outputs[0] && outputs[1] > outputs[2]) 
                imageData[800 * lDist + rDist] = GREEN + ALPHA;
            if (outputs[2] > outputs[1] && outputs[2] > outputs[1])
                imageData[800 * lDist + rDist] = BLUE + ALPHA;
            
        }
    }
    neuralNet.lock.unlock();
    SDL_BlitSurface( image, NULL, gScreenSurface, NULL );
    SDL_UpdateWindowSurface( gWindow );

    draw = false;
    cout << "Done drawing!" << endl;
}

bool Plotter::initSDL()
{
    //Initialization flag
    bool success = true;

    //Initialize SDL
    if( SDL_Init( SDL_INIT_VIDEO ) < 0 )
    {
        printf( "SDL could not initialize! SDL_Error: %s\n", SDL_GetError() );
        success = false;
    }
    else
    {
        //Create window
        gWindow = SDL_CreateWindow( "Network plotter", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 800, 800, SDL_WINDOW_SHOWN );
        if( gWindow == NULL )
        {
            printf( "Window could not be created! SDL_Error: %s\n", SDL_GetError() );
            success = false;
        }
        else
        {
            //Get window surface
            gScreenSurface = SDL_GetWindowSurface( gWindow );
        }
    }

    return success;
}

bool Plotter::shouldDraw()
{
    return draw;
}   
