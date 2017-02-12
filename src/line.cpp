#include "line.hpp"

Line::Line():Line(0.0,0.0){}

Line::Line(double pA, double pB):
a(pA),b(pB)
{}


Line::Line(Point p1, Point p2) : Line() {
	pivot = Point(p1.x,p1.y);
	other = Point(p2.x,p2.y);
	Point pi,pj;
	// On prend le point le plus a droite
	if(p1.x < p2.x){
		pi = p1;
		pj = p2;
	}else{
		pi = p2;
		pj = p1;
	}

	// Si les deux points sont different (pour eviter une division par zero)
	if(pi.x != pj.x){
		// Calcul du coefficient a
		a = ((double)pi.y - (double)pj.y) / ((double)pi.x - (double)pj.x);
		b = pi.y - a * pi.x; // Calcul du biais
	}else{
		// Sinon on pass tout a zero
		a = 0;
		b = 0;
	}
	
	// On cacul le theta du point qui fait la ligne
	theta = atan(((double)other.y-(double)pivot.y)/((double)other.x-(double)pivot.x)) * 180 / M_PI;
}

bool Line::getIncludedPoints(vector<LinePoint>& allPoints, bool careBelongsToLine){
	double estimatedY;
	bool res = false;
 	vector<LinePoint*> ref_pts;
	for(unsigned i = 0; i<allPoints.size(); i++){
		LinePoint pt = allPoints[i];
	 	estimatedY = a*(double)pt.getPos().x + b;	// Calcul du y avec l'equation de la droite

	 	// On verifie que le point estimee correspond avec le vrai point y
	 	// Si c'est le cas alors le point est sur la droite
		if(!(pt.isBelongsToLine() && careBelongsToLine) &&
			estimatedY  > pt.getPos().y - EPSILON && estimatedY < pt.getPos().y + EPSILON){
			// Considere comme valide
			ref_pts.push_back(&allPoints[i]);
		}
	}
	
	// Si on a le bon nombre de points
	if(ref_pts.size() == NB_POINTS){
		res = true;
		// On les trie
		sort(ref_pts.begin(), ref_pts.end(), 
			[](LinePoint* a, LinePoint* b) -> bool
			{ 
				return a->getPos().x > b->getPos().x; 
			}
		);
		
		// On associe les points a la droite
		for (LinePoint* pt : ref_pts){
			if(careBelongsToLine){
				pt->setBelongsToLine(true);
			} else {
				pt->incNbLines();
			}
			pts.push_back(LinePoint(*pt));
		}
		
		// On calcul le crossRatio
		computeCrossRatio();
	}
	return res;
}

bool Line::getIncludedPointsPolar(vector<LinePoint>& allPoints, bool careBelongsToLine){
	bool res = false;
	double theta_line, theta_cur;
 	vector<LinePoint*> ref_pts;
 	
 	// on va retirer pivot.x, pivot.y aux autres points 
	// pour deplacer le repere sur ce point
		
	for(unsigned i = 0; i<allPoints.size(); i++){
		LinePoint pt = allPoints[i];
		
		// Cas ou on tombe sur le pivot
		if(pt.getPos().y == pivot.y && pt.getPos().x == pivot.x){
			// On le rajoute
			ref_pts.push_back(&allPoints[i]);
		} else {
			// On cacul le theta d'un autre point
			theta_cur = atan(((double)pt.getPos().y-(double)pivot.y)/((double)pt.getPos().x-(double)pivot.x)) * 180 / M_PI;
		
			// On verifie que le point estimee correspond avec le vrai point y
		 	// Si c'est le cas alors le point est sur la droite
			if(!(pt.isBelongsToLine() && careBelongsToLine) &&
				theta_cur  > theta - EPSILON && theta_cur < theta + EPSILON){
				// Considere comme valide
				ref_pts.push_back(&allPoints[i]);
			}
		}
	}
	
	// Si on a le bon nombre de points
	if((careBelongsToLine && ref_pts.size() == NB_POINTS) || (!careBelongsToLine && ref_pts.size() >= NB_POINTS)){	
		
		// On les trie
		sort(ref_pts.begin(), ref_pts.end(), 
			[](LinePoint* a, LinePoint* b) -> bool
			{ 
				return a->getPos().x > b->getPos().x; 
			}
		);

		// On associe les points a la droite
		for (LinePoint* pt : ref_pts){
			if(careBelongsToLine){
				pt->setBelongsToLine(true);
			}
			pts.push_back(LinePoint(*pt));
		}

		// On calcul le crossRatio
		if(ref_pts.size() == NB_POINTS){
			computeCrossRatio();
		}
		res = true;
		
	}
	return res;
}

void Line::computeCrossRatio(){
	if(pts.size() == 4){
		crossRatio = (pts[0].getDistance(pts[2]) * pts[1].getDistance(pts[3])) / 
		(pts[1].getDistance(pts[2]) * pts[0].getDistance(pts[3]));
	}else{
		std::cout << "Nb pts incorrect" << std::endl;
	}
}
		
