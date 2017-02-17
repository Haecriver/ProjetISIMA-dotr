#ifndef LINE_HPP
#define LINE_HPP

#include <vector>
#include <iostream>
#include <vector>
#include <algorithm>

#include "linePoint.hpp"
#include "axe.hpp"

class Line{
	private:
		std::vector<LinePoint*> pts;				// Liste des points passant par cette droite
		
		long double a;							// Coefficient a de l'equation y = ax + b
		long double b;							// Biais de l'equation y = ax + b
		long double theta;
		
		Point pivot;
		Point other;
		
		const static unsigned NB_POINTS = 4;				// Nombre de points contenus par droite
		constexpr static double EPSILON_THETA = 2.0; 		// Marge erreur
		constexpr static double EPSILON_THETA_MAX = 3.0; 	// Marge erreur max
		constexpr static double EPSILON_CROSS_RATIO = 0.20;
		double crossRatio;						// Cross ratio de la droite
		const Axe* axe;
		
	public:
		// Contructeurs
		Line();
		Line(double pA, double pB);
		Line(Point p1, Point p2);
		
		bool getIncludedPoints(std::vector<LinePoint>& allPoints, bool careBelongsToLine=true);
		bool getIncludedPointsPolar(std::vector<LinePoint>& allPoints);
		
		bool sameCrossRatio(double pCrossRatio, double epsilon = EPSILON_CROSS_RATIO) const{
			return (crossRatio >  pCrossRatio - epsilon) 
					&& (crossRatio < pCrossRatio + epsilon);
		}
		
		void computeCrossRatio();
		
		double getCrossRatio(){
			return crossRatio;
		}
		
		void setAxe(const Axe* paxe){
			axe = paxe;
		}
		
		const Axe* getAxe(){
			return axe;
		}
		
		
		// Setter/Getter
		const std::vector<LinePoint*>& getPts() const{
			return pts;
		}
		
		// Setter/Getter
		void setPts(std::vector<LinePoint*> ppts){
			pts.clear();
			for(unsigned i=0; i<ppts.size() ; i++){
				pts.push_back(ppts[i]);
			}	
		}
		
		bool firstPointsHasGoodRatio(double ratioPoint);
		
		void reversePoints(){
			std::reverse(pts.begin(),pts.end());
		}
};

#endif
