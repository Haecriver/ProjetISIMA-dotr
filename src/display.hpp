#ifndef DISPLAY_HPP
#define DISPLAY_HPP

#include <vector>

#include<opencv2/highgui/highgui.hpp>
#include<opencv2/imgproc/imgproc.hpp>

#include "filtre.hpp"
#include "etiquetage.hpp"

using namespace cv;

class Display{
	private:
		std::vector<Filtre*> filtres;
		
	public:
		virtual ~Display();
	
		Mat render(Mat& src);		
		void addFiltre(Filtre* filtre);
		
		bool isAllCompFound(unsigned nbCompToFind, Etiquetage* filtre_etiquetage_courant, Mat& src);
};

#endif
