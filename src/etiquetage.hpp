#ifndef ETIQUETAGE_HPP
#define ETIQUETAGE_HPP

#include <vector>
#include <tuple>
#include <algorithm>

#include "filtre.hpp"
#include "composante.hpp"

class Etiquetage : public Filtre {
	private:
		const unsigned MAX_NB_COMP;
		static const unsigned DEPLACEMENT = 15;
		
		static const unsigned ITE_MAX = 20;
		unsigned _cpt = 0;
		
		std::vector<Composante> comps;
		std::vector<std::pair<Composante, bool>> compsOrdonnees;

	public :
		// Contructeurs
		Etiquetage(unsigned pMaxNbComp = 5);
		virtual ~Etiquetage(){}
		
		// Setter/Getter
		const std::vector<Composante>& getComps() const;
		
		// Methodes
		Mat render(Mat& img);
};

#endif
