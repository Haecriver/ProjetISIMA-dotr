#include "etiquetage.hpp"
#include <iostream>

// Constructeur
Etiquetage::Etiquetage(unsigned pMaxNbComp):
MAX_NB_COMP(pMaxNbComp)
{}

// Setter/Getter
const std::vector<Composante>& Etiquetage::getComps() const{
	return comps;
}

const std::vector<Composante>& Etiquetage::getOldComps() const{
	return oldComps;
}

// Methodes
Mat Etiquetage::render(Mat& img){
	Mat res(img.clone());
	
	// Composante connexe
    comps = Composante::getComposantes(res,MAX_NB_COMP);
    
    // Passage en couleur RGB
    // (RGB ET POURTANT BGR ??)
    cvtColor(res, res, CV_GRAY2RGB);
    
    // Detection des points calcules
    for(Composante comp : comps){
    	circle(res, comp.getPosition(), 5, Scalar(0,0,255));
    }
    
    oldComps.clear();
    oldComps = comps;
    
    return res;
}
