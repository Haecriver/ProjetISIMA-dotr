#include "detectShape.hpp"
#include <iostream>

DetectShape::DetectShape(const std::vector<Composante>& pcomps, unsigned pNbComps):
NB_COMPS(pNbComps),
comps(pcomps)
{}

Mat DetectShape::render(Mat& img){
	Mat res(img.clone());
	int moveX, moveY;
	
	bool allBlopEstimated;
	
	// Cas ou on initialise les points de l'objet detecte
	if(compsOrdonnees.size() == 0 && comps.size() == NB_COMPS){
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
	
	allBlopEstimated = estimateRemainingBlobs(moveX, moveY);
	
	// On gere le compteur, et on supprime eventuellement le vecteur
	// de forme s'il n'a plus lieu d'etre
	if(comps.size()==0 || allBlopEstimated){
		_cpt++;
	}else{
		_cpt = 0;
	}
	
	if(_cpt >= ITE_MAX){
		_cpt = 0;
		compsOrdonnees.clear();
	}
	
	computeNote();
	
	drawShape(res);
	
	return res;
}

void DetectShape::assignDetectedCompToBlob(int& meanMoveX_OUT, int& meanMoveY_OUT){
	Point pos_cur, pos_ord;
	
	unsigned distanceX, distanceY;
	float distance2, distance2min;
	float nbDetectedBlob = 0;
	float noteSurface, noteProportion, noteDistance;
	
	// On va calculer le mouvement moyen de chaque point
	meanMoveX_OUT=0;
	meanMoveY_OUT=0;
	
	std::vector<Composante> comps_copy;
	comps_copy.assign(comps.begin(), comps.end());
	
	float noteMax;
	float note;
	
	float surfaceMax = computeSurfaceMax();
	
	for(DetectedBlob& comp_ord: compsOrdonnees){
		// Init variables
		noteMax = 0.0;
		auto it = comps_copy.begin();
		auto better = comps_copy.end();
		pos_ord = comp_ord.getComp().getPosition();
		
		distance2min = computeDistanceMin(comp_ord,comps_copy);
		
		// On va maintenant parcourir les composantes detectees
		// Et associee la meilleure au blob courant (en fonction
		// de la note calculee)
		while(it != comps_copy.end()){
			
			// Calcul de la meilleur note
			// Note surface
			noteSurface = (float)(it->getPoints().size()) / surfaceMax;
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
			pos_cur = it->getPosition();
			
			distanceX = (unsigned) abs(pos_cur.x - pos_ord.x);
			distanceY = (unsigned) abs(pos_cur.y - pos_ord.y);
			distance2 = distanceX*distanceX + distanceY*distanceY;
			
			noteDistance = distance2min / distance2;
			if (noteDistance > 1.0){
				noteDistance = 100.0;
			}else{
				noteDistance *= 100.0;
			}

			note = DetectedBlob::computeNote(noteSurface, noteProportion, noteDistance);

			if(note > noteMax && note > 10.0){
				noteMax = note;
				better = it;
			}
			
			++it;
		}
		
		if(better!=comps_copy.end()){
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

bool DetectShape::estimateRemainingBlobs(const unsigned moveX, const unsigned moveY){
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
   			
   			dblob.setNote(dblob.getNote()*0.8);
   		}
   	}
   	return allBlopEstimated;
}

void DetectShape::drawShape(Mat& resc){
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

void DetectShape::computeNote(){
	note = 0.0;
	float coef_notes_blobs = 1.0;
	for(DetectedBlob& dblob : compsOrdonnees){
		//dblob.computeNote();
		note += dblob.getNote();
	}
	note /= compsOrdonnees.size();
	note *= coef_notes_blobs;
}

float DetectShape::computeDistanceMin(const DetectedBlob& comp_ord, const std::vector<Composante>& comps_copy){
	Point pos_cur, pos_ord;
	unsigned distanceX, distanceY;
	float distance2;
	float distance2min = -1;
	
	// On va chercher la composante la plus pret de ce blob
	for(const Composante& comp_cpy_cur : comps_copy){
		pos_ord = comp_ord.getComp().getPosition();
		pos_cur = comp_cpy_cur.getPosition();
	
		distanceX = (unsigned) abs(pos_cur.x - pos_ord.x);
		distanceY = (unsigned) abs(pos_cur.y - pos_ord.y);
		distance2 = distanceX*distanceX + distanceY*distanceY;

		if(distance2min == -1 || distance2 < distance2min){
			distance2min = distance2;
		}
	}
	return distance2min;
}

float DetectShape::computeSurfaceMax(){
	float surfaceMax = 0;
	for(const Composante& comp : comps){
		if(surfaceMax < comp.getPoints().size()){
			surfaceMax = (float)comp.getPoints().size();
		}
	}
	return surfaceMax;
}
