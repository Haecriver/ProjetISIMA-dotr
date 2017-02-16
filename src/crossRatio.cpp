#include <iostream>
#include <algorithm>

using namespace std;

const float longueurBarre = 30.0;

float cpt_distance(float pts1, float pts2){
	return abs(pts1-pts2);
}

int main()
{
    float pts[4];
    float crPts[4];
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
    
    // calcul des cr de points
    for (int i = 0; i < 4; i++){
    	float dsts[3];
    	// calcul du vecteur de distance
    	int index = 0;
    	for(int j = 0; j < 4; j++){
    		if(i!=j){
    			dsts[index] = cpt_distance(pts[i],pts[j]);
    			index ++;
    		}
    	}
    	std::sort (dsts, dsts+3); 
    	crPts[i]=(dsts[0]/dsts[2]);
    }
    
    for(int i=0;i<4;i++){
    	 cout << "pt: " << pts[i] << " cr: " << crPts[i] << endl;
    }



    return 0;
}

