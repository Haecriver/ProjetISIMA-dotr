#include "detectLine.hpp"

std::random_device rd;
std::mt19937 gen(rd());

// Contructeurs
DetectLine::DetectLine(const std::vector<Composante>& pcomps, unsigned pNbLines, bool pDisplaySearching):
comps(pcomps),
NB_LINES(pNbLines),
DISPLAY_SEARCHING(pDisplaySearching)
{}

// Methodes
Mat DetectLine::render(Mat& img){
	Mat res(img.clone());
	
	// On calcul les lignes depuis le vecteur de composants
	// donne au constructeur
	getLinesFromPoints(res);
	
	// On affiche les lignes graphiquement
	for(Line el_line: lines){
		line(res, el_line.getPts()[0]->getPos(), 
			el_line.getPts()[3]->getPos(),
			Scalar(255, 255, 0), 3);
	}
	
	return res;
}

void DetectLine::getLinesFromPoints(Mat& img){
	bool searchingForLines = true, searchingForPoints = true;
	Mat display, display_cur;
	unsigned nb_iterations = 0;
		
	if(DISPLAY_SEARCHING){
		display = img.clone();
	}
	
	// On initialise les vecteurs de points qui vont etre utilises
	// a partir du vecteur de composant passe au constructeur
	allPoints = LinePoint::convertCompToLinePoint(comps);
	for(unsigned i = 0; i < allPoints.size(); i++){
		lonelyPoints.push_back(&allPoints[i]);
	}
	
	// Tant que toutes les lignes n'ont pas ete trouvees
	// Ou que le nombre d'iterations de l'algo a depasse le nb max
	while(searchingForLines){
		// On tire un point a partir duquel on cherche les lignes
		LinePoint* pivot = *selectRandomPoint(lonelyPoints);
		lonelyPoints.remove(pivot); // On supprime ce point des tirages possibles
		searchingForPoints = true;
		
		while(searchingForPoints){
			// On tire un point pour tirer une ligne
			LinePoint* cur = *selectRandomPoint(lonelyPoints);
			lonelyPoints.remove(cur); // On supprime ce point des tirages possibles
			
			// On tire la ligne
			Line lineCur(pivot->getPos(), cur->getPos());
			
			// On l'affiche si demande
			if(DISPLAY_SEARCHING){
				display_cur = display.clone();
				
				line(display_cur, pivot->getPos(), 
					cur->getPos(),
					Scalar(0, 255, 0), 1);
				
				imshow("Searching",display_cur);
				waitKey(1000);
				display_cur.release();
			}
			
			// On test si la droite creee, passe par 4 points
			if(lineCur.getIncludedPoints(allPoints)){
				// Si oui
				// On stock la ligne
				lines.push_back(lineCur);
				
				// On reinitialise le nombre d'iteration
				nb_iterations = 0;
				
				// On stop la boucle
				searchingForPoints = false;
				
				// On l'affiche si demande
				if(DISPLAY_SEARCHING){
					line(display, lineCur.getPts()[0]->getPos(), 
						lineCur.getPts()[3]->getPos(),
						Scalar(255, 255, 0), 1);
				
					imshow("Searching",display);
				}
			}
			
			// Si on a teste tous les points disponibles
			if(lonelyPoints.empty()){
				// On affiche un message d'erreur (car ca ne devrait pas arriver si tous
				// les points sont la)
				std::cout << "Erreur: aucune ligne detectee pour le point " << 
				"x:"<<pivot->getPos().x << " y:" << pivot->getPos().y << std::endl;
				
				// On increment le nombre d'iteration d'erreurs
				nb_iterations++;
				
				// On stop la boucle
				searchingForPoints = false;
			}
		}
		
		// On nettoie le vecteur de point a stocker dans des lignes
		lonelyPoints.erase(lonelyPoints.begin(), lonelyPoints.end());
		
		// On met a jout le vecteur des points en n'incluant les points
		// deja compris dans les lignes
		for(unsigned i = 0; i < allPoints.size(); i++){
			if(!allPoints[i].isBelongsToLine()){
				lonelyPoints.push_back(&allPoints[i]);
			}
		}
		
		// Si le nombre de lignes est atteint ou si le nombre 
		// d'iteration max a ete depasse, on stop l'algorithme
		searchingForLines = lines.size() != NB_LINES && nb_iterations < NB_MAX_ITERATION;
	}
}

std::list<LinePoint*>::iterator DetectLine::selectRandomPoint(std::list<LinePoint*> pts) {
	std::list<LinePoint*>::iterator begin = pts.begin();
    std::uniform_int_distribution<> dis(0, std::distance(pts.begin(), pts.end()) - 1);
    std::advance(begin, dis(gen));
    return begin;
}
