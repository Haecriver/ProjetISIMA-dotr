#include "adathreshold.hpp"

Adathreshold::Adathreshold(double pRatio,unsigned pDebutIteration):
ratio(pRatio),
debutIteration(pDebutIteration)
{
	if(ratio>100) throw std::domain_error("Mauvais ratio");
}

// Setter/Getter
void Adathreshold::setRatio(double pRatio){
	ratio = pRatio;
}

double Adathreshold::getRatio(){
	return ratio;
}

Mat Adathreshold::render(Mat& img){
	const static unsigned ITERATION_MAX = 1000;

	Mat res;
	MatIterator_<uchar> it, end;
			
	bool cont = true, control_cont_iteration;
	unsigned ite = 0;
	unsigned count_white_pixel, count_black_pixel;
	unsigned upper_border = MAX_BINARY_VALUE, lower_border = 0;
	unsigned thresh_cur = 0;
	double ratio_cur = 0.0;
	
	while(cont){
		res = img.clone();
		count_white_pixel = 0;
		count_black_pixel = 0;
		control_cont_iteration = true;

		// On fait le threshold manuellement
		// Parcours de l'image
		for(int j=0; j<res.rows; j++){
			for(int i=0; i<res.cols; i++){	
				if(res.ptr(j)[i] > thresh_cur){
		    		res.ptr(j)[i] = MAX_BINARY_VALUE;
		    		count_white_pixel++;
		    	} else {
		    		res.ptr(j)[i] = 0;
		    		count_black_pixel++;
		    	}
			}
		}
       
        // On regarde si on doit faire un autre threshold
        ratio_cur = ((double)count_white_pixel * 100.0) / (double)(count_black_pixel+1.0);
        if((double)abs(ratio - ratio_cur) > debutIteration){
        	// Dichotomie
        	if(ratio_cur > ratio) {
				thresh_cur = thresh_cur+(upper_border-thresh_cur)/2;
			} else if (ratio_cur < ratio) {
				thresh_cur = lower_border + thresh_cur/2;
			}
        	
        }else{
        	// Iterative
        	if(ratio_cur > ratio) {
				// Too many white pixels spotted, increasing threshold value
				if (thresh_cur < upper_border) {
					thresh_cur = thresh_cur + 1;
				}
			}
			else if (ratio_cur < ratio) {
				// Not enough white pixels, lowering threshold value
				if(thresh_cur > lower_border+1) {
					thresh_cur = thresh_cur - 1;					
				}
			}
			
        	control_cont_iteration = MIN_THRESHOLD_VALUE < thresh_cur && MAX_THRESHOLD_VALUE > thresh_cur;	
        }
        
        cont = 
        	abs_double(ratio - ratio_cur) > THRESHOLD_PRECISION &&
        	ite < ITERATION_MAX &&
        	control_cont_iteration;
		
        if(cont){
        	res.release();
        } else if (!control_cont_iteration) {
        	// Si l'algo depasse les bornes imposees,
        	// on considere l'image comme noire
		    for(int j=0; j<res.rows; j++){
				for(int i=0; i<res.cols; i++){	
					res.ptr(j)[i] = 0;
				}
		    }
        }
        
        ite++;
	}
	
	if(DEBUG){
		std::cout << "RES : thresh cur : " << thresh_cur << std::endl;
		std::cout << "RES : Ratio : " << std::setprecision(10) << ratio_cur << std::endl;
		std::cout << "RES : nbblanc : " << count_white_pixel << std::endl;
		std::cout << "RES : nbblack : " << count_black_pixel << std::endl;
	}
	
	return res;
}


double abs_double(double nb){
	if(nb<0.0) nb*=-1;
	return nb;
}
