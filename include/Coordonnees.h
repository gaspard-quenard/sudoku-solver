#ifndef COORDONNEES_H
#define COORDONNEES_H


class Coordonnees
{
    public:
        Coordonnees(int _caseX, int _caseY);
        virtual ~Coordonnees();

        int getCaseX() {return caseX;};
        int getCaseY() {return caseY;};

    protected:

    private:
        int caseX;
        int caseY;
};

#endif // COORDONNEES_H
