#ifndef DETECT_SHAPE_HPP
#define DETECT_SHAPE_HPP

#include "filtre.hpp"
#include "detectedBlob.hpp"
#include <vector>

class DetectShape : public Filtre {
	private:
		
		const unsigned NB_COMPS;

		static const unsigned ITE_MAX = 15;
		
		unsigned _cpt = 0;
		
		std::vector<DetectedBlob> compsOrdonnees;
		const std::vector<Composante>& comps;
		
		float note; /*0-100*/
		/* 100% de la note correspond a la moyenne des notes de chaque blob
		   Si une composante est trouvee sont coef est 1, sont coef est 0.5
		   
		   (20% de la note peut  correspondre a la forme de l'objet)
		*/
		
	public :
		// Contructeurs
		DetectShape(const std::vector<Composante>& pcomps, unsigned pNbComps = 5);
		virtual ~DetectShape(){}
		
		// Setter/Getter
		const std::vector<Composante>& getComps() const;
		
		// Methodes
		Mat render(Mat& img);
		
		void assignDetectedCompToBlob(int& meanMoveX_OUT, int& meanMoveY_OUT);
		bool estimateRemainingBlobs(const unsigned moveX, const unsigned moveY);
		void drawShape(Mat& resc);
		
		void computeNote();
		
		float computeDistanceMin(const DetectedBlob& comp_ord, const std::vector<Composante>& comps_copy);
		float computeSurfaceMax();
};


#endif
