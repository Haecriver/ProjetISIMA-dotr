#include "line.hpp"

Line::Line(double pA, double pB, unsigned pNbPoints):
a(pA),b(pB),
NB_POINTS(pNbPoints)
{}


Line::Line(Point p1, Point p2) : Line(0.0,0.0) {
	Point pi,pj;
	if(p1.x < p2.x){
		pi = p1;
		pj = p2;
	}else{
		pi = p2;
		pj = p1;
	}

	if(pi.x != pj.x){
		a = (pi.y - pj.y) / (pi.x - pj.x);
		b = pi.y - a * pi.x;
		//std::cout << "ok" << std::endl;
	}else{
		a = 0;
		b = 0;
		//std::cout << "Erreur, p1 = p2" << std::endl;
	}
}

bool Line::getIncludedPoints(vector<LinePoint>& allPoints){
	double estimatedY;
	bool res = false;
	for(LinePoint& pt : allPoints){
	 	estimatedY = a*pt.getPos().x + b;
		if(estimatedY  > pt.getPos().y - EPSILON && estimatedY < pt.getPos().y + EPSILON){
			// Considere comme valide
			pts.push_back(&pt);
		}
	}
	
	// Si on a le bon nombre de points
	if(pts.size() == NB_POINTS){
		res = true;
		// On les trie
		sort(pts.begin(), pts.end(), 
			[](LinePoint* a, LinePoint* b) -> bool
			{ 
				return a->getPos().x > b->getPos().x; 
			}
		);
		
		// On associe les points a la droite
		for (LinePoint* pt : pts){
			pt->setBelongsToLine(true);
		}
		
		// On calcul le crossRatio
		computeCrossRatio();
	}
	return res;
}

void Line::computeCrossRatio(){
	if(pts.size() == 4){
		crossRatio = (pts[0]->getDistance(*pts[2]) * pts[1]->getDistance(*pts[3])) / 
		(pts[1]->getDistance(*pts[2]) * pts[0]->getDistance(*pts[3]));
	}else{
		std::cout << "Nb pts incorrect" << std::endl;
	}
}
		
