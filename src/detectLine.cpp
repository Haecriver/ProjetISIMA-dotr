#include "detectLine.hpp"

std::random_device rd;
std::mt19937 gen(rd());

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
	for(unsigned i = 0; i < allPoints.size(); i++){
		lonelyPoints.push_back(&allPoints[i]);
	}
	
	/*for(LinePoint* ptpt : lonelyPoints){
		std::cout << "fill x:"<<ptpt->getPos().x << " y:" <<ptpt->getPos().y<< std::endl;
	}*/
	
	while(searchingForLines){
		// Point pivot
		LinePoint* pt_pivot = selectRandomPoint(lonelyPoints);
		//std::cout << "out x:"<<pt_pivot->getPos().x << " y:" << pt_pivot->getPos().y<< std::endl;
		searchingForPoints = true;
		
		while(searchingForPoints){
			LinePoint* pt_cur = selectRandomPoint(lonelyPoints);
			//std::cout << "in x:"<<pt_cur->getPos().x << " y:" << pt_cur->getPos().y<< std::endl;
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

LinePoint* DetectLine::selectRandomPoint(std::vector<LinePoint*> pts) {
    std::uniform_int_distribution<> dis(0, std::distance(pts.begin(), pts.end()) - 1);
    return pts[dis(gen)];
}
