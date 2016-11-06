/*!
 * \file main.cpp
 * \brief Implementation de l'objet composante
 * \author Chevalier_Pierre
 * \version 0.1
 */
 
#include "composante.hpp"

#include <iostream>
#include <opencv2/imgproc/imgproc.hpp>

Composante::Composante(): 
	_pos(0,0),
	_height(0),
	_width(0) {}
	
Composante::Composante(Mat& copy, Point pointCur): Composante(){
	this->computeAComposante(copy, pointCur);
	this->computeAtt();
}
	

void Composante::computeAtt(){

	int xMean=0, yMean=0,
		xMax=_points[0].x, xMin=_points[0].x, 
		yMax=_points[0].y, yMin=_points[0].y;

	// Parcours vecteur
	for(Point point: _points){
		// Sum
		xMean+=point.x;
		yMean+=point.y;
		
		// Calcul width
		if(xMax<point.x)
			xMax=point.x;
			
		if(xMin>point.x)
			xMin=point.x;
			
		// Calcul height
		if(yMax<point.y)
			yMax=point.y;
			
		if(yMin>point.y)
			yMin=point.y;
	}

	// Calcul moyenne
	xMean/=_points.size();
	yMean/=_points.size();

	// Ecriture attributs
	this->_pos.x = xMean;
	this->_pos.y = yMean;
	this->_width = xMax - xMin;
	this->_height = yMax - yMin;
}

void Composante::computeAComposante(Mat& copy, Point pointCur){
	int posx, posy;
	//Robustesse
	if(copy.ptr(pointCur.y)[pointCur.x]!=0){
		this->addPoint(pointCur);				// On ajoute le point a la composante
		copy.ptr(pointCur.y)[pointCur.x] = 0; 	// On passe le point a 0
		// On parcour le voisinage
		for(int i=-1; i<=1; i++){
			for(int j=-1; j<=1; j++){
				posx=pointCur.x + i;
				posy=pointCur.y + j;
			
				// Si le coordonnees sont valides et que le voisin est egal a 1
				if((posx>=0 && posx<copy.rows) &&
				  (posy>=0 && posy<copy.cols) &&
				   copy.ptr(posy)[posx] != 0){
				   	// On relance l'algo sur ce nouveau point, 
				   	// avec la reference de la matrice et de 
				   	// la composante modifie par l'appel courant
					this->computeAComposante(copy,Point(posx,posy));
				}
			}
		}
	}
}
		
std::vector<Composante> Composante::getCompostantes(const Mat& src){
	Mat copy = src.clone(); // Copy de la ref courante
	std::vector<Composante> composantes;
	
	// Parcours de l'image
	for(int j=0; j<copy.rows; j++){
		for(int i=0; i<copy.cols; i++){
			uchar& pixelCur = copy.ptr(j)[i];
			
			// Si le pixel courant est elligible
			if(pixelCur!=0){
				// On creer un nouveau composant et on l'ajoute au vecteur resultat
				composantes.push_back(Composante(copy, Point(i,j)));
			}
		}
	}

	return composantes;
}

