/*!
 * \file main.cpp
 * \brief Fichier main du programme
 * \author Chevalier_Pierre
 * \version 0.1
 */
 
#include "window.hpp"
#include "display.hpp"

// filtres
#include "grayscale.hpp"
#include "bthreshold.hpp"
#include "erosion.hpp"
#include "dilatation.hpp"
#include "etiquetage.hpp"
#include "adathreshold.hpp"
#include "detectLine.hpp"
#include "axe.hpp"

int main(int argc, const char* argv[] )
{
	// std::cerr << getBuildInformation() << std::endl;
	bool record_capture;
	std::string path_video;
	std::vector<Axe> axes;
	
	if(argc >= 2){
		path_video = std::string(argv[1]);
	} else {
		path_video = "./rsc/Scenario_3_fluxFPGA/in_";
	}
	
	// Chargement du modele
	axes = Axe::getAxes("./rsc/model1.xml");
	if(axes.empty()){
		std::cerr << "Le modele n'a pas ete charge" << std::endl;
		return 1;
	}
	
	//Chargements images
	Window window(path_video, 20);
	
	// Creation des sorties
	Display source;
	Display filtered;
	
	Etiquetage* filtreEtiquetage = new Etiquetage();
	
	// Parametrage de la sortie filtree
	filtered.addFiltre(new Grayscale());
	filtered.addFiltre(new Bthreshold(240));
	filtered.addFiltre(filtreEtiquetage);
	filtered.addFiltre(new DetectLine(axes, filtreEtiquetage->getComps(),4));
	
	// Ajout des rendus a la fenetre
	window.addDisplay(source);
	window.addDisplay(filtered);
	
	// Verif des args du main
	record_capture = argc==2;
	
	// Dans le cas d'un enregistrement video, creation du fichier
	if(record_capture){
		//Enregistrement video
		window.enregistrementImgs(std::string(argv[1]));
	}
	
	// On affiche le traitement
	window.renderAll();
	
    return 0;
}
