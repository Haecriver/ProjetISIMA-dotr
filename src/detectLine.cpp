#include "detectLine.hpp"

// Contructeurs
DetectLine::DetectLine(const std::vector<Composante>& pcomps):
comps(pcomps)
{}

// Methodes
Mat DetectLine::render(Mat& img){
	Mat res(img.clone());
	getLinesFromPoints();
	
	for(Line el_line: lines){
		line(res, Point(el_line.getPts()[0]->getPos()), 
			Point(el_line.getPts()[3]->getPos()),
			Scalar(255, 0, 255), 3);
	}
	
	return res;
}

void DetectLine::getLinesFromPoints(){
	bool searchingForLines = true;
	bool searchingForPoints = true;
		
	allPoints = LinePoint::convertCompToLinePoint(comps);
	for(LinePoint pt : allPoints){
		lonelyPoints.push_back(&pt);
	}
	
	while(searchingForLines){
		// Point pivot
		LinePoint* pt_pivot = *selectRandomPoint(lonelyPoints);
		searchingForPoints = true;
		
		while(searchingForPoints){
			LinePoint* pt_cur = *selectRandomPoint(lonelyPoints);
			Line lineCur(pt_pivot->getPos(), pt_cur->getPos());
			
			if(lineCur.getIncludedPoints(allPoints)){
				searchingForPoints=false;
				lines.push_back(lineCur);
			}
		}
		
		// Maj des points a tirer
		for(LinePoint pt : allPoints){
			if(!pt.isBelongsToLine()){
				lonelyPoints.push_back(&pt);
			}
		}
		
		searchingForLines = lines.size() == 3;
	}
}

vector<LinePoint*>::iterator DetectLine::selectRandomPoint(std::vector<LinePoint*> pts) {
	vector<LinePoint*>::iterator start = pts.begin();
	static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, std::distance(start, pts.end()) - 1);
    std::advance(start, dis(gen));
    return start;
}
