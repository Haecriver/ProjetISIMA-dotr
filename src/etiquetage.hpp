#ifndef ETIQUETAGE_HPP
#define ETIQUETAGE_HPP

#include <vector>
#include <algorithm>

#include "filtre.hpp"
#include "composante.hpp"

class DetectedBlob {
	private :
		Composante comp;
		bool estimated;
		
		float note; /*0-100% Depend de la forme de l'objet, si estimee elle tombe a 10%*/
		
	public:
		DetectedBlob(Composante& pcomp, bool pestimated);
		
		void setComp(Composante& pcomp);
		Composante& getComp();
		
		void setEstimated(bool pestimated);
		bool isEstimated() const;
		
		void setNote(float pnote);
		float getNote() const;
		
		void computeNote();
};

class Etiquetage : public Filtre {
	private:
		const unsigned MAX_NB_COMP;
		const bool DETECT_SHAPE;
		
		static const unsigned DEPLACEMENT = 600;
		
		static const unsigned ITE_MAX = 15;
		unsigned _cpt = 0;
		
		std::vector<Composante> comps;
		std::vector<DetectedBlob> compsOrdonnees;
		
		float note; /*0-100*/
		/* 100% de la note correspond a la moyenne des notes de chaque blob
		   Si une composante est trouvee sont coef est 1, sont coef est 0.5
		   
		   (20% de la note peut  correspondre a la forme de l'objet)
		*/

		void detectShape(Mat& resc);
		
	public :
		// Contructeurs
		Etiquetage(unsigned pMaxNbComp = 5, bool pDetectShape = true);
		virtual ~Etiquetage(){}
		
		// Setter/Getter
		const std::vector<Composante>& getComps() const;
		
		// Methodes
		Mat render(Mat& img);
		
		void assignDetectedCompToBlob(int& meanMoveX_OUT, int& meanMoveY_OUT);
		void assignDetectedCompToBlob2(int& meanMoveX_OUT, int& meanMoveY_OUT);
		bool estimateRemainingBlobs(const unsigned moveX, const unsigned moveY);
		void drawShape(Mat& resc);
		
		void computeNote();
};

#endif
