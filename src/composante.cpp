/*!
 * \file main.cpp
 * \brief Implementation de l'objet composante
 * \author Chevalier_Pierre
 * \version 0.1
 */
 
#include "composante.hpp"

Composante::Composante(): pos(0,0) {}
		
std::vector<Composante> Composante::getCompostantes(const Mat& src){
	Mat copy(src);
	uchar* rowi;
	uchar pixelCur;
	std::vector<Composante> composantes;
	
	for(int i=0; i<copy.rows; i++){
		rowi = copy.ptr/*<uchar>*/(i);
		for(int j=0; j<copy.cols; j++){
			pixelCur = rowi[j];
			
			if(pixelCur!=0){
				Composante newComp;
				computeAComposante(copy, Point(i,j), newComp);
				composantes.push_back(newComp);
			}
		}
	}

	return composantes;
}

void Composante::computeAComposante(Mat& copy, Point pointCur, Composante& cur){
	int posx, posy;
	
	//Robustesse
	if(copy.ptr(pointCur.x)[pointCur.y] == 1){
		cur.addPoint(pointCur);					// On ajoute le point a la composante
		copy.ptr(pointCur.x)[pointCur.y] = 0;	// On passe le point a 0
		
		// On parcour le voisinage
		for(int i=-1; i<=1; i++){
			for(int j=-1; j<=1; j++){
				posx=pointCur.x + i;
				posy=pointCur.y + j;
			
				// Si le coordonnees sont valides et que le voisin est egal a 1
				if((posx>=0 && posx<copy.rows) &&
				  (posy>=0 && posy<copy.cols) &&
				   copy.ptr(i)[j] == 1){
				   	// On relance l'algo sur ce nouveau point, 
				   	// avec la reference de la matrice et de 
				   	// la composante modifie par l'appel courant
					computeAComposante(copy,Point(posx,posy),cur);
				}
			}
		}
	}
}
