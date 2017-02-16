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
/*
bool Line::getIncludedPoints(std::vector<LinePoint>& allPoints, bool careBelongsToLine){
	double estimatedY;
	bool res = false;
 	std::vector<LinePoint*> ref_pts;
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
			pts.push_back(pt);
		}
		
		// On calcul le crossRatio
		computeCrossRatio();
	}
	return res;
}*/

bool Line::getIncludedPointsPolar(std::vector<LinePoint>& allPoints){
	bool res = false;
	double theta_cur;
	
	double epsilon_theta_cur = EPSILON_THETA;
 	std::vector<LinePoint*> ref_pts;
 	
	// On fait varier la marge d'erreur
	for(epsilon_theta_cur = EPSILON_THETA; epsilon_theta_cur <= EPSILON_THETA_MAX && ref_pts.size() != NB_POINTS; epsilon_theta_cur += 0.2){
		// On nettoie le vecteur s'il n'est pas vide
		if(!ref_pts.empty()) { ref_pts.clear(); }
		
		// on va retirer pivot.x, pivot.y aux autres points 
		// pour deplacer le repere sur ce point
		
		for(unsigned i = 0; i<allPoints.size(); i++){
			LinePoint pt = allPoints[i];
			
			// Cas ou on tombe sur le pivot
			if(pt.getPos().y == pivot.y && pt.getPos().x == pivot.x){
				// On le rajoute
				allPoints[i].setModuleCur(0.0);
				ref_pts.push_back(&allPoints[i]);
			} else {
				// On cacul le theta d'un autre point
				theta_cur = atan(((double)pt.getPos().y-(double)pivot.y)/((double)pt.getPos().x-(double)pivot.x)) * 180 / M_PI;
			
				// On verifie que le point estimee correspond avec le vrai point y
				// Si c'est le cas alors le point est sur la droite
				if((!pt.isBelongsToLine() || pt.isExtremite()) &&
					theta_cur  > theta - epsilon_theta_cur && theta_cur < theta + epsilon_theta_cur){
					// Considere comme valide
					allPoints[i].setModuleCur(
						(pt.getPos().y-pivot.y)*(pt.getPos().y-pivot.y) + 
						(pt.getPos().x-pivot.x)*(pt.getPos().x-pivot.x));
					
					ref_pts.push_back(&allPoints[i]);
				}
			}
		}
	}
	
	// Si on a le bon nombre de points
	if(ref_pts.size() == NB_POINTS){
			
		// On les trie en fonction de leur module par rapport au pivot
		sort(ref_pts.begin(), ref_pts.end(), 
			[](LinePoint* a, LinePoint* b) -> bool
			{ 
				return a->getModuleCur() > b->getModuleCur(); 
				//return a->getPos().x > b->getPos().x; 
			}
		);

		// Maintenant le 1er points est une extremite
		LinePoint extremite = LinePoint(*ref_pts[0]);
		
		// On les en fonction de leur distance par rapport au 1er point
		sort(ref_pts.begin(), ref_pts.end(), 
			[&, extremite](LinePoint* a, LinePoint* b) -> bool
			{ 
				return a->getDistance(extremite) > b->getDistance(extremite); 
			}
		);
	
		// On notifie les extremites
		ref_pts[0]->setExtremite(true);
		ref_pts[NB_POINTS-1]->setExtremite(true);

		// On associe les points a la droite
		for (LinePoint* pt : ref_pts){
			pt->setBelongsToLine(true);
			pt->incNbLines();
			pts.push_back(pt);
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
		crossRatio = (pts[0]->getDistance(*pts[2]) * pts[1]->getDistance(*pts[3])) / 
		(pts[1]->getDistance(*pts[2]) * pts[0]->getDistance(*pts[3]));
	}else{
		std::cout << "Nb pts incorrect" << std::endl;
	}
}

bool Line::firstPointsHasGoodRatio(double ratioPoint){
	double dstPointDeb[3], dstPointFin[3];
    int index = 0;
    double ratioPointDeb, ratioPointFin;
    	
	for(int i = 1; i < 4; i++){
		dstPointDeb[index] = pts[0]->getDistance(*pts[i]);
		index ++;
	}
	index = 0;
	for(int i = 0; i < 3; i++){
		dstPointFin[index] = pts[3]->getDistance(*pts[i]);
		index ++;
	}
	
	std::sort (dstPointDeb, dstPointDeb+3);
	std::sort (dstPointFin, dstPointFin+3);
	
	ratioPointDeb = dstPointDeb[0]/dstPointDeb[2];
	ratioPointFin = dstPointFin[0]/dstPointFin[2];

	std::cout << "deb:" << pts[0]->getPos() << ";fin:" <<  pts[3]->getPos() << std::endl;
	std::cout << "rdeb:" << ratioPointDeb << ";rfin:" << ratioPointFin << std::endl;
	if(abs(ratioPointDeb - ratioPoint) < abs(ratioPointFin - ratioPoint)){
		std::cout << "on inverse pas" << std::endl;
	}else{
		std::cout << "on inverse" << std::endl;
	}

	return abs(ratioPointDeb - ratioPoint) < abs(ratioPointFin - ratioPoint);
}
		
