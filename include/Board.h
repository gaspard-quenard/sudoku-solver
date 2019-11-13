#ifndef BOARD_H
#define BOARD_H

#include <iostream>
#include <vector>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "Coordonnees.h"
#include "CaseTest.h"

class Board
{
    public:
        Board(SDL_Renderer* _renderer, SDL_Window* _window);
        virtual ~Board();

        void initializeBoard();

        void printTerminal();
        void printWindow();
        void printHypotheses(int caseX, int caseY);
        void loadTest();
        void initializeHypotheses();


        void reductionHypotheses();
        void reductionByColumn(int caseX, int caseY);
        void reductionByRaw(int caseX, int caseY);
        void reductionBySquare(int caseX, int caseY);

        int uniqueHypotheseInCase(int caseX, int caseY);  // Retourne l'hypothese si elle est unique, 0 sinon
        bool uniqueHypotheseInSquare9(int caseX, int caseY, int i, int* resonseX, int* resonseY); // Retourne true si l'hypothese i est unique dans un carré de 9 cases
                                                                                                    // et indique le carré dans responseX et resonseY

        bool ErrorInHypothese(int caseX, int caseY); // Retoune true si un carre n'a pas de chiffres ni d'hypotheses

        bool errorInSudoku() {return errorSudoku;};
        Coordonnees findCaseWithLessHypotheses(); // Retoune la case avec le moins d'hypotheses

        bool isBoardFull();

        void writeNumbers();



    protected:

    private:

        int board[9][9];
        SDL_Renderer* renderer;
        SDL_Window* window;
        SDL_Texture* chiffresTexture;


        int witdh;
        int height;

        SDL_Rect src;
        SDL_Rect dest;

        bool hypotheses[9][9][9];
        bool errorSudoku;

        std::vector<CaseTest> tests;

};

#endif // BOARD_H
