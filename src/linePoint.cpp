#include "linePoint.hpp"
#include <iostream>

LinePoint::LinePoint() : LinePoint(Point(0,0)) {}

LinePoint::LinePoint(Point pPos):
pos(pPos.x, pPos.y)
{}
		
std::vector<LinePoint> LinePoint::convertCompToLinePoint(const std::vector<Composante>& comps){
	std::vector<LinePoint> res;
	for(Composante comp : comps){
		res.push_back(LinePoint(Point(comp.getPosition().x, comp.getPosition().y)));
	}
	return res;
}
