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

bool DetectedBlob::isEstimated() const{
	return estimated;
}

void DetectedBlob::setNote(float pnote){
	note = pnote;
}

float DetectedBlob::getNote() const{
	return note;
}

void DetectedBlob::computeNote(){
	if(!estimated){
		note = 10.0;
	}else{
		if (comp.getWidth() < comp.getHeight()){
			note = (float)comp.getWidth() / (float)comp.getHeight();
		} else {
			note = (float)comp.getHeight() / (float)comp.getWidth();
		}
		note *= 100.0;
	}
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

	// Composante connexe
    comps = Composante::getComposantes(res,MAX_NB_COMP);
    
    // Passage en couleur RGB
    // (RGB ET POURTANT BGR ??)
    if(res.channels() == 1){
	    cvtColor(res, resc, CV_GRAY2RGB);
    }else{
    	resc = res.clone();
    }
    res.release();
    
    // Detection des points calcules
    for(Composante comp : comps){
    	circle(resc, comp.getPosition(), 5, Scalar(0,0,255));
    }
    
    // Detection de forme
    if(DETECT_SHAPE){
		detectShape(resc);
    }
    
    return resc;
}

void Etiquetage::detectShape(Mat& resc){
	int moveX, moveY;
	
	bool allBlopEstimated;
	
	// Cas ou on initialise les points de l'objet detecte
	if(compsOrdonnees.size() == 0 && comps.size() == MAX_NB_COMP){
		for (Composante comp: comps){
			compsOrdonnees.push_back(DetectedBlob(comp,false));
		}
   	}else{
   	
	   	// Dans tous les cas il faut repasser les booleans des compo ordonnees a false
	   	for(DetectedBlob& dblop : compsOrdonnees){
	   		dblop.setEstimated(false);
	   	}
   	}
	
	assignDetectedCompToBlob(moveX, moveY);
	//assignDetectedCompToBlob2(moveX, moveY);
	
	allBlopEstimated = estimateRemainingBlobs(moveX, moveY);
	
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
	
	computeNote();
	
	drawShape(resc);
}

void Etiquetage::assignDetectedCompToBlob(int& meanMoveX_OUT, int& meanMoveY_OUT){
	Point pos_cur, pos_ord;
	
	unsigned distanceX, distanceY;
	float distance2, distance2min;
	unsigned nbDetectedBlob = 0;
	
	// On va calculer le mouvement moyen de chaque point
	meanMoveX_OUT=0;
	meanMoveY_OUT=0;
	
	// On cherche a identifier chaque point pour les remettre a leur place
	// dans compsOrdonnees
	
	// Pour chaque composantes detectees
	for(Composante comp_cur: comps){
		auto it = compsOrdonnees.begin();
		auto closer = compsOrdonnees.end();
		distance2min = -1;
		
		pos_cur = comp_cur.getPosition();
		
		// On va chercher le point detecter precedement le plus pret
		while(it != compsOrdonnees.end()){
			pos_ord = it->getComp().getPosition();
			
			distanceX = (unsigned) abs(pos_cur.x - pos_ord.x);
			distanceY = (unsigned) abs(pos_cur.y - pos_ord.y);
			
			if (!it->isEstimated() /*&&
				distanceX < DEPLACEMENT && 
				distanceY < DEPLACEMENT*/
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
			meanMoveX_OUT += (comp_cur.getPosition().x - closer->getComp().getPosition().x);
			meanMoveY_OUT += (comp_cur.getPosition().y - closer->getComp().getPosition().y);
		
			// On le remplace
			closer->setComp(comp_cur);
			closer->setEstimated(true);
			
			nbDetectedBlob++;
		}
	}
	
	// Calcul moyenne
	if(nbDetectedBlob!=0){
   		meanMoveX_OUT /= nbDetectedBlob;
   		meanMoveY_OUT /= nbDetectedBlob;
   	}
}

void Etiquetage::assignDetectedCompToBlob2(int& meanMoveX_OUT, int& meanMoveY_OUT){
	Point pos_cur, pos_ord;
	
	unsigned distanceX, distanceY;
	float distance2;
	unsigned nbDetectedBlob = 0;
	
	// On va calculer le mouvement moyen de chaque point
	meanMoveX_OUT=0;
	meanMoveY_OUT=0;
	
	std::vector<Composante> comps_copy;
	comps_copy.assign(comps.begin(), comps.end());
	
	float noteMax;
	float note;
	
	std::cout << "-------------" << std::endl;
	
	for(DetectedBlob& comp_ord: compsOrdonnees){
		noteMax = 0.0;
		auto it = comps_copy.begin();
		auto better = comps_copy.end();
		
		while(it != comps_copy.end()){
			static const unsigned SURFACE_MAX = 100;
			static const unsigned DISTANCE_MIN2 = 40000;
			float noteSurface, noteProportion, noteDistance;
			static const float COEF_NOTE_SURFACE = 0.1, COEF_NOTE_PROPORTION = 0.1, COEF_NOTE_DISTANCE = 0.8;
			
			// Calcul de la meilleur note
			// Note surface
			noteSurface = (float)(it->getWidth() * it->getHeight()) / (float)SURFACE_MAX;
			if (noteSurface > 1.0){
				noteSurface = 100.0;
			}else{
				noteSurface *= 100.0;
			}
			
			// Note proportion
			if (it->getWidth() < it->getHeight()){
				noteProportion = (float)it->getWidth() / (float)it->getHeight();
			} else {
				noteProportion = (float)it->getHeight() / (float)it->getWidth();
			}
			noteProportion *= 100.0;
			
			// Note distance
			
			pos_ord = it->getPosition();
			
			distanceX = (unsigned) abs(pos_cur.x - pos_ord.x);
			distanceY = (unsigned) abs(pos_cur.y - pos_ord.y);
			distance2 = distanceX*distanceX + distanceY*distanceY;
			
			noteDistance = DISTANCE_MIN2 / distance2;
			if (noteDistance > 1.0){
				noteDistance = 100.0;
			}else{
				noteDistance *= 100.0;
			}
			
			note = noteSurface * COEF_NOTE_SURFACE + noteProportion * COEF_NOTE_PROPORTION + noteDistance * COEF_NOTE_DISTANCE;
			

			if(note > noteMax && note > 10.0){
				noteMax = note;
				better = it;
			}
			
			++it;
		}
		
		if(better!=comps_copy.end()){
			std::cout << "Note : " << note << std::endl;
			// On calcul la distance parcourue et on l'ajoute a la moyenne
			meanMoveX_OUT += (better->getPosition().x - comp_ord.getComp().getPosition().x);
			meanMoveY_OUT += (better->getPosition().y - comp_ord.getComp().getPosition().y);
		
			// On le remplace
			comp_ord.setComp(*better);
			comp_ord.setEstimated(true);
			comp_ord.setNote(noteMax);
			
			nbDetectedBlob++;
			comps_copy.erase(better);
		}
	}
	
	// Calcul moyenne
	if(nbDetectedBlob!=0){
   		meanMoveX_OUT /= nbDetectedBlob;
   		meanMoveY_OUT /= nbDetectedBlob;
   	}
}

bool Etiquetage::estimateRemainingBlobs(const unsigned moveX, const unsigned moveY){
	// Reste a assigner les points qui n'ont pas ete detectes
	bool allBlopEstimated = true;
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
   	return allBlopEstimated;
}

void Etiquetage::drawShape(Mat& resc){
	int index_next_comp;
	
	// Preparation affichage text
	Scalar color;
	int fontFace = CV_FONT_HERSHEY_COMPLEX_SMALL;
	double fontScale = 1;
	int thickness = 1;
	int baseline = 0;
	baseline += thickness;
	
	// On affiche compsOrdonnees avec ses points
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
		
		std::string toPrint = std::to_string(i);
		if(false){
			toPrint += "(" + std::to_string(compsOrdonnees[i].getNote()) + ")";
		}
		// On affiche le num de la comp
		putText(resc, toPrint, comp.getPosition(), fontFace, fontScale,
	    	color, thickness);
	    	
		// On lie les composantes par une ligne
		line(resc, comp.getPosition(), nextComp.getPosition(), Scalar(0, 255, 0));
	}
	
	// Affichage de la note
	putText(resc, "Note : " + std::to_string(note), Point(10,20), fontFace, fontScale,
	    	Scalar(255, 0, 0), thickness);
}

void Etiquetage::computeNote(){
	note = 0.0;
	float coef_notes_blobs = 1.0;
	for(DetectedBlob& dblob : compsOrdonnees){
		//dblob.computeNote();
		note += dblob.getNote();
	}
	note /= compsOrdonnees.size();
	note *= coef_notes_blobs;
}
