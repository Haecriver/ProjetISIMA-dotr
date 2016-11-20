#ifndef ETIQUETAGE_HPP
#define ETIQUETAGE_HPP

#include <vector>

#include "filtre.hpp"
#include "composante.hpp"

class Etiquetage : public Filtre {
	private:
		std::vector<Composante> oldComps;

	public :
		Etiquetage();
		virtual ~Etiquetage(){}
		Mat render(Mat& img);
};

#endif
