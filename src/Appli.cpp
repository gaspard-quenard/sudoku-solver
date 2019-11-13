#include "Appli.h"
#include <cstdlib>

using namespace std;

Appli::Appli()
{

    if (SDL_Init(SDL_INIT_EVERYTHING) == -1)
    {
        cout << "Error on initializing SDL" << endl;
        exit(EXIT_FAILURE);
    }
    window = SDL_CreateWindow("Sudoku Resolver",
                                SDL_WINDOWPOS_CENTERED,
                                SDL_WINDOWPOS_CENTERED,
                                800,
                                600,
                                SDL_WINDOW_SHOWN);
    if (window == nullptr)
    {
         cout << "Error on creating window" << endl;
         exit(EXIT_FAILURE);
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == nullptr)
    {
        cout << "Error on creating renderer" << endl;
        exit(EXIT_FAILURE);
    }
}

Appli::~Appli()
{
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}
