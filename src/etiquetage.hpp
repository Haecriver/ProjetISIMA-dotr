#ifndef ETIQUETAGE_HPP
#define ETIQUETAGE_HPP

#include <vector>

#include "filtre.hpp"
#include "composante.hpp"

class Etiquetage : public Filtre {
	private:
		const unsigned MAX_NB_COMP;
		std::vector<Composante> comps;
		std::vector<Composante> oldComps;

	public :
		// Contructeurs
		Etiquetage(unsigned pMaxNbComp = 5);
		virtual ~Etiquetage(){}
		
		// Setter/Getter
		const std::vector<Composante>& getComps() const;
		const std::vector<Composante>& getOldComps() const;
		
		// Methodes
		Mat render(Mat& img);
};

#endif
