#include "line.hpp"

Line::Line(double pA, double pB, unsigned pNbPoints):
a(pA),b(pB),
NB_POINTS(pNbPoints)
{}


Line::Line(Point p1, Point p2) : Line(0.0,0.0) {
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
}

bool Line::getIncludedPoints(vector<LinePoint>& allPoints){
	double estimatedY;
	bool res = false;
 	vector<LinePoint*> ref_pts;
	for(unsigned i = 0; i<allPoints.size(); i++){
		LinePoint pt = allPoints[i];
	 	estimatedY = a*(double)pt.getPos().x + b;	// Calcul du y avec l'equation de la droite
	 	
	 	/*std::cout << "a :" << a << std::endl;
	 	std::cout << "b :" << b << std::endl;
	 	std::cout << "x :" << pt.getPos().x << std::endl;
	 	std::cout << "y :" << pt.getPos().y << std::endl;
	 	std::cout << "estimatedY :" << estimatedY << std::endl;*/
	 	
	 	// On verifie que le point estimee correspond avec le vrai point y
	 	// Si c'est le cas alors le point est sur la droite
		if(!pt.isBelongsToLine() &&
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
			pt->setBelongsToLine(true);
			pts.push_back(LinePoint(*pt));
		}
		
		// On calcul le crossRatio
		computeCrossRatio();
	}
	return res;
}

bool Line::getIncludedPointsPolar(vector<LinePoint>& allPoints){
	bool res = false;
	double theta_line, theta_cur;
 	vector<LinePoint*> ref_pts;
 	
 	// on va retirer pivot.x, pivot.y aux autres points 
	// pour deplacer le repere sur ce point

	// On cacul le theta du point qui fait la ligne
	theta_line = atan(((double)other.y-(double)pivot.y)/((double)other.x-(double)pivot.x)) * 180 / M_PI;
		
	for(unsigned i = 0; i<allPoints.size(); i++){
		LinePoint pt = allPoints[i];
		
		// Cas ou on tombe sur le pivot
		if(pt.getPos().y == pivot.y && pt.getPos().x == pivot.x){
			// On le rajoute
			ref_pts.push_back(&allPoints[i]);
		} else {
			// On cacul le theta d'un autre point
			theta_cur = atan(((double)pt.getPos().y-(double)pivot.y)/((double)pt.getPos().x-(double)pivot.x)) * 180 / M_PI;
		
			/*std::cout << "theta :" << theta_line << std::endl;
		 	std::cout << "theta cur :" << theta_cur << std::endl;*/
		 	
			// On verifie que le point estimee correspond avec le vrai point y
		 	// Si c'est le cas alors le point est sur la droite
			if(!pt.isBelongsToLine() &&
				theta_cur  > theta_line - EPSILON && theta_cur < theta_line + EPSILON){
				// Considere comme valide
				ref_pts.push_back(&allPoints[i]);
			}
		}
	}
	
	// Si on a le bon nombre de points
	if(ref_pts.size() == NB_POINTS){		
		
		// On les trie
		sort(ref_pts.begin(), ref_pts.end(), 
			[](LinePoint* a, LinePoint* b) -> bool
			{ 
				return a->getPos().x > b->getPos().x; 
			}
		);

		// On associe les points a la droite
		for (LinePoint* pt : ref_pts){
			pt->setBelongsToLine(true);
			pts.push_back(LinePoint(*pt));
		}

		// On calcul le crossRatio
		computeCrossRatio();
		
		// Comparer avec la ligne qui avait le meme cross ratio, et ne pas l'accepter si ça fait nimporte quoi
		// (Compter le nombre de fois ou la ligne est constester)
		
		res = (crossRatio >  1.666 - ESPISLON_CROSS_RATIO && crossRatio <  1.666 + ESPISLON_CROSS_RATIO)||
			(crossRatio >  1.111 - ESPISLON_CROSS_RATIO && crossRatio <  1.111 + ESPISLON_CROSS_RATIO) ||
			(crossRatio >  2.0 - ESPISLON_CROSS_RATIO && crossRatio <  2.0 + ESPISLON_CROSS_RATIO);
		
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
		
