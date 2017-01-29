#include <iostream>

using namespace std;

const float longueurBarre = 30.0;

int main()
{
    float pts[4];
    for(int i =0 ; i < 4; i ++){
        cout << "Saisir distance point "<< i + 1 << " de l'origine : " << endl;
        cin >> pts[i];
    }

    // calcul cross ratio
    float
    AC = pts[2] - pts[0],
    BD = pts[3] - pts[1],
    BC = pts[2] - pts[1],
    AD = pts[3] - pts[0];

    float crossRatio = (AC*BD) / (BC*AD);

    cout << "cross ratio = " << crossRatio << endl;



    return 0;
}

