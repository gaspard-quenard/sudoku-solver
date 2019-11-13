#ifndef APPLI_H
#define APPLI_H

#include <SDL2/SDL.h>
#include <iostream>

class Appli
{
    public:
        Appli();
        virtual ~Appli();

        SDL_Renderer* getRenderer() {return renderer;};
        SDL_Window* getWindow() {return window;};

    protected:

    private:
        SDL_Renderer* renderer;
        SDL_Window* window;

};

#endif // APPLI_H
