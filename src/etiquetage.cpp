#include "etiquetage.hpp"
#include <iostream>


// Constructeur
Etiquetage::Etiquetage(unsigned pMaxNbComp, bool pDetectShape):
MAX_NB_COMP(pMaxNbComp),
DETECT_SHAPE(pDetectShape)
{}

// Setter/Getter
const std::vector<Composante>& Etiquetage::getComps() const{
	return comps;
}

// Methodes
Mat Etiquetage::render(Mat& img){
	Mat res(img.clone()), resc;
	bool fin;
	Point pos_cur, pos_ord;
	int moveX, moveY;
	
	// Preparation affichage text
	int fontFace = CV_FONT_HERSHEY_COMPLEX_SMALL;
	double fontScale = 1;
	int thickness = 1;
	int baseline = 0;
	/*Size textSize = getTextSize(text, fontFace,
			                    fontScale, thickness, &baseline);*/
	baseline += thickness;
	
	// Composante connexe
    comps = Composante::getComposantes(res,MAX_NB_COMP);
    
    // Passage en couleur RGB
    // (RGB ET POURTANT BGR ??)
    if(res.channels() == 1){
	    cvtColor(res, resc, CV_GRAY2RGB);
    }
    
    // Detection des points calcules
    for(Composante comp : comps){
    	circle(resc, comp.getPosition(), 5, Scalar(0,0,255));
    }
    
    // Detection de forme
    if(DETECT_SHAPE){
    
		// Cas ou on initialise les points de l'objet detecte
		if(compsOrdonnees.size() == 0 && comps.size() == MAX_NB_COMP){
			for (Composante comp: comps){
				compsOrdonnees.push_back(std::pair<Composante, bool>(comp,false));
			}
	   	}
	   	
	   	// Dans tous les cas on repasse les booleans des compo ordonnees a false
	   	for(std::pair<Composante, bool>& pair : compsOrdonnees){
	   		pair.second = false;
	   	}
		
		// On va calculer le mouvement moyen de chaque point
		moveX=0;
		moveY=0;
		
		// On cherche a identifier chaque point pour les remettre a leur place
		// dans compsOrdonnees
		for(Composante comp_cur: comps){
			fin = false;
			auto it = compsOrdonnees.begin();
			pos_cur = comp_cur.getPosition();
			
			while(!fin && it != compsOrdonnees.end()){
				// Si l'iterator courant est elligible et n'a pas deja ete choisi
				pos_ord = it->first.getPosition();
				if(!it->second &&
					(unsigned) abs(pos_cur.x - pos_ord.x) < DEPLACEMENT && 
					(unsigned) abs(pos_cur.y - pos_ord.y) < DEPLACEMENT ){
					
					// On calcul la distance parcourue et on l'ajoute a la moyenne
					moveX += (comp_cur.getPosition().x - it->first.getPosition().x);
	   				moveY += (comp_cur.getPosition().y - it->first.getPosition().y);
					
					// On le remplace
					it->first = comp_cur;
					it->second = true;
					fin = true;
				}
				++it;
			}
		}
		
		// Calcul moyenne
		if(comps.size()!=0){
	   		moveX /= (int)comps.size();
	   		moveY /= (int)comps.size();
	   	}
		
		// Reste a assigner les points qui n'ont pas ete detectes
		for(std::pair<Composante, bool>& pair : compsOrdonnees){
	   		if(!pair.second){
	   			Point& point = pair.first.getPosition();
	   			point.x += moveX;
	   			point.y += moveY;
	   		}
	   	}
		
		// On gere le compteur, et on supprime eventuellement le vecteur
		// de forme s'il n'a plus lieu d'etre
		if(comps.size()==0){
			_cpt++;
		}else{
			_cpt=0;
		}
		
		if(_cpt >= ITE_MAX){
			_cpt = 0;
			compsOrdonnees.clear();
		}
		
		
		// On affiche compsOrdonnees avec ses points
		int index_next_comp;
		Scalar color;
		for(unsigned i =0; i<compsOrdonnees.size(); i++){
			// On initialise les ref de composantes
			Composante& comp = compsOrdonnees[i].first;
			index_next_comp = (i + 1) % compsOrdonnees.size();
			Composante& nextComp = compsOrdonnees[index_next_comp].first;
			
			// On choisit la couleur du point a utilise
			if(compsOrdonnees[i].second){
				color = Scalar(0,255,0); // vert => point reel
			} else {
				color = Scalar(255,0,0); // rouge => point estime
			}

			// On affiche le num de la comp
			putText(resc, std::to_string(i), comp.getPosition(), fontFace, fontScale,
		    	color, thickness);
		    	
			// On lie les composantes par une ligne
			line(resc, comp.getPosition(), nextComp.getPosition(), Scalar(0, 255, 0));
		}
    }
    res.release();
    return resc;
}
