#ifndef DILATATION_HPP
#define DILATATION_HPP

#include "filtre.hpp" 

class Dilatation : public Filtre {
	private:
		Mat elementDilatation;
		unsigned sizeDilatation;
		unsigned nbDilatation;
	public:
		Dilatation(unsigned pSizeDilatation, unsigned pNbDilatation);
		virtual ~Dilatation(){}
		Mat render(Mat& img);
};

#endif
