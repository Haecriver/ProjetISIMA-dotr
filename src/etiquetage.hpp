#ifndef ETIQUETAGE_HPP
#define ETIQUETAGE_HPP

#include <vector>
#include <algorithm>

#include "filtre.hpp"
#include "composante.hpp"

class Etiquetage : public Filtre {
	private:		
		std::vector<Composante> comps;
	
	public :
		// Contructeurs
		Etiquetage();
		virtual ~Etiquetage(){}
		
		// Setter/Getter
		const std::vector<Composante>& getComps() const;
		
		// Methodes
		Mat render(Mat& img);
};

#endif
