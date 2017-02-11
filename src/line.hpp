#ifndef LINE_HPP
#define LINE_HPP

#include <vector>
#include <iostream>
#include <vector>
#include <algorithm>

#include "linePoint.hpp"

class Line{
	private:
		std::vector<LinePoint> pts;				// Liste des points passant par cette droite
		
		long double a;							// Coefficient a de l'equation y = ax + b
		long double b;							// Biais de l'equation y = ax + b
		
		Point pivot;
		Point other;
		
		const unsigned NB_POINTS;				// Nombre de points contenus par droite
		const static unsigned EPSILON = 2.5; 	// Marge erreur
		double ESPISLON_CROSS_RATIO = 0.04;
		double crossRatio;						// Cross ratio de la droite
		
	public:
		// Contructeurs
		Line(double pA, double pB, unsigned pNbPoints = 4);
		Line(Point p1, Point p2);
		
		bool getIncludedPoints(vector<LinePoint>& allPoints);
		bool getIncludedPointsPolar(vector<LinePoint>& allPoints);
		
		void computeCrossRatio();
		
		double getCrossRatio(){
			return crossRatio;
		}
		
		
		// Setter/Getter
		const std::vector<LinePoint>& getPts() const{
			return pts;
		}
};

#endif
