#ifndef LINE_POINT_HPP
#define LINE_POINT_HPP

#include <vector>
#include <opencv2/highgui/highgui.hpp>

#include "composante.hpp"
using namespace cv;

class LinePoint {
	private:
		Point pos;						// Position du point
		bool belongsToLine = false;		// true si le point appartient a une ligne
		bool wasPivot = false;			// true si le point a ete utilise en tant que pivot
		bool extremite = false;
		double moduleCur = 0.0;
		unsigned nbLines = 0;
		
	public:
		LinePoint();
		LinePoint(Point pPos);
		
		static std::vector<LinePoint> convertCompToLinePoint(const std::vector<Composante>& comps);
		
		const Point& getPos() const;
		
		bool isBelongsToLine() const;
		void setBelongsToLine(bool val);
		
		bool getWasPivot() const;
		void setWasPivot(bool val);
		
		bool isExtremite() const;
		void setExtremite(bool val);
		
		unsigned getNbLines() const;
		void incNbLines();
		
		double getModuleCur() const;
		void setModuleCur(double val);
		
		double getDistance(const LinePoint& pt) const;
};

#endif

