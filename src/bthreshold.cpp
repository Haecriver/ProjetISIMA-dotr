#include "bthreshold.hpp"

Bthreshold::Bthreshold(unsigned pthreshold_value):
threshold_value(pthreshold_value)
{}

Mat Bthreshold::render(Mat& img){
	Mat res;
	threshold(img, res, threshold_value, MAX_BINARY_VALUE, THRESHOLD_TYPE );
	return res;
}
