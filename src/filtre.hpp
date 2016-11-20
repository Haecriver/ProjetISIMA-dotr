#ifndef FILTRE_HPP
#define FILTRE_HPP

#include<opencv2/highgui/highgui.hpp>
#include<opencv2/imgproc/imgproc.hpp>

using namespace cv;

class Filtre{
	public :
		virtual ~Filtre() {}
		virtual Mat render(Mat& img)=0;
};

#endif
