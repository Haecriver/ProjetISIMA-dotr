#include "linePoint.hpp"

LinePoint::LinePoint() : LinePoint(Point(0,0)) {}

LinePoint::LinePoint(const Point& pPos):
pos(pPos)
{}
		
std::vector<LinePoint> LinePoint::convertCompToLinePoint(const std::vector<Composante>& comps){
	std::vector<LinePoint> res;
	for(Composante comp : comps){
		res.push_back(LinePoint(comp.getPosition()));
	}
	return res;
}
