#include "etiquetage.hpp"
#include <iostream>

// Constructeur
Etiquetage::Etiquetage()
{}

// Setter/Getter
const std::vector<Composante>& Etiquetage::getComps() const{
	return comps;
}

// Methodes
Mat Etiquetage::render(Mat& img){
	
	Mat res(img.clone()), resc;

	// Composante connexe
    comps = Composante::getComposantes(res);
    
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

    return resc;
}
