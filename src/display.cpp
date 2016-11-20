#include"display.hpp"

Display::~Display(){
	for (std::vector<Filtre*>::iterator it = filtres.begin(); it != filtres.end(); ++it)
    {
       // delete *it;
    }
}

Mat Display::render(Mat& src){
	Mat img(src.clone());
	for(Filtre* filtre: filtres){
		img = filtre->render(img);
	}
	return img;
}

void Display::addFiltre(Filtre* filtre){
	filtres.push_back(filtre);
}
