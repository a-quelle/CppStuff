#pragma once
#define SDL_MAIN_HANDLED
#include "SDL.h"
#include "..\NeuralNetwork\NeuralNet.h"

class Plotter {
    bool draw = true;
    SDL_Window* gWindow = nullptr;
    SDL_Surface* gScreenSurface = nullptr;
    SDL_Surface* image = nullptr;
    SDL_Event e;
    Uint32* imageData;
    double fDist = 90;

    bool initSDL();
    Plotter();
    bool init();
    void close();

public:
    static Plotter& getInstance();
    void drawPlot(NeuralNet& neuralNet);
    Plotter(Plotter&) = delete;
    void operator = (const Plotter&) = delete;
    void handleEvents();
    ~Plotter();
    bool shouldDraw();
};