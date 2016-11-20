#ifndef EROSION_HPP
#define EROSION_HPP

#include "filtre.hpp" 

class Erosion : public Filtre {
	private:
		Mat elementErosion;
		unsigned sizeErosion;
		unsigned nbErosion;
	public:
		Erosion(unsigned pSizeErosion, unsigned pNbErosion);
		virtual ~Erosion(){}
		Mat render(Mat& img);
};


#endif
