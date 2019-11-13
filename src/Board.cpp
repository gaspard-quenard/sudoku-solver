#include "Board.h"

using namespace std;

Board::Board(SDL_Renderer* _renderer, SDL_Window* _window):
    renderer(_renderer),
    window(_window),
    errorSudoku(false)
{
    for (int i = 0; i < 9; i++)
        for (int j = 0; j < 9; j++)
            board[i][j] = -1;

    initializeHypotheses();

    loadTest();

    SDL_GetWindowSize(window, &witdh, &height);

    dest.w = witdh/9;
    dest.h = height/9;

    src.w = src.h = 200;
    src.y = 0;

    SDL_Surface* tmpSurface;
    if ((tmpSurface = IMG_Load("Chiffres.png")) == nullptr)
        cout << "Cannot load chiffres.png" << endl;

    chiffresTexture = SDL_CreateTextureFromSurface(renderer, tmpSurface);
    SDL_FreeSurface(tmpSurface);
}

Board::~Board()
{
    SDL_DestroyTexture(chiffresTexture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
}


void Board::printTerminal()
{
    cout << " -------------------------------------" << endl;
    for (int i = 0; i < 9; i++)
    {
        for (int j = 0; j < 9; j++)
        {
            if (board[i][j] == -1)
                cout << " |  ";
            else
                cout << " | " << board[i][j];
        }
        cout << " |" << endl << " -------------------------------------" << endl;
    }
}


void Board::printWindow()
{
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderClear(renderer);
    for (int i = 0; i < 10; i++)
    {
        if (i%3 == 0)
            SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        else
        {
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

        }

        SDL_RenderDrawLine(renderer, i*witdh/9, 0, i*witdh/9, height);
        SDL_RenderDrawLine(renderer, 0, i*height/9, witdh, i*height/9);

    }

    src.x = 200;
    dest.x = dest.y = 0;

    //reductionHypotheses();
    //writeNumbers();

    for (int i = 0; i < 9; i++)
        for (int j = 0; j < 9; j++)
        {
            if (board[i][j] != -1)
            {
                src.x = 200*board[i][j];
                dest.x = witdh/9*j;
                dest.y = height/9*i +i;
                SDL_RenderCopy(renderer, chiffresTexture, &src, &dest);
            }
            else
                printHypotheses(i,j);
        }

    SDL_RenderPresent(renderer);
}


void Board::initializeHypotheses()
{
    for (int i = 0; i < 9; i++)
        for (int j = 0; j < 9; j++)
            for (int k = 0; k < 9; k++)
                hypotheses[i][j][k] = true;

}

void Board::reductionHypotheses()
{
    for (int i = 0; i < 9; i++)
        for (int j = 0; j < 9; j++)
        {
            if (board[i][j] == -1)
            {
                reductionByColumn(i, j);
                reductionByRaw(i, j);
                reductionBySquare(i, j);
                if (ErrorInHypothese(i,j))
                {
                        cout << "MAIS LE SUDOKU QUE TU M'A DONNE EST IRRESOLVABLE !" << endl;
                        errorSudoku = true;
                        return;
                }
            }
        }
}


void Board::reductionByColumn(int caseX, int caseY)
{
    for (int i = 0; i < 9; i++)
    {
        if (board[i][caseY] != -1)
            hypotheses[caseX][caseY][board[i][caseY]-1] = false;
    }
}

void Board::reductionByRaw(int caseX, int caseY)
{
    for (int j = 0; j < 9; j++)
    {
        if (board[caseX][j] != -1)
            hypotheses[caseX][caseY][board[caseX][j]-1] = false;
    }
}


void Board::reductionBySquare(int caseX, int caseY)
{
    int i = caseX;
    int j = caseY;

    while (i-- % 3 != 0);
    while (j-- % 3 != 0);

    i++;
    j++;

    for (int k = i; k < i+3; k++)
        for (int l = j; l < j+3; l++)
        {
            if (board[k][l] != -1)
                hypotheses[caseX][caseY][board[k][l]-1] = false;
        }
}

void Board::writeNumbers()
{
    for (int i = 0; i < 9; i++)
        for (int j = 0; j < 9; j++)
            if (board[i][j] == -1 && uniqueHypotheseInCase(i,j))
            {
                board[i][j] = uniqueHypotheseInCase(i,j);
                //cout << "case rempli : " << i << " " << j << " (Methode case unique)" << endl;
                return;
            }

    int posX;
    int posY;

    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
            for (int k = 0; k < 9; k++)
                if (uniqueHypotheseInSquare9(3*i,3*j,k, &posX, &posY)) // Cette fonction est peut etre inutile
                {
                    //cout << "case rempli : " << posX << " " << posY << " (Methode square9)" << endl;
                    board[posX][posY] = k+1;
                    return;
                }

    //cout << "Je suis bloqué ici" << endl;
    Coordonnees t = findCaseWithLessHypotheses();
    tests.push_back(CaseTest(t, hypotheses));
    board[t.getCaseX()][t.getCaseY()] = tests[tests.size()-1].getHypothese();

}

int Board::uniqueHypotheseInCase(int caseX, int caseY)
{
    int nbHypotheses= 0;
    int number;
    for (int i = 0; i < 9; i++)
    {
        if (hypotheses[caseX][caseY][i])
        {
            nbHypotheses++;
            number = i+1;
        }
    }
    if (nbHypotheses == 1)
        return number;
    return 0;
}

bool Board::uniqueHypotheseInSquare9(int caseX, int caseY, int hypothese, int* responseX, int* responseY)
{
    int somme = 0;
    for (int i = caseX; i < caseX+3; i++)
        for (int j = caseY; j < caseY+3; j++)
        {
            //cout << i << " " << j << endl;
            if (board[i][j] == -1 && hypotheses[i][j][hypothese])
            {
                somme++;
                *responseX = i;
                *responseY = j;
            }
            if (somme > 1)
                return false;
        }
    //cout << hypothese << " : " << somme << endl;
    if (somme == 1)
        return true;
    return false;
}


void Board::printHypotheses(int caseX, int caseY)
{
    dest.w = witdh/27;
    dest.h = height/27;

    for (int k = 0; k < 9; k++)
    {
        if (hypotheses[caseX][caseY][k])
        {

            src.x = 200* (k+1);
            if (k+1 < 4)
                dest.x = witdh/9*caseY;
            else if (k+1 < 7)
                dest.x = witdh/9*caseY + witdh/27;
            else
                dest.x = witdh/9*caseY + 2*witdh/27;

            if (k+1 == 1
             || k+1 == 4
             || k+1 == 7)
                dest.y = height/9*caseX +caseX;

            else if (k+1 == 2
                  || k+1 == 5
                  || k+1 == 8)
                dest.y = height/9*caseX +caseX + height/27;

            else
                dest.y = height/9*caseX +caseX + 2*height/27;

            SDL_RenderCopy(renderer, chiffresTexture, &src, &dest);
        }
    }
    dest.w = witdh/9;
    dest.h = height/9;
}


bool Board::ErrorInHypothese(int caseX, int caseY)
{
    int somme = 0;
    for (int k = 0; k < 9; k++)
        somme+= hypotheses[caseX][caseY][k];
    if (somme == 0)
    {
        cout << "Error in hypotheses" << endl;
        return true;
    }
    return false;
}


Coordonnees Board::findCaseWithLessHypotheses()
{
    int nbHypotheses = 10;
    int caseX;
    int caseY;
    int somme;
    for (int i = 0; i < 9; i++)
        for (int j = 0; j < 9; j++)
        {
            if (board[i][j] == -1)
            {
                somme = 0;
                for (int k = 0; k < 9; k++)
                    somme+=hypotheses[i][j][k];
                if (somme < nbHypotheses)
                {
                    caseX = i;
                    caseY = j;
                    nbHypotheses = somme;
                }
            }
        }
    //cout << "La case avec le moins d'hypotheses est la case : " << caseX << " " << caseY << endl;
    return Coordonnees(caseX, caseY);
}


void Board::loadTest()
{
    /*board[0][0] = 7;
    board[0][4] = 6;
    board[0][6] = 3;

    board[1][4] = 8;
    board[1][5] = 9;
    board[1][6] = 1;

    board[2][0] = 8;
    board[2][5] = 4;

    board[3][2] = 9;
    board[3][7] = 8;

    board[4][0] = 2;
    board[4][1] = 8;
    board[4][4] = 1;
    board[4][7] = 7;
    board[4][8] = 3;

    board[5][1] = 4;
    board[5][6] = 5;

    board[6][3] = 1;
    board[6][8] = 4;

    board[7][2] = 5;
    board[7][3] = 4;
    board[7][4] = 7;

    board[8][2] = 3;
    board[8][4] = 2;
    board[8][8] = 6;*/
}

bool Board::isBoardFull()
{
    for (int i = 0; i < 9; i++)
        for (int j = 0; j < 9; j++)
        {
            if (board[i][j] == -1)
                return false;
        }
    return true;
}


void Board::initializeBoard()
{
    SDL_Event e;
    bool endInitiliazing = false;
    int windowWitdh;
    int windowHeight;
    int resultat;

    int tempsPresent;
    int tempsPrecedant = 0;

    SDL_GetWindowSize(window, &windowWitdh, &windowHeight);
    while (!endInitiliazing)
    {
        tempsPresent = SDL_GetTicks();
        //if (tempsPresent - tempsPrecedant > 30)
        {
            SDL_PollEvent(&e);
            switch (e.type)
            {
                case SDL_KEYDOWN:
                    if (e.key.keysym.sym == SDLK_SPACE || e.key.keysym.sym == SDLK_ESCAPE)
                    {
                        cout << "end initializing" << endl;
                        endInitiliazing = true;
                    }
                    break;

                case SDL_MOUSEBUTTONDOWN:
                    int mousePosX;
                    int mousePosY;
                    SDL_GetMouseState(&mousePosX, &mousePosY);
                    //cout << "Pos X : " << mousePosX << " Pos Y : " << mousePosY << endl;
                    if (mousePosX > 0 && mousePosX < windowWitdh
                    && mousePosY > 0 && mousePosY < windowHeight)
                    {
                        double caseX = (double) mousePosX/(windowWitdh/9);
                        double caseY = (double) mousePosY/(windowHeight/9);
                        cout << (int) caseX << " " << (int) caseY;
                        cout << " : ";
                        cin >> resultat;
                        board[(int) caseY][(int) caseX] = resultat;
                        printWindow();
                    }
                    break;

            }
            tempsPrecedant = tempsPresent;
        }
        //else
          //  SDL_Delay(30 - (tempsPresent - tempsPrecedant));
    }
    cout << endl;
    printTerminal();
}
