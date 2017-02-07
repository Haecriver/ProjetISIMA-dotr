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
		a = ((double)pi.y - (double)pj.y) / ((double)pi.x - (double)pj.x);
		b = pi.y - a * pi.x;
	}else{
		a = 0;
		b = 0;
	}
}

bool Line::getIncludedPoints(vector<LinePoint>& allPoints){
	double estimatedY;
	bool res = false;
	for(unsigned i = 0; i<allPoints.size(); i++){
		LinePoint pt = allPoints[i];
	 	estimatedY = a*(double)pt.getPos().x + b;
	 	/*std::cout << "a :" << a << std::endl;
	 	std::cout << "b :" << b << std::endl;
	 	std::cout << "x :" << pt.getPos().x << std::endl;
	 	std::cout << "y :" << pt.getPos().y << std::endl;
	 	std::cout << "estimatedY :" << estimatedY << std::endl;*/
		if(estimatedY  > pt.getPos().y - EPSILON && estimatedY < pt.getPos().y + EPSILON){
			// Considere comme valide
			pts.push_back(&allPoints[i]);
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
		
