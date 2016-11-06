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

/*!
 *  \brief Renvoie une string representant l'entier i avec un certain nombre de digits
 *
 *  \param i : Entier a representer.
 *  \param lenght : Nombre de digits
 *  \return string representant l'entier
 */
std::string setDigit(const int i, const int length)
{
    std::ostringstream ostr;

    if (i < 0)
        ostr << '-';

    ostr << std::setfill('0') << std::setw(length) << (i < 0 ? -i : i);

    return ostr.str();
}

/*!
 *  \brief Pose cote a cote une serie d'image
 *
 *  \param imgs : Serie d'image
 *  \return Images cote a cote
 */
Mat catImages(std::vector<Mat> imgs)
{	
	unsigned width=0,width_cur=0;
	Mat res;
	if(imgs.size()>0){

		for(Mat img: imgs){
			width+=img.size().width;
		}
	
		res = Mat(imgs[0].size().height, width, CV_8UC3);
	
		for(Mat img: imgs){
			Size sz = img.size();
			img.copyTo(res(Rect(width_cur, 0, sz.width, sz.height)));
			width_cur+=sz.width;
		}
    }
	return res;
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

/*!
 *  \brief Fonction temporaire, applique les filtre sur une copie de l'image en reference
 *
 *  \param src : Ref image.
 *  \return Image traitee
 */
Mat applyFilter(const Mat& src){
	Mat res, res_gray, element_erosion, element_dilatation;
	std::vector<Composante> comps;
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
    
    // Composante connexe
    comps = Composante::getCompostantes(res);
    
    // RGB ET POURTANT BRG ??
    cvtColor(res, res, CV_GRAY2RGB);
    
    // Detection des points calcules
    for(Composante comp : comps){
    	circle(res, comp.getPosition(), 5, Scalar(255,0,0));
    }
	
	return res;
}

int main(int argc, const char* argv[] )
{
	// Chrono pour les fps
	std::chrono::time_point<std::chrono::system_clock> start, end;
	std::vector<Mat> imgs;		// Liste des images chargees
	int msToWait;				// Nombre de seconde a attendre entre chaque image
	int msToWaitOpt = 1000/fps;	// Nombre de seconde a attendre dans le meilleur des cas
	Mat render;
	
	bool record_capture;
	std::string nomFichier,pathFichier;
	VideoWriter outputVideo;  
	
	// Chargement des images
	for(unsigned i=0;i<NB_IMG;i++){
		std::string pathimg=PATH_RES+setDigit(i,4)+".jpg";
		Mat temp = imread(pathimg);
		if (!temp.empty()){	// Si l'image a bien ete chargee
			imgs.push_back(temp);
		}
	}
	
	// Verif des args du main
	record_capture = argc==2;
	// Dans le cas d'un enregistrement video, creation du fichier
	if(record_capture){
		nomFichier=std::string(argv[1]) + ".avi";
		pathFichier="rsc/"+nomFichier;
		std::cout<< pathFichier << std::endl;
		outputVideo.open("video.AVI",  CV_FOURCC('M', 'J', 'P', 'G'), 15.0, Size(640, 480));
		if (!outputVideo.isOpened())
		{
		    std::cout  << "Could not open the output video for write" << std::endl;
			record_capture = false;
		}
	}

	// Pour chaque images chargees
    for(Mat img: imgs){
    	start = std::chrono::system_clock::now();
    	
    	render = catImages(std::vector<Mat>({img,applyFilter(img)}));
    	imshow("Rendu",render);
    	if(record_capture){
    		outputVideo << render;
    	}
    	
    	// On calcul le temps de l'application du filtre
		end = std::chrono::system_clock::now();
		std::chrono::duration<double> elapsed_seconds = end-start;
		msToWait = msToWaitOpt - elapsed_seconds.count();
		
		// Si <0, cela veut dire que l'on a pas a attendre pour afficher l'image suivante
		if(msToWait<0)
			msToWait=0;
		
		// On attend pend msToWait ms l'appuie sur la touche ESC
		if (waitKey(msToWait) == 27){
			std::cout << "ESC key pressed by user" << std::endl;
			break; 
		}
    }
    
    return 0;
}
