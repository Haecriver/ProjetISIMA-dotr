#include "window.hpp"

Window::Window(std::string pPathRes, unsigned pNbImg, unsigned pFps):
PATH_RES(pPathRes),
NB_IMG(pNbImg),
fps(pFps),
msToWaitOpt(1000/fps)	// Nombre de seconde a attendre dans le meilleur des cas
{
	chargementImgs();
}


/*!
 *  \brief Pose cote a cote une serie d'image
 *
 *  \param imgs : Serie d'image
 *  \return Images cote a cote
 */
Mat Window::catImages(std::vector<Mat> imgs)
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

// Chargement des images
void Window::chargementImgs(){
	for(unsigned i=0;i<NB_IMG;i++){
		std::string pathimg_=PATH_RES+setDigit(i,4)+".jpg";
		Mat temp = imread(pathimg_);
		if (!temp.empty()){	// Si l'image a bien ete chargee
			imgs.push_back(temp);
		}
	}
}

// On enregistre les images
void Window::enregistrementImgs(std::string nameFile){
	string pathFichier="rsc/"+nameFile + ".avi";
	int codec = CV_FOURCC('m', 'p', '4', 'v');
	outputVideo.open(pathFichier, codec, 15.0, Size(640, 480), true);
	if (!outputVideo.isOpened())
	{
	    std::cout << "Could not open the output video for write" << std::endl;
	}
}

void Window::renderAll(){
	std::vector<Mat> displayRenders;
	Mat render;
	
	// Chrono pour les fps
	std::chrono::time_point<std::chrono::system_clock> start, end;
	int msToWait;				// Nombre de seconde a attendre entre chaque image
		
	// Pour chaque images chargees
    for(Mat img: imgs){
    	start = std::chrono::system_clock::now();
    	for(Display display: displays){
    		displayRenders.push_back(display.render(img));
    	}
    	
    	render = catImages(displayRenders);
    	
    	displayRenders.clear();
    	
    	imshow("Rendu",render);
    	
    	if(outputVideo.isOpened()){
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
		int key = waitKey(msToWait);
		if ( key == 27 || key == 'q'){
			std::cout << "ESC key pressed by user" << std::endl;
			break; 
		}
    }
}

void Window::addDisplay(Display display){
	displays.push_back(display);
}

std::string setDigit(const int i, const int length)
{
    std::ostringstream ostr;

    if (i < 0)
        ostr << '-';

    ostr << std::setfill('0') << std::setw(length) << (i < 0 ? -i : i);

    return ostr.str();
}
