#ifndef DETECTED_BLOB_HPP
#define DETECTED_BLOB_HPP

#include <opencv2/highgui/highgui.hpp>
#include "composante.hpp"

using namespace cv;

class DetectedBlob {
	private :
		constexpr static const float COEF_NOTE_SURFACE = 0.2; 
		constexpr static const float COEF_NOTE_PROPORTION = 0;
		constexpr static const float COEF_NOTE_DISTANCE = 0.8;
		
		Composante comp;
		bool estimated;	
	
		float note; /*0-100% Depend de :
					- la forme de l'objet
					- de son nombre de points
					- de sa distance avec l'ancienne position
			si estimee elle divisee par 4/5 a chaque ite*/
		
	public:
		DetectedBlob(const Composante& pcomp, bool pestimated);
		
		void setComp(Composante pcomp){
			comp = pcomp;
		}
		Composante& getComp(){
			return comp;
		}
		const Composante& getComp() const{
			return comp;
		}

		void setEstimated(bool pestimated){
			estimated = pestimated;
		}
		bool isEstimated() const{
			return estimated;
		}

		void setNote(float pnote){
			note = pnote;
		}
		float getNote() const{
			return note;
		}
		
		static float computeNote(float noteSurface, float noteProportion, float noteDistance);
};


#endif

