#include "dilatation.hpp"

Dilatation::Dilatation(unsigned pSizeDilatation, unsigned pNbDilatation):
sizeDilatation(pSizeDilatation),
nbDilatation(pNbDilatation)
{
	// MORPH_RECT, MORPH_CROSS,  MORPH_ELLIPSE 
	elementDilatation = getStructuringElement( MORPH_CROSS,
                                       Size( 2*sizeDilatation + 1, 2*sizeDilatation+1 ),
                                       Point( sizeDilatation, sizeDilatation ) );
}

Mat Dilatation::render(Mat& img){
	Mat res;
	dilate(img, res, elementDilatation, Point(-1,-1), nbDilatation, BORDER_REPLICATE);
	return res;
}
