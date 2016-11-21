#include "etiquetage.hpp"
#include <iostream>
Etiquetage::Etiquetage(unsigned pMaxNbComp):
MAX_NB_COMP(pMaxNbComp)
{}

Mat Etiquetage::render(Mat& img){
	std::vector<Composante> comps;

	Mat res(img.clone());
	
	// Composante connexe
    comps = Composante::getComposantes(res,MAX_NB_COMP);
    
    std::cout << "comps size: " << comps.size() << std::endl;
    
    // Passage en couleur RGB
    // (RGB ET POURTANT BGR ??)
    cvtColor(res, res, CV_GRAY2RGB);
    
    // Detection des points calcules
    for(Composante comp : comps){
    	circle(res, comp.getPosition(), 5, Scalar(0,0,255));
    }
    
    /*oldComps.clear();
    oldComps = comps;*/
    
    return res;
}
