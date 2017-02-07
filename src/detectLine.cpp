#include "detectLine.hpp"

std::random_device rd;
std::mt19937 gen(rd());

// Contructeurs
DetectLine::DetectLine(const std::vector<Composante>& pcomps, unsigned pNbLines):
comps(pcomps),
NB_LINES(pNbLines)
{}

// Methodes
Mat DetectLine::render(Mat& img){
	Mat res(img.clone());
	getLinesFromPoints();
	
	for(Line el_line: lines){
		line(res, Point(el_line.getPts()[0]->getPos()), 
			Point(el_line.getPts()[3]->getPos()),
			Scalar(255, 255, 0), 3);
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
	

	while(searchingForLines){
		// Point pivot
		LinePoint* pivot = *selectRandomPoint(lonelyPoints);
		lonelyPoints.remove(pivot);
		//std::cout << "out x:"<<pivot->getPos().x << " y:" << pivot->getPos().y<< std::endl;
		searchingForPoints = true;
		
		while(searchingForPoints){
			LinePoint* cur = *selectRandomPoint(lonelyPoints);
			lonelyPoints.remove(cur);
			
			//std::cout << "in x:"<<cur->getPos().x << " y:" << cur->getPos().y<< std::endl;
			Line lineCur(pivot->getPos(), cur->getPos());
			
			if(lineCur.getIncludedPoints(allPoints)){
				lines.push_back(lineCur);
				
				searchingForPoints=false;
			}
			
			if(lonelyPoints.empty()){
				std::cout << "Erreur: aucune ligne detectee pour le point " << 
				"x:"<<pivot->getPos().x << " y:" << pivot->getPos().y << std::endl;
				
				searchingForPoints = false;
			}
		}
		
		// Maj des points a tirer
		for(unsigned i = 0; i < allPoints.size(); i++){
			if(!allPoints[i].isBelongsToLine()){
				lonelyPoints.push_back(&allPoints[i]);
			}
		}
		
		searchingForLines = lines.size() != NB_LINES;
	}
}

std::list<LinePoint*>::iterator DetectLine::selectRandomPoint(std::list<LinePoint*> pts) {
	std::list<LinePoint*>::iterator begin = pts.begin();
    std::uniform_int_distribution<> dis(0, std::distance(pts.begin(), pts.end()) - 1);
    std::advance(begin, dis(gen));
    return begin;
}
