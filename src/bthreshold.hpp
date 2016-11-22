#ifndef BTHRESHOLD_HPP
#define BTHRESHOLD_HPP

#include "filtre.hpp" 

class Bthreshold : public Filtre {
	private:
	
	  /* 0: Binary
		 1: Binary Inverted
		 2: Threshold Truncated
		 3: Threshold to Zero
		 4: Threshold to Zero Inverted
	   */
		static unsigned const THRESHOLD_TYPE = 0;
		static unsigned const MAX_BINARY_VALUE = 255;
		
		unsigned thresholdValue;
		
	public:
		// Constructeur
		Bthreshold(unsigned pThresholdValue);
		virtual ~Bthreshold(){}
				
		// Setter/Getter
		void setThresholdValue(unsigned pThresholdValue);
		unsigned getThresholdValue();
		
		// Methodes
		Mat render(Mat& img);
};

#endif
