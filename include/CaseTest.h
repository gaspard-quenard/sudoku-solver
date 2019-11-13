#ifndef CASETEST_H
#define CASETEST_H

#include <vector>
#include <iostream>

#include "Coordonnees.h"

class CaseTest
{
    public:
        CaseTest(Coordonnees _pos, bool _hypotheses[9][9][9]);
        virtual ~CaseTest();

        int getHypothese();

    protected:

    private:
        std::vector<int> Hypotheses; // Toutes les hypotheses que peut être la case test
        Coordonnees pos;

};

#endif // CASETEST_H
