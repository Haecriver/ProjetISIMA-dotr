/*!
 * \file main.cpp
 * \brief Fichier main du programme
 * \author Chevalier_Pierre
 * \version 0.1
 */

#include <opencv2/imgproc/imgproc.hpp>

#include <vector>
#include <algorithm>
#include <fstream>
#include <sstream>

#include "window.hpp"
#include "display.hpp"

// filtres
#include "grayscale.hpp"
#include "bthreshold.hpp"
#include "erosion.hpp"
#include "dilatation.hpp"
#include "etiquetage.hpp"

struct statResult{
	unsigned nbCompFound;
	
	unsigned nbEro;
	unsigned sizeEro;
	unsigned nbDil;
	unsigned sizeDil;
	unsigned thres;
	
	// NbComp ; thresh ; nbEro ; SizeEro ; nbDil ; SizeDil
	std::string toCSV(){
		std::stringstream ss;
		ss	<< nbCompFound << "\t" << thres << "\t"
			<< nbEro << "\t" << sizeEro << "\t"
			<< nbDil << "\t" << sizeDil;
		return ss.str();
	}
};

std::ostream& operator<<(std::ostream& os, const statResult& res){
	os	<< "Nb comp trouves  :\t" << res.nbCompFound << std::endl
		<< "Limite threshold :\t" << res.thres << std::endl
		<< "Nb d'erosions    :\t" << res.nbEro << std::endl
		<< "Size d'erosions  :\t" << res.sizeEro << std::endl
		<< "Nb dilatations   :\t" << res.nbDil << std::endl
		<< "Size dilatations :\t" << res.sizeDil << std::endl;
	return os;
}

// Sort descending
bool sortRes(statResult i, statResult j){ 
	return i.nbCompFound > j.nbCompFound; 
}

int main(int argc, const char* argv[] )
{
	std::vector<statResult> results;
	std::fstream file("res/res.csv", std::fstream::in | std::fstream::out | std::fstream::trunc);

	unsigned MIN_NB_ERO = 1, MAX_NB_ERO = 3;
	unsigned MIN_SIZE_ERO = 1, MAX_SIZE_ERO = 3;
	
	unsigned MIN_NB_DIL = 1, MAX_NB_DIL = 3;
	unsigned MIN_SIZE_DIL = 1, MAX_SIZE_DIL = 3;
	
	unsigned MIN_THRES = 100, MAX_THRES = 150;
	
	unsigned nbEro, sizeEro, nbDil, sizeDil, thres, nbCompFound, cpt=0;
	
	unsigned nbIterationTotal = 
		(MAX_NB_ERO - MIN_NB_ERO + 1) *
		(MAX_SIZE_ERO - MIN_SIZE_ERO + 1) *
		(MAX_NB_DIL - MIN_NB_DIL + 1) *
		(MAX_SIZE_DIL - MIN_SIZE_DIL + 1) *
		(MAX_THRES - MIN_THRES + 1);
	
	std::cout << "Nb d'iteration a faire : " << nbIterationTotal << std::endl;
	
	//Chargements images
	Window window("./rsc/Scenario_1_1/in1_", 517);
	
	// Creation des sorties
	Display filtered;
	
	Grayscale* 	grayscale = new Grayscale();
	Bthreshold* threshold = new Bthreshold(MIN_THRES);
	Erosion* 	erosion = new Erosion(MIN_NB_ERO,MIN_SIZE_ERO,MORPH_CROSS);
	Dilatation* dilatation = new Dilatation(MIN_NB_DIL,MIN_SIZE_DIL,MORPH_CROSS);
	Etiquetage* etiquetage = new Etiquetage();
	
	// Parametrage de la sortie filtree
	filtered.addFiltre(grayscale);
	filtered.addFiltre(threshold);
	filtered.addFiltre(erosion);
	filtered.addFiltre(dilatation);
	filtered.addFiltre(etiquetage);
	
	// Ajout des rendus a la fenetre
	window.addDisplay(filtered);
	
	file << "INFO\tValeur Threshold\tNombre d'erosions\tTaille d'erosion\tNombre dilatation\tTaille dilatation" << std::endl;
	file << "MIN\t"<<MIN_THRES<<"\t"<<MIN_NB_ERO<<"\t"<<MIN_SIZE_ERO<<"\t"<<MIN_NB_DIL<<"\t"<<MIN_SIZE_DIL<<std::endl;
	file << "MIN\t"<<MAX_THRES<<"\t"<<MAX_NB_ERO<<"\t"<<MAX_SIZE_ERO<<"\t"<<MAX_NB_DIL<<"\t"<<MAX_SIZE_DIL<<std::endl;
	file << std::endl;
	
	// On fait le traitement statique pour chaque valeur
	file << "Nombre Comp trouve\tValeur Threshold\t" <<
			"Nombre d'erosions\tTaille d'erosion\t" <<
			"Nombre dilatation\tTaille dilatation" << std::endl;
	for(thres = MIN_THRES; thres <= MAX_THRES; ++thres){
		threshold->setThresholdValue(thres);
		
		for(nbEro = MIN_NB_ERO; nbEro <= MAX_NB_ERO; ++nbEro){
			erosion->setNbErosion(nbEro);
			
			for(sizeEro = MIN_SIZE_ERO; sizeEro <= MAX_SIZE_ERO; ++sizeEro){
				erosion->setSizeErosion(sizeEro);
			
				for(nbDil = MIN_NB_DIL; nbDil <= MAX_NB_DIL; ++nbDil){
					dilatation->setNbDilatation(nbDil);
			
					for(sizeDil = MIN_SIZE_DIL; sizeDil <= MAX_SIZE_DIL; ++sizeDil){
						dilatation->setSizeDilatation(sizeDil);

						std::cout << (cpt * 100) / nbIterationTotal << "%" << std::endl;
						++cpt;
								
						nbCompFound = window.getNbValidPicture(5,etiquetage);
						
						statResult res_cur;
						res_cur.nbCompFound = nbCompFound;
						res_cur.nbEro = nbEro;
						res_cur.sizeEro = sizeEro;
						res_cur.nbDil = nbDil;
						res_cur.sizeDil = sizeDil;
						res_cur.thres = thres;
						
						file << res_cur.toCSV() << std::endl;
						results.push_back(res_cur);
					}
				}
			}
		}
	}
	
	std::sort(results.begin(), results.end(), sortRes);
		
	// Affichage des 10 meilleurs resultats
	std::cout << "Meilleurs resultats" << std::endl << std::endl;
	file << "-------------- 10 meilleurs res --------------" << std::endl << std::endl;
	file << "Nombre Comp trouve\tValeur Threshold\t" <<
			"Nombre d'erosions\tTaille d'erosion\t" <<
			"Nombre dilatation\tTaille dilatation" << std::endl;
	for(std::vector<statResult>::iterator it=results.begin(); it!=results.end() && it!=results.begin()+10; ++it){			
		std::cout << *it << std::endl;
		file << it->toCSV() << std::endl;
	}
	
	file.close();
	
    return 0;
}
