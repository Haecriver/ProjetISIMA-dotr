#ifndef LINE_HPP
#define LINE_HPP

#include <vector>
#include <iostream>
#include <vector>
#include <algorithm>

#include "linePoint.hpp"

class Line{
	private:
		std::vector<LinePoint*> pts;			// Liste des points passant par cette droite
		double a;								// Coefficient a de l'equation y = ax + b
		double b;								// Biais de l'equation y = ax + b
		const unsigned NB_POINTS;				// Nombre de points contenus par droite
		const static unsigned EPSILON = 2.0; 	// Marge erreur
		double crossRatio;						// Cross ratio de la droite
		
	public:
		// Contructeurs
		Line(double pA, double pB, unsigned pNbPoints = 4);
		Line(Point p1, Point p2);
		
		bool getIncludedPoints(vector<LinePoint>& allPoints);
		
		void computeCrossRatio();
		
		double getCrossRatio(){
			return crossRatio;
		}
		
		
		// Setter/Getter
		const std::vector<LinePoint*>& getPts() const{
			return pts;
		}
};

#endif
