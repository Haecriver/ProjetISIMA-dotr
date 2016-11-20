#ifndef GRAYSCALE_HPP
#define GRAYSCALE_HPP

#include "filtre.hpp" 

class Grayscale : public Filtre {
	private:		
	public:
		Grayscale();
		//virtual ~Grayscale(){}
		Mat render(Mat& img);
};

#endif
