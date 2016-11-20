#ifndef WINDOW_HPP
#define WINDOW_HPP

// Gestion du temps
#include <chrono>
#include <vector>
#include <string>

#include <iostream>
#include <iomanip>

#include<opencv2/highgui/highgui.hpp>
#include<opencv2/imgproc/imgproc.hpp>

#include "display.hpp"

using namespace cv;

class Window{
	private:
		const std::string PATH_RES;
		const unsigned NB_IMG;
		
		unsigned fps;
		int msToWaitOpt = 1000/fps;	// Nombre de seconde a attendre dans le meilleur des cas
		
		std::vector<Mat> imgs;		// Liste des images chargees
		std::vector<Display> displays;
		
		VideoWriter outputVideo;
		
	public:
		Window(std::string pPathRes, unsigned pNbImg, unsigned pFps = 30);

		Mat catImages(std::vector<Mat> imgs);
		void chargementImgs();
		void enregistrementImgs(std::string nameFile);
		void renderAll();
		
		void addDisplay(Display display);
};

/*!
 *  \brief Renvoie une string representant l'entier i avec un certain nombre de digits
 *
 *  \param i : Entier a representer.
 *  \param lenght : Nombre de digits
 *  \return string representant l'entier
 */
std::string setDigit(const int i, const int length);

#endif
