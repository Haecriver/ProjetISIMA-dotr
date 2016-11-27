#ifndef ADATHRESHOLD_HPP
#define ADATHRESHOLD_HPP

#include "filtre.hpp" 
#include <stdexcept>
#include <cmath>
#include <iostream>
#include <iomanip>      // std::setprecision



class Adathreshold : public Filtre {
	private:
		static const bool DEBUG = true;
		
		static const unsigned MAX_BINARY_VALUE = 255;
		static const unsigned MIN_THRESHOLD_VALUE = 120;
		static const unsigned MAX_THRESHOLD_VALUE = 230;
		static constexpr double THRESHOLD_PRECISION = 0.1;
		
		double ratio;
		unsigned debutIteration;
		
		
	public:
		// Constructeur
		Adathreshold(double pRatio, unsigned pDebutIteration = 10);
		virtual ~Adathreshold(){}
				
		// Setter/Getter
		void setRatio(double pRatio);
		double getRatio();
		
		// Methodes
		Mat render(Mat& img);
};

double abs_double(double nb);

#endif
