#pragma once

// Gestion du temps
#include <chrono>
#include <vector>
#include <string>
#include <stdexcept>

#include <iostream>
#include <iomanip>

#include<opencv2/highgui/highgui.hpp>
#include<opencv2/imgproc/imgproc.hpp>

#include "display.hpp"

using namespace cv;

class Window{
	private:
		const std::string PATH_RES;
		
		unsigned fps;
		int msToWaitOpt;	// Nombre de seconde a attendre dans le meilleur des cas
		bool imageMode;
		bool showFps;
		
		//std::vector<Mat> imgs;		// Liste des images chargees
		VideoCapture _imgs;
		std::vector<Display> displays;
		
		VideoWriter outputVideo;
		
	public:
		Window(std::string pPathRes, unsigned pFps = 30, bool pImageMode = false, bool showFps = false);

		Mat catImages(std::vector<Mat> imgs);
		void chargementImgs();
		void enregistrementImgs(std::string nameFile);
		void renderAll();
		
		unsigned getNbValidPicture(unsigned nbCompToFind, Etiquetage* filtre_etiquetage_courant);
		
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
