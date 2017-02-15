/*!
 * \file main.cpp
 * \brief Fichier main du programme
 * \author Chevalier_Pierre
 * \version 0.1
 */
 
#include "window.hpp"
#include "display.hpp"

// filtres
#include "bthreshold.hpp"
#include "etiquetage.hpp"
#include "detectLine.hpp"
#include "grayscale.hpp"
#include "axe.hpp"

int main(int argc, const char* argv[] )
{
	std::string path_video;
	std::vector<Axe> axes;
	
	if(argc >= 2){
		path_video = std::string(argv[1]);
	} else {
		//path_video = "./rsc/artificial_images/apparaillement_test_";
		path_video = "./rsc/Scenario_4_fluxFPGA/in_";
	}
	
	// Chargement du modele
	axes = Axe::getAxes("./rsc/model1.xml");
	if(axes.empty()){
		std::cerr << "Le modele n'a pas ete charge" << std::endl;
		return 1;
	}
	
	//Chargements images
	Window window1(path_video, 20, true);
	
	// Creation des sorties
	Display source;
	Display etiquetage;
	Display line;
	
	Bthreshold* filtreBthreshold = new Bthreshold(200);
	Etiquetage* filtreEtiquetage = new Etiquetage();
	DetectLine* filtreDetectLine = new DetectLine(axes, filtreEtiquetage->getComps(),3,true);
	
	// Parametrage de la sortie filtree
	etiquetage.addFiltre(new Grayscale());
	etiquetage.addFiltre(filtreBthreshold);
	etiquetage.addFiltre(filtreEtiquetage);

	line.addFiltre(new Grayscale());
	line.addFiltre(filtreBthreshold);
	line.addFiltre(filtreEtiquetage);
	line.addFiltre(filtreDetectLine);

	// Ajout des rendus a la fenetre
	window1.addDisplay(source);
	window1.addDisplay(etiquetage);
	window1.addDisplay(line);
	
	// On affiche le traitement
	window1.renderAll();
	
    return 0;
}
