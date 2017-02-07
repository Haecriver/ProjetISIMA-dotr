#ifndef DETECT_LINE_HPP
#define DETECT_LINE_HPP

#include <vector>
#include <list>
#include <random>
#include "filtre.hpp"
#include "line.hpp"
#include "linePoint.hpp"

class DetectLine : public Filtre{
	private:
		const std::vector<Composante>& comps;
		const unsigned NB_LINES;
		
		std::vector<LinePoint> allPoints;
		std::list<LinePoint*> lonelyPoints;
		std::vector<Line> lines;
	
	public:
		// Contructeurs
		DetectLine(const std::vector<Composante>& pcomps, unsigned pNbLines = 3);
		virtual ~DetectLine(){}
		
		// Methodes
		Mat render(Mat& img);
		void getLinesFromPoints();
		static std::list<LinePoint*>::iterator selectRandomPoint(std::list<LinePoint*> pts);
};


#endif

