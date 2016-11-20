#include "erosion.hpp"

Erosion::Erosion(unsigned pSizeErosion, unsigned pNbErosion):
sizeErosion(pSizeErosion),
nbErosion(pNbErosion)
{
	// MORPH_RECT, MORPH_CROSS,  MORPH_ELLIPSE 
	elementErosion = getStructuringElement( MORPH_CROSS,
                                       Size( 2*sizeErosion + 1, 2*sizeErosion+1 ),
                                       Point( sizeErosion, sizeErosion ) );
}

Mat Erosion::render(Mat& img){
	Mat res;
	erode(img, res, elementErosion, Point(-1,-1), nbErosion, BORDER_REPLICATE);
	return res;
}
