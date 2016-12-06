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
		unsigned note; /*0-100*/
		
	public:
		DetectedBlob(Composante& pcomp, bool pestimated);
		
		void setComp(Composante& pcomp);
		Composante& getComp();
		
		void setEstimated(bool pestimated);
		const bool& isEstimated();
		
		void setNote(unsigned pnote);
		const unsigned& getNote();
};

class Etiquetage : public Filtre {
	private:
		const unsigned MAX_NB_COMP;
		const bool DETECT_SHAPE;
		
		static const unsigned DEPLACEMENT = 600;
		
		static const unsigned ITE_MAX = 20;
		unsigned _cpt = 0;
		
		std::vector<Composante> comps;
		std::vector<DetectedBlob> compsOrdonnees;

	public :
		// Contructeurs
		Etiquetage(unsigned pMaxNbComp = 5, bool pDetectShape = true);
		virtual ~Etiquetage(){}
		
		// Setter/Getter
		const std::vector<Composante>& getComps() const;
		
		// Methodes
		Mat render(Mat& img);
};

#endif
