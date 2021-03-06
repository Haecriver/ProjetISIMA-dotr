#include"display.hpp"

Display::~Display(){
	for (std::vector<Filtre*>::iterator it = filtres.begin(); it != filtres.end(); ++it)
    {
       // delete *it;
    }
}

Mat Display::render(Mat& src){
	Mat img(src.clone());
	Mat temp;
	for(Filtre* filtre: filtres){
		temp = filtre->render(img);
		img.release();
		img = temp;
	}
	return img;
}

void Display::addFiltre(Filtre* filtre){
	filtres.push_back(filtre);
}

bool Display::isAllCompFound(unsigned nbCompToFind, Etiquetage* filtre_etiquetage_courant, Mat& src){
	Mat img(src.clone());
	for(Filtre* filtre: filtres){
		img = filtre->render(img);
	}
	return filtre_etiquetage_courant->getComps().size() == nbCompToFind;
}
