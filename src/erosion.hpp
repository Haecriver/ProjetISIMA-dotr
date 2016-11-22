#ifndef EROSION_HPP
#define EROSION_HPP

#include "filtre.hpp" 

class Erosion : public Filtre {
	private:
		Mat elementErosion;
		unsigned sizeErosion;
		unsigned nbErosion;
		int shape;
	public:
		// Constructeurs
		Erosion(unsigned pSizeErosion, unsigned pNbErosion, int pShape = MORPH_CROSS);
		virtual ~Erosion(){}
		
		// Setter/Getter
		void setSizeErosion(unsigned pSizeErosion);
		unsigned getSizeErosion();
		
		void setNbErosion(unsigned pNbErosion);
		unsigned getNbErosion();
		
		void setShape(int pShape);
		int getShape();
		
		// Methode
		Mat render(Mat& img);
};


#endif
