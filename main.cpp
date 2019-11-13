#include <iostream>

#include "Appli.h"
#include "Board.h"

using namespace std;

int main()
{

    Appli* a = new Appli();
    Board* b = new Board(a->getRenderer(), a->getWindow());
    b->printTerminal();
    b->printWindow();

    b->initializeBoard();

    SDL_Event e;
    bool quit = false;
    bool boardFull = false;

    int tempsPresent;
    int tempsPrecedant = 0;

    cout << "TOUCHE t pour construire immédiatement le sudoku fini," << endl
        << "Touche espace pour ne rajouter qu'un chiffre" << endl;

    while (!quit)
    {
        tempsPresent = SDL_GetTicks();
        if (tempsPresent - tempsPrecedant > 30)
        {
            SDL_PollEvent(&e);
            switch (e.type)
            {
                case SDL_KEYDOWN:
                {
                    if (e.key.keysym.sym == SDLK_ESCAPE)
                        quit = true;

                    else if (e.key.keysym.sym == SDLK_SPACE && !boardFull)
                    {
                        b->reductionHypotheses();
                        b->writeNumbers();
                        b->printWindow();
                        if (b->isBoardFull() || b->errorInSudoku())
                        {
                            cout << "FINI ! " << endl;
                            boardFull = true;
                        }
                    }
                    else if (e.key.keysym.sym == SDLK_t && !boardFull)
                    {
                        while (!b->isBoardFull() || b->errorInSudoku())
                        {
                            b->reductionHypotheses();
                            b->writeNumbers();
                            b->printWindow();
                            //b->printTerminal();
                        }
                        cout << "FINI ! " << endl;
                        b->printTerminal();
                        boardFull = true;
                    }
                }
            }
            tempsPrecedant = tempsPresent;
        }
        else
            SDL_Delay(30 - (tempsPresent - tempsPrecedant));
    }

    delete a;
    delete b;
    return 0;
}
