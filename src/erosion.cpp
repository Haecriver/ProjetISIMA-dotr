#include "erosion.hpp"

Erosion::Erosion(unsigned pSizeErosion, unsigned pNbErosion, int pShape):
sizeErosion(pSizeErosion),
nbErosion(pNbErosion),
shape(pShape)
{
	// MORPH_RECT, MORPH_CROSS,  MORPH_ELLIPSE 
	elementErosion = getStructuringElement( shape,
                                       Size( 2*sizeErosion + 1, 2*sizeErosion+1 ),
                                       Point( sizeErosion, sizeErosion ) );
}

// Setter/Getter
void Erosion::setSizeErosion(unsigned pSizeErosion){
	sizeErosion = pSizeErosion;
}

unsigned Erosion::getSizeErosion(){
	return sizeErosion;
}

void Erosion::setNbErosion(unsigned pNbErosion){
	nbErosion = pNbErosion;
}

unsigned Erosion::getNbErosion(){
	return nbErosion;
}

void Erosion::setShape(int pShape){
	shape = pShape;
}

int Erosion::getShape(){
	return shape;
}

Mat Erosion::render(Mat& img){
	Mat res;
	erode(img, res, elementErosion, Point(-1,-1), nbErosion, BORDER_REPLICATE);
	return res;
}
