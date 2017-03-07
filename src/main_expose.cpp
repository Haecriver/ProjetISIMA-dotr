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
#include "detectLine.hpp"
#include "axe.hpp"
#include "poseEstimation.hpp"

int main(int argc, const char* argv[] )
{
	std::string path_video, path_camera_matrix="./rsc/calibration_files/res_calib/calibration1.xml";
	std::vector<Axe> axes;
	
	FileStorage fs( path_camera_matrix, FileStorage::READ );
	cv::Mat K;
	
	if(argc != 3){
		std::cout << "Entrez le chemin des images a sauvegarder et un nom" << std::endl
			<<"\tExemple : $> " << std::string(argv[0]) << "./rsc/Scenario_2/in_ scenario2" << std::endl;
	}else{
		fs["Camera_Matrix"] >> K;
		fs.release();
		K.convertTo(K, CV_64F);
	
		path_video = argv[1];
	
		// Chargement du modele
		// Dans le cas du modele en aluminium, il faut utiliser
		// le modele "./rsc/model2.xml"
		axes = Axe::getAxes("./rsc/model1.xml");
		if(axes.empty()){
			std::cerr << "Le modele n'a pas ete charge" << std::endl;
			return 1;
		}
	
		//Chargements images
		Window window(path_video, 100, true);
	
		// Creation des sorties
		Display src;
		Display thre;
		Display eros;
		Display dila;
		Display etiq;
		Display line;
		Display pose;
	
		Grayscale* filtreGrayscale = new Grayscale();
		Bthreshold* filtreBthreshold = new Bthreshold(240);
		Erosion* filtreErosion = new Erosion(1,1);
		Dilatation* filtreDilatation = new Dilatation(1,1);
		Etiquetage* filtreEtiquetage = new Etiquetage();
		DetectLine* filtreDetectLine = new DetectLine(axes, filtreEtiquetage->getComps(),3);
		PoseEstimation* filtrePoseEstimation = new PoseEstimation(K,axes,filtreDetectLine->getLines());
	
		// Parametrage de la sortie filtree
		src.addFiltre(filtreGrayscale);
	
		thre.addFiltre(filtreGrayscale);
		thre.addFiltre(filtreBthreshold);
	
		eros.addFiltre(filtreGrayscale);
		eros.addFiltre(filtreBthreshold);
		eros.addFiltre(filtreErosion);
	
		dila.addFiltre(filtreGrayscale);
		dila.addFiltre(filtreBthreshold);
		dila.addFiltre(filtreErosion);
		dila.addFiltre(filtreDilatation);
	
		etiq.addFiltre(filtreGrayscale);
		etiq.addFiltre(filtreBthreshold);
		etiq.addFiltre(filtreErosion);
		etiq.addFiltre(filtreDilatation);
		etiq.addFiltre(filtreEtiquetage);
	
		line.addFiltre(filtreGrayscale);
		line.addFiltre(filtreBthreshold);
		line.addFiltre(filtreErosion);
		line.addFiltre(filtreDilatation);
		line.addFiltre(filtreEtiquetage);
		line.addFiltre(filtreDetectLine);
	
		pose.addFiltre(filtreGrayscale);
		pose.addFiltre(filtreBthreshold);
		pose.addFiltre(filtreErosion);
		pose.addFiltre(filtreDilatation);
		pose.addFiltre(filtreEtiquetage);
		pose.addFiltre(filtreDetectLine);
		pose.addFiltre(filtrePoseEstimation);
	
		window.addDisplay(src);
		window.addDisplay(thre);
		window.addDisplay(eros);
		window.addDisplay(dila);
		window.addDisplay(etiq);
		window.addDisplay(line);
		window.addDisplay(pose);
	
		// On affiche le traitement
		window.renderAll();
	
		// Sauvegarde d'images
		const std::vector<Mat>& displaysRenders = window.getDisplaysRenders();
		for(unsigned i=0; i < displaysRenders.size(); i++){
			imwrite("./results/images_soutenance/image_" + std::string(argv[2]) + "_" + std::to_string(i) +".png",displaysRenders[i]);
		}
	}
	
    return 0;
}
