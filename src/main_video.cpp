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

int main(int argc, const char* argv[] )
{
	// std::cerr << getBuildInformation() << std::endl;
	bool record_capture;
	
	//Chargements images
	Window window("./rsc/Scenario_1_1/in1_", 517);
	
	// Creation des sorties
	Display source;
	Display filtered;
	
	// Parametrage de la sortie filtree
	filtered.addFiltre(new Grayscale());
	//filtered.addFiltre(new Adathreshold(0.1));
	filtered.addFiltre(new Bthreshold(148)); // 148 res optimal
	filtered.addFiltre(new Erosion(1,1));
	filtered.addFiltre(new Dilatation(1,2));
	filtered.addFiltre(new Etiquetage(5,true));
	
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
