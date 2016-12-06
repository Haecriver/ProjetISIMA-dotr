/*!
 * \file main.cpp
 * \brief Implementation de l'objet composante
 * \author Chevalier_Pierre
 * \version 0.1
 */
 
#include "composante.hpp"

#include <iostream>
#include <stack>
#include <opencv2/imgproc/imgproc.hpp>



Composante::Composante(): 
	_pos(0,0),
	_height(0),
	_width(0)
	 {}
	
Composante::Composante(Mat& copy, Point pointCur): Composante(){
	computeAComposante(copy, pointCur);
	computeAtt();
}

void Composante::addPoint(Point point){
	_points.push_back(point);
}

void Composante::computeAtt(){
	if(_points.size() != 0){
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
		_pos.x = xMean;
		_pos.y = yMean;
		
		_baseline.x = xMin;
		_baseline.y = yMin;
		_width = xMax - xMin + 1;
		_height = yMax - yMin + 1;
		
		// Creation de l'image de la comp
		_image.create(_height,_width,CV_8SC1);
		
		for(Point point: _points){
			int x = point.x - _baseline.x;
			int y = point.y - _baseline.y;
			_image.ptr(y)[x] = 255;
		}		
	}
}

void Composante::computeAComposante(Mat& copy, Point pPointCur){
	int posx, posy;
	bool isValide;
	std::stack<Point> pile;
	pile.push(pPointCur);
		
	Point pointCur;
	
	while(!pile.empty()){
		pointCur = pile.top();
		pile.pop();
	
		//Robustesse
		if((pointCur.x>=0 && pointCur.x<copy.rows) &&
			(pointCur.y>=0 && pointCur.y<copy.cols) &&
			copy.ptr(pointCur.y)[pointCur.x] == MAX_BINARY_VALUE){
					
			addPoint(pointCur);				// On ajoute le point a la composante
			copy.ptr(pointCur.y)[pointCur.x] = 0; 	// On passe le point a 0
			
			// On parcour le voisinage
			for(int i=-1; i<=1; i++){
				for(int j=-1; j<=1; j++){
					posx=pointCur.x + i;
					posy=pointCur.y + j;
			
					// Si le coordonnees sont valides et que le voisin est egal a 1
					isValide = (posx>=0 && posx<copy.rows) &&
					  (posy>=0 && posy<copy.cols) &&
					  (copy.ptr(posy)[posx] == MAX_BINARY_VALUE);
					
					if(isValide){

					   	// On relance l'algo sur ce nouveau point, 
					   	// avec la reference de la matrice et de 
					   	// la composante modifie par l'appel courant					
						Point point(posx,posy);
						pile.push(point);
										
					}
				}
			}
		}
	}
}
		
std::vector<Composante> Composante::getComposantes(const Mat& src, unsigned max_comp){
	Mat copy(src.clone()); // Copy de la ref courante
	std::vector<Composante> composantes;
	
	// Parcours de l'image
	for(int j=0; j<copy.rows; j++){
		for(int i=0; i<copy.cols; i++){	
			// Si le pixel courant est elligible
			if( copy.ptr(j)[i] == MAX_BINARY_VALUE && composantes.size() <= max_comp){
				// On creer un nouveau composant et on l'ajoute au vecteur resultat
				composantes.push_back(Composante(copy, Point(i,j)));
			}
		}
	}

	return composantes;
}

