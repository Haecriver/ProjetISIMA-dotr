#include "dilatation.hpp"

Dilatation::Dilatation(unsigned pSizeDilatation, unsigned pNbDilatation, int pShape):
sizeDilatation(pSizeDilatation),
nbDilatation(pNbDilatation),
shape(pShape)
{
	// MORPH_RECT, MORPH_CROSS,  MORPH_ELLIPSE 
	elementDilatation = getStructuringElement( shape,
                                       Size( 2*sizeDilatation + 1, 2*sizeDilatation+1 ),
                                       Point( sizeDilatation, sizeDilatation ) );
}

// Setter/Getter
void Dilatation::setSizeDilatation(unsigned pSizeDilatation){
	sizeDilatation = pSizeDilatation;
}

unsigned Dilatation::getSizeDilatation(){
	return sizeDilatation;
}

void Dilatation::setNbDilatation(unsigned pNbDilatation){
	nbDilatation = pNbDilatation;
}

unsigned Dilatation::getNbDilatation(){
	return nbDilatation;
}

void Dilatation::setShape(int pShape){
	shape = pShape;
}

int Dilatation::getShape(){
	return shape;
}

Mat Dilatation::render(Mat& img){
	Mat res;
	dilate(img, res, elementDilatation, Point(-1,-1), nbDilatation, BORDER_REPLICATE);
	return res;
}
