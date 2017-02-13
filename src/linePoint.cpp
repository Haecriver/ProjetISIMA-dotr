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

const Point& LinePoint::getPos() const{
	return pos;
}

bool LinePoint::isBelongsToLine() const{
	return belongsToLine;
}

void LinePoint::setBelongsToLine(bool val){
	belongsToLine = val;
}

bool LinePoint::getWasPivot() const{
	return wasPivot;
}

void LinePoint::setWasPivot(bool val){
	wasPivot = val;
}

bool LinePoint::isExtremite() const{
	return extremite;
}

void LinePoint::setExtremite(bool val){
	extremite = val;
}

unsigned LinePoint::getNbLines() const{
	return nbLines;
}

void LinePoint::incNbLines(){
	nbLines ++;
}

double LinePoint::getModuleCur() const{
	return moduleCur;
}

void LinePoint::setModuleCur(double val){
	moduleCur = val;
}

double LinePoint::getDistance(const LinePoint& pt) const{
	return sqrt((pos.x-pt.getPos().x)*(pos.x-pt.getPos().x) + (pos.y-pt.getPos().y)*(pos.y-pt.getPos().y));
}
