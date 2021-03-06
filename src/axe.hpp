#ifndef AXE_HPP
#define AXE_HPP

#include <opencv2/core/core.hpp>
#include <iostream>
#include <string>
#include <vector>

using namespace cv;

struct Axe {
	Axe();
	
	double crossRatio;
	double epsilon;
		
	double point1X;
	double point1Y;
	double point1Z;
	double point1CR;
	
	double point2X;
	double point2Y;
	double point2Z;
	double point2CR;
	
	double point3X;
	double point3Y;
	double point3Z;
	double point3CR;
	
	double point4X;
	double point4Y;
	double point4Z;
	double point4CR;
	
	void write(FileStorage& fs) const;
	
	void read(const FileNode& node);
	
	static std::vector<Axe> getAxes(const std::string filename);

};

void write(FileStorage& fs, const std::string&, const Axe& x);
void read(const FileNode& node, Axe& x, const Axe& default_value = Axe());

#endif
