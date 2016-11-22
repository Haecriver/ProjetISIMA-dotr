#ifndef DILATATION_HPP
#define DILATATION_HPP

#include "filtre.hpp" 

class Dilatation : public Filtre {
	private:
		Mat elementDilatation;
		unsigned sizeDilatation;
		unsigned nbDilatation;
		int shape;
	public:
		// Constructeurs
		Dilatation(unsigned pSizeDilatation, unsigned pNbDilatation, int pShape = MORPH_CROSS);
		virtual ~Dilatation(){}
		
		// Setter/Getter
		void setSizeDilatation(unsigned pSizeDilatation);
		unsigned getSizeDilatation();
		
		void setNbDilatation(unsigned pNbDilatation);
		unsigned getNbDilatation();
		
		void setShape(int pShape);
		int getShape();
		
		// Methodes
		Mat render(Mat& img);
};

#endif
