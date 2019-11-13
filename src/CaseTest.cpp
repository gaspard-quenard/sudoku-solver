#include "CaseTest.h"

using namespace std;

CaseTest::CaseTest(Coordonnees _pos, bool _hypotheses[9][9][9]):
    pos(_pos)
{
    for (int k = 0; k < 9; k++)
    {
        if (_hypotheses[pos.getCaseX()][pos.getCaseY()][k])
        {
            Hypotheses.push_back(k+1);
            //cout << "Je rajoute l'hypotheses : " << k+1  << endl;
        }
    }
}

CaseTest::~CaseTest()
{
    //dtor
}


int CaseTest::getHypothese()
{
    int k = Hypotheses[Hypotheses.size()-1];
    Hypotheses.pop_back();
    //cout << "Je donne l'hypothese " << k << " Je l'enlève donc de mon vector d'hypothese" << endl;
    return k;
}
