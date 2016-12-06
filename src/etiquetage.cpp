#include "etiquetage.hpp"
#include <iostream>

DetectedBlob::DetectedBlob(Composante& pcomp, bool pestimated)
: comp(pcomp), estimated(pestimated), note(0)
 {}

void DetectedBlob::setComp(Composante& pcomp){
	comp = pcomp;
}

Composante& DetectedBlob::getComp(){
	return comp;
}

void DetectedBlob::setEstimated(bool pestimated){
	estimated = pestimated;
}

const bool& DetectedBlob::isEstimated(){
	return estimated;
}

void DetectedBlob::setNote(unsigned pnote){
	note = pnote;
}

const unsigned& DetectedBlob::getNote(){
	return note;
}

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
	Point pos_cur, pos_ord;
	
	unsigned distanceX, distanceY;
	float distance2, distance2min;
	int nbDetectedBlob, moveX, moveY;
	
	bool allBlopEstimated = false;
	
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
    }else{
    	resc = res.clone();
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
				compsOrdonnees.push_back(DetectedBlob(comp,false));
			}
	   	}
	   	
	   	// Dans tous les cas on repasse les booleans des compo ordonnees a false
	   	for(DetectedBlob& dblop : compsOrdonnees){
	   		dblop.setEstimated(false);
	   	}
		
		// On va calculer le mouvement moyen de chaque point
		moveX=0;
		moveY=0;
		
		// On cherche a identifier chaque point pour les remettre a leur place
		// dans compsOrdonnees
		nbDetectedBlob = 0;
		for(Composante comp_cur: comps){
			auto it = compsOrdonnees.begin();
			auto closer = compsOrdonnees.end();
			distance2min = -1;
			
			pos_cur = comp_cur.getPosition();
			
			// On va chercher le point detecter precedement le plus pret
			while(it != compsOrdonnees.end()){
				distanceX = (unsigned) abs(pos_cur.x - pos_ord.x);
				distanceY = (unsigned) abs(pos_cur.y - pos_ord.y);
				if (!it->isEstimated() &&
					distanceX < DEPLACEMENT && 
					distanceY < DEPLACEMENT
				   ){
					distance2 = distanceX*distanceX + distanceY*distanceY;
				
					if(distance2min == -1 || distance2 < distance2min){
						distance2min = distance2;
						closer = it;
					}
				}
				++it;
			}
			
			if(closer != compsOrdonnees.end()){
				pos_ord = closer->getComp().getPosition();
			
				// On calcul la distance parcourue et on l'ajoute a la moyenne
				moveX += (comp_cur.getPosition().x - closer->getComp().getPosition().x);
				moveY += (comp_cur.getPosition().y - closer->getComp().getPosition().y);
			
				// On le remplace
				closer->setComp(comp_cur);
				closer->setEstimated(true);
				
				nbDetectedBlob++;
			}
		}
		
		// Calcul moyenne
		if(nbDetectedBlob!=0){
	   		moveX /= nbDetectedBlob;
	   		moveY /= nbDetectedBlob;
	   	}
		
		// Reste a assigner les points qui n'ont pas ete detectes
		allBlopEstimated = true;
		for(DetectedBlob& dblob : compsOrdonnees){
			// Si l'un des points n'est pas estime,
			// le boolean est passe a false
			allBlopEstimated = allBlopEstimated && !dblob.isEstimated();
			
			// Si le booleen est a estimer, on l'estime
	   		if(!dblob.isEstimated()){
	   			Point& point = dblob.getComp().getPosition();
	   			point.x += moveX;
	   			point.y += moveY;
	   		}
	   	}
		
		// On gere le compteur, et on supprime eventuellement le vecteur
		// de forme s'il n'a plus lieu d'etre
		if(comps.size()==0 || allBlopEstimated){
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
			Composante& comp = compsOrdonnees[i].getComp();
			index_next_comp = (i + 1) % compsOrdonnees.size();
			Composante& nextComp = compsOrdonnees[index_next_comp].getComp();
			
			// On choisit la couleur du point a utilise
			if(compsOrdonnees[i].isEstimated()){
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
