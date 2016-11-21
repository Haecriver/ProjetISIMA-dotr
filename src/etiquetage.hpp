#ifndef ETIQUETAGE_HPP
#define ETIQUETAGE_HPP

#include <vector>

#include "filtre.hpp"
#include "composante.hpp"

class Etiquetage : public Filtre {
	private:
		const unsigned MAX_NB_COMP;
		std::vector<Composante> oldComps;

	public :
		Etiquetage(unsigned pMaxNbComp = 5);
		virtual ~Etiquetage(){}
		Mat render(Mat& img);
};

#endif
