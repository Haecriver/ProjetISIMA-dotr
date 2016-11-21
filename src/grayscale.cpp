#include "grayscale.hpp"

Grayscale::Grayscale()
{}

Mat Grayscale::render(Mat& img){
	Mat res;
	cvtColor(img, res, CV_RGB2GRAY );
	return res;
}
