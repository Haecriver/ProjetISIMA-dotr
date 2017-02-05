#ifndef LINE_POINT_HPP
#define LINE_POINT_HPP

#include <vector>
#include <opencv2/highgui/highgui.hpp>

#include "composante.hpp"
using namespace cv;

class LinePoint {
	private:
		Point pos;
		bool belongsToLine = false;
		
	public:
		LinePoint();
		LinePoint(Point pPos);
		
		static std::vector<LinePoint> convertCompToLinePoint(const std::vector<Composante>& comps);
		
		const Point& getPos() const{
			return pos;
		}
		
		bool isBelongsToLine() const{
			return belongsToLine;
		}
		
		void setBelongsToLine(bool val){
			belongsToLine = val;
		}
		
		double getDistance(const LinePoint& pt) const{
			return sqrt((pos.x-pt.getPos().x)*(pos.x-pt.getPos().x) + (pos.y-pt.getPos().y)*(pos.y-pt.getPos().y));
		}
};

#endif

