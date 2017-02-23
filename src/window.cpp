#include "window.hpp"

Window::Window(std::string pPathRes, unsigned pFps, bool pImageMode, bool pshowFps):
PATH_RES(pPathRes),
fps(pFps),
msToWaitOpt(1000/fps),	// Nombre de milliseconde a attendre dans le meilleur des cas
imageMode(pImageMode),
showFps(pshowFps)
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
			if(img.channels() == 1){
				cvtColor(img, img, CV_GRAY2RGB);
			}
			Size sz = img.size();
			img.copyTo(res(Rect(width_cur, 0, sz.width, sz.height)));
			width_cur+=sz.width;
		}
    }
	return res;
}

// Chargement des images
void Window::chargementImgs(){
	_imgs.open(PATH_RES+ "%04d.jpg");

	if( !_imgs.isOpened() ){
		throw std::domain_error("Error when reading steam_avi");
	}
}

// On enregistre les images
void Window::enregistrementImgs(std::string nameFile){
	std::string pathFichier="rsc/" + nameFile + ".avi";
	int codec = CV_FOURCC('m', 'p', '4', 'v');
	outputVideo.open(pathFichier, codec, 15.0, Size(640, 480), true);
	if (!outputVideo.isOpened())
	{
	    std::cout << "Could not open the output video for write" << std::endl;
	}
}

void Window::renderAll(){
	std::vector<Mat> displayRenders;
	Mat render, img;
	unsigned cpt = 0;
	double effectiveTime = 0.0;
	
	// Chrono pour les fps
	std::chrono::time_point<std::chrono::system_clock> start, end;
	int msToWait;				// Nombre de seconde a attendre entre chaque image
		
	// Booleen continue
	bool cont = _imgs.read(img);
	bool freeze = false;

	// Pour chaque images chargees
   	while(cont){	      
	   	if(!freeze){
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
			if(showFps){
				std::cout<<"== Image rendu en : " << elapsed_seconds.count()*1000
					<< "ms soit :" << 1/ (elapsed_seconds.count()) << " FPS ==" <<std::endl;
			}
			
			effectiveTime+=elapsed_seconds.count()*1000;
			cpt++;
			
			// Si <0, cela veut dire que l'on a pas a attendre pour afficher l'image suivante
			if(msToWait<0){
				msToWait=0;
			}
		}else{
			msToWait=0;
		}
		
		// On attend pend msToWait ms l'appuie sur la touche ESC
		int key = waitKey(msToWait);
		switch(key){
			case 27:
			case 'q':
				std::cout << "ESC key pressed by user" << std::endl;
				cont = false;
			break;
			
			case ' ':
				std::cout << "Save current image (" << cpt << ")" << std::endl;
				imwrite("./results/image_"+ std::to_string(cpt) +".png",render);
			break;
			
			default:
			break;
		
		}
		
		// On libere la memoires
		render.release();
		img.release();

		if(!imageMode && cont){
			// increment
			cont = _imgs.read(img);
		}else{
			freeze = true;
		}
		
		
    }
    std::cout<<"== Images rendues en : " << effectiveTime
		<< "ms soit environ :" << 1/ (effectiveTime/(cpt*1000)) << " FPS ==" <<std::endl;
}

unsigned Window::getNbValidPicture(unsigned nbCompToFind, Etiquetage* filtre_etiquetage_courant){
	unsigned cpt = 0;
	Mat img;
	
	// Pour chaque images chargees
   while(_imgs.read(img)){
    	if(displays[0].isAllCompFound(nbCompToFind, filtre_etiquetage_courant, img)){
    		cpt++;
    	}
    	img.release();
    }
    
    return cpt;
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
