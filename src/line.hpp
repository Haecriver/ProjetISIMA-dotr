#ifndef LINE_HPP
#define LINE_HPP

#include <vector>
#include <iostream>
#include <vector>
#include <algorithm>

#include "linePoint.hpp"

class Line{
	private:
		std::vector<LinePoint*> pts;
		double a;
		double b;
		const unsigned NB_POINTS;
		const static unsigned EPSILON = 2.0; // Marge erreur
		
		double crossRatio;
		
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
