#include "detectLine.hpp"

std::random_device rd;
std::mt19937 gen(rd());

// Contructeurs
DetectLine::DetectLine(const std::vector<Axe>& pAxes, const std::vector<Composante>& pcomps,
	unsigned pNbLines, bool pDisplaySearching):
axes(pAxes),
comps(pcomps),
NB_LINES(pNbLines),
DISPLAY_SEARCHING(pDisplaySearching)
{}

// Methodes
Mat DetectLine::render(Mat& img){
	Mat res(img.clone());
	
	Scalar color;
	int fontFace = CV_FONT_HERSHEY_COMPLEX_SMALL;
	double fontScale = 1;
	int thickness = 1;
	string str;

	
	// On calcul les lignes depuis le vecteur de composants
	// donne au constructeur
	getLinesFromPoints(res);
	
	// On affiche les lignes graphiquement
	for(Line el_line: lines){

		if(el_line.sameCrossRatio(axes[0].crossRatio)){
			// X
			str = "Axe X";
			color = Scalar(255, 0, 0);
		}else if (el_line.sameCrossRatio(axes[1].crossRatio)){
			// Y
			str = "Axe Y";
			color = Scalar(0, 255, 0);
		}else if (el_line.sameCrossRatio(axes[2].crossRatio)){
			// Z
			str = "Axe Z";
			color = Scalar(0, 0, 255);
		} else {
			str = "ERREUR";
			color = Scalar(255, 0, 255);
		}
		
		
		line(res, el_line.getPts()[0].getPos(), 
			el_line.getPts()[3].getPos(),
			color, 3);
		putText(res, str, el_line.getPts()[0].getPos(), fontFace, fontScale,
			color, thickness);
	}
	
	return res;
}

void DetectLine::getLinesFromPoints(Mat& img){
	bool searchingForLines = true, searchingForPoints = true;
	Mat display, display_cur;
	unsigned nb_iterations = 0;
	
	std::vector<LinePoint> allPoints;				// Stockage de tous les points detectes
	std::vector<LinePoint*> lonelyPoints;				// References temporaire des points detectes 
													// (dont on ne connait pas encore les lignes)
	std::vector<LinePoint*> wasntPivotPoints;			// References temporaire des points detectes
													// (que l'on a pas encore utiliser comme pivot)
	
	// Nettoyage des vecteurs
	lines.clear();

	if(DISPLAY_SEARCHING){
		display = img.clone();
	}
	
	// On initialise les vecteurs de points qui vont etre utilises
	// a partir du vecteur de composant passe au constructeur
	allPoints = LinePoint::convertCompToLinePoint(comps);
	for(unsigned i = 0; i < allPoints.size(); i++){
		lonelyPoints.push_back(&allPoints[i]);
		wasntPivotPoints.push_back(&allPoints[i]);
	}
	
	// Tant que toutes les lignes n'ont pas ete trouvees
	// Ou que le nombre d'iterations de l'algo a depasse le nb max
	while(searchingForLines){
		// On tire un point a partir duquel on cherche les lignes
		LinePoint* pivot = *selectRandomPoint(wasntPivotPoints);
		pivot->setWasPivot(true);	// On marque le point comme un pivot
		
		// On supprime ce point des tirages possibles
		wasntPivotPoints.erase(std::remove(wasntPivotPoints.begin(), wasntPivotPoints.end(), pivot));
		
		searchingForPoints = true;
		
		while(searchingForPoints){
			// On tire un point pour tirer une ligne
			LinePoint* cur = *selectRandomPoint(lonelyPoints);
			
			// On supprime ce point des tirages possibles
			lonelyPoints.erase(std::remove(lonelyPoints.begin(), lonelyPoints.end(), cur));
			
			// On tire la ligne
			Line lineCur(pivot->getPos(), cur->getPos());
			
			// On l'affiche si demande
			if(DISPLAY_SEARCHING){
				display_cur = display.clone();
				
				line(display_cur, pivot->getPos(), 
					cur->getPos(),
					Scalar(0, 255, 0), 1);
				
				imshow("Searching lines",display_cur);
				waitKey(500);
				display_cur.release();
			}
			
			// On test si la droite creee, passe par 4 points
			if(lineCur.getIncludedPointsPolar(allPoints)){
			
				// On verifie le cross ratio de la ligne
				bool crValid = false;
				
				for(Axe axe : axes){
					crValid = crValid 
						|| lineCur.sameCrossRatio(axe.crossRatio);
				}
				
				
				if(crValid){
					// Si oui
					// On stock la ligne
					lines.push_back(lineCur);
				
					// On reinitialise le nombre d'iteration
					nb_iterations = 0;
				
					// On stop la boucle
					searchingForPoints = false;
				
					// On l'affiche si demande
					if(DISPLAY_SEARCHING){
						line(display, lineCur.getPts()[0].getPos(), 
							lineCur.getPts()[3].getPos(),
							Scalar(255, 255, 0), 1);
				
						imshow("Searching lines",display);
					}
				}
			}
			
			// Si on a teste tous les points disponibles
			if(lonelyPoints.empty()){	
				// On increment le nombre d'iteration d'erreurs
				nb_iterations++;
				
				// On stop la boucle
				searchingForPoints = false;
			}
		}
		
		// On nettoie le vecteur de point a stocker dans des lignes
		lonelyPoints.erase(lonelyPoints.begin(), lonelyPoints.end());
		wasntPivotPoints.erase(wasntPivotPoints.begin(), wasntPivotPoints.end());
		
		// On met a jout le vecteur des points en n'incluant les points
		// deja compris dans les lignes et le vecteur des points qui n'ont
		// pas ete pivot
		for(unsigned i = 0; i < allPoints.size(); i++){
			if(!allPoints[i].isBelongsToLine()){
				lonelyPoints.push_back(&allPoints[i]);
				if(!allPoints[i].getWasPivot()){
					wasntPivotPoints.push_back(&allPoints[i]);
				}
			}
		}
		
		// Si le nombre de lignes est atteint ou si le nombre 
		// d'iteration max a ete depasse, on stop l'algorithme
		searchingForLines = !wasntPivotPoints.empty() && lines.size() != NB_LINES && nb_iterations < NB_MAX_ITERATION;
		if (nb_iterations >= NB_MAX_ITERATION) std::cout << "Itemax atteint" << std::endl;
		if (wasntPivotPoints.empty()) std::cout << "Tous points parcourus" << std::endl;
	}
}

std::vector<LinePoint*>::iterator DetectLine::selectRandomPoint(std::vector<LinePoint*> pts) {
	std::vector<LinePoint*>::iterator begin = pts.begin();
    std::uniform_int_distribution<> dis(0, std::distance(pts.begin(), pts.end()) - 1);
    std::advance(begin, dis(gen));
    return begin;
}
