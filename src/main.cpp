/*!
 * \file main.cpp
 * \brief Fichier main du programme
 * \author Chevalier_Pierre
 * \version 0.1
 */
 
#include <string>
#include <vector>
#include <iostream>
#include <iomanip>

// Gestion du temps
#include <chrono>

// OpenCV
#include<opencv2/highgui/highgui.hpp>
#include<opencv2/imgproc/imgproc.hpp>

#include "composante.hpp"

using namespace cv;

unsigned fps = 30;
const std::string PATH_RES = "./rsc/Scenario_1_1/in1_";
const unsigned NB_IMG=517;

std::string setDigit(const int i, const int length)
{
    std::ostringstream ostr;

    if (i < 0)
        ostr << '-';

    ostr << std::setfill('0') << std::setw(length) << (i < 0 ? -i : i);

    return ostr.str();
}

/* 0: Binary
 1: Binary Inverted
 2: Threshold Truncated
 3: Threshold to Zero
 4: Threshold to Zero Inverted
*/
int THRESHOLD_TYPE = 0;
int THRESHOLD_VALUE = 150;
int const MAX_BINARY_VALUE = 255;

Mat applyFilter(Mat src){
	Mat res, element_erosion, element_dilatation;
	int erosion_size = 1;
	int dilatation_size = 1;
	int nb_erosion = 2;
	int nb_dilatation = 1;
	
	// Grayscale
	cvtColor(src, res, CV_BGR2GRAY );
	
	// Binary threshold
 	threshold(res, res, THRESHOLD_VALUE, MAX_BINARY_VALUE,THRESHOLD_TYPE );
 	
 	// Erosion
 	/* MORPH_RECT, MORPH_CROSS,  MORPH_ELLIPSE */
 	element_erosion = getStructuringElement( MORPH_ELLIPSE,
                                       Size( 2*erosion_size + 1, 2*erosion_size+1 ),
                                       Point( erosion_size, erosion_size ) );
                                       
    for(int i =0;i<nb_erosion;i++){
  		erode(res, res, element_erosion );
    }
    
    //Dilatation
    /* MORPH_RECT, MORPH_CROSS,  MORPH_ELLIPSE */
 	element_dilatation = getStructuringElement( MORPH_ELLIPSE,
                                       Size( 2*dilatation_size + 1, 2*dilatation_size+1 ),
                                       Point( dilatation_size, dilatation_size ) );
                                       
    for(int i =0;i<nb_dilatation;i++){
  		dilate(res, res, element_dilatation );
    }
	
	return res;
}

int main()
{
	std::vector<Mat> imgs;
	std::chrono::time_point<std::chrono::system_clock> start, end;
	int msToWait, msToWaitOpt = 1000/fps;	// Nombre de seconde a attendre entre chaque image
		
	for(unsigned i=0;i<NB_IMG;i++){
		std::string pathimg=PATH_RES+setDigit(i,4)+".jpg";
		Mat temp = imread(pathimg);
		if (!temp.empty()){
			imgs.push_back(temp);
		}
	}



    for(Mat img: imgs){
    	start = std::chrono::system_clock::now();
    	
    	imshow("Capturedemo",img);
    	imshow("Filter", applyFilter(img));
    	
		end = std::chrono::system_clock::now();
		std::chrono::duration<double> elapsed_seconds = end-start;
		msToWait = msToWaitOpt - elapsed_seconds.count();
		
		if(msToWait<0)
			msToWait=0;
			
		if (waitKey(msToWait) == 27){
			std::cout << "ESC key pressed by user" << std::endl;
			break; 
		}
    }
    
    waitKey(30);

    return 0;
}
