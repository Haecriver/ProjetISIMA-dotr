#ifndef DETECT_LINE_HPP
#define DETECT_LINE_HPP

#include <vector>
#include <random>
#include "filtre.hpp"
#include "line.hpp"
#include "linePoint.hpp"

class DetectLine : public Filtre{
	private:
		const std::vector<Composante>& comps;
		std::vector<LinePoint> allPoints;
		std::vector<LinePoint*> lonelyPoints;
		std::vector<Line> lines;
	
	public:
		// Contructeurs
		DetectLine(const std::vector<Composante>& pcomps);
		virtual ~DetectLine(){}
		
		// Methodes
		Mat render(Mat& img);
		void getLinesFromPoints();
		static vector<LinePoint*>::iterator selectRandomPoint(std::vector<LinePoint*> pts);
};


#endif

