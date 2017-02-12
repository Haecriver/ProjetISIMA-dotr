#ifndef DETECT_LINE_HPP
#define DETECT_LINE_HPP

#include <vector>
#include <list>
#include <random>
#include <string>
#include <map>

#include "filtre.hpp"
#include "line.hpp"
#include "linePoint.hpp"
#include "axe.hpp"

using std::string;

class DetectLine : public Filtre{
	private:
		const std::vector<Axe>& axes;					// Vecteur des axes du modele
		const std::vector<Composante>& comps;			// Vecteur de composants donne par le filtre d'etiquetage
		const unsigned NB_LINES;						// Nombre de lignes a detecte
		const bool DISPLAY_SEARCHING;					// Affichage de la recherche de ligne
		const static unsigned NB_MAX_ITERATION = 10;	// Nombre max d'iterations en erreur toleree

		
		std::vector<Line> lines;						// Stockage des lignes detectees
		
		static std::vector<LinePoint*>::iterator selectRandomPoint(std::vector<LinePoint*> pts);
		void getLinesFromPoints(Mat& img);
		void getLinesFromPoints2(Mat& img);
	
	public:
		// Contructeurs
		DetectLine(const std::vector<Axe>& pAxes, const std::vector<Composante>& pcomps,
			unsigned pNbLines, bool pDisplaySearching = false);
		virtual ~DetectLine(){}
		
		// Methodes
		Mat render(Mat& img);
		

};


#endif

