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

int main(int argc, const char* argv[] )
{
	// std::cerr << getBuildInformation() << std::endl;
	bool record_capture;
	std::string path_video;
	
	if(argc >= 2){
		path_video = std::string(argv[1]);
	} else {
		path_video = "./rsc/artificial_images/apparaillement_test_1.jpg";
	}
	
	//Chargements images
	Window window(path_video, 20, true);
	
	// Creation des sorties
	Display filtered;
	
	Etiquetage* filtreEtiquetage = new Etiquetage();
	
	// Parametrage de la sortie filtree
	filtered.addFiltre(new Bthreshold(240)); // 148 res optimal
	filtered.addFiltre(filtreEtiquetage);
	filtered.addFiltre(new DetectLine(filtreEtiquetage->getComps()));
	
	// Ajout des rendus a la fenetre
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
