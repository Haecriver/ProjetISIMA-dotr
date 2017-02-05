#include "bthreshold.hpp"

Bthreshold::Bthreshold(unsigned pThresholdValue):
thresholdValue(pThresholdValue)
{}

// Setter/Getter
void Bthreshold::setThresholdValue(unsigned pThresholdValue){
	thresholdValue = pThresholdValue;
}

unsigned Bthreshold::getThresholdValue(){
	return thresholdValue;
}

Mat Bthreshold::render(Mat& img){
	Mat res;
	threshold(img, res, thresholdValue, MAX_BINARY_VALUE, THRESHOLD_TYPE ); 
	return res;
}
