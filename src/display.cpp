#include"display.hpp"
#include <iostream>

Display::~Display(){
	for (std::vector<Filtre*>::iterator it = filtres.begin(); it != filtres.end(); ++it)
    {
       // delete *it;
    }
}

Mat Display::render(Mat& src){
	Mat img(src.clone());
	int i =0;
	for(Filtre* filtre: filtres){
		std::cout << "filtre" << i++ << std::endl;
		img = filtre->render(img);
	}
	return img;
}

void Display::addFiltre(Filtre* filtre){
	filtres.push_back(filtre);
}
