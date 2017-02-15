#include "axe.hpp"

Axe::Axe():
crossRatio(0.0),
epsilon(0.0),
	
point1X(0.0),
point1Y(0.0),
point1Z(0.0),
point1CR(0.0),

point2X(0.0),
point2Y(0.0),
point2Z(0.0),
point2CR(0.0),

point3X(0.0),
point3Y(0.0),
point3Z(0.0),
point3CR(0.0),

point4X(0.0),
point4Y(0.0),
point4Z(0.0),
point4CR(0.0)
{}

void Axe::write(FileStorage& fs) const                        //Write serialization for this class
{
	fs << "{" 
	<< "crossRatio" << crossRatio
	<< "epsilon" << epsilon

	<< "point1X" << point1X
	<< "point1Y" << point1Y
	<< "point1Z" << point1Z
	<< "point1CR" << point1CR

	<< "point2X" << point2X
	<< "point2Y" << point2Y
	<< "point2Z" << point2Z
	<< "point2CR" << point2CR

	<< "point3X" << point3X
	<< "point3Y" << point3Y
	<< "point3Z" << point3Z
	<< "point3CR" << point3CR

	<< "point4X" << point4X
	<< "point4Y" << point4Y
	<< "point4Z" << point4Z
	<< "point4CR" << point4CR

	<<"}";
}

void Axe::read(const FileNode& node)                          //Read serialization for this class
{  
	crossRatio= (double)node["crossRatio"];
	epsilon= (double)node["epsilon"];
	
	point1X= (double)node["point1X"];
	point1Y= (double)node["point1Y"];
	point1Z= (double)node["point1Z"];
	point1CR= (double)node["point1CR"];

	point2X= (double)node["point2X"];
	point2Y= (double)node["point2Y"];
	point2Z= (double)node["point2Z"];
	point2CR= (double)node["point2CR"];

	point3X= (double)node["point3X"];
	point3Y= (double)node["point3Y"];
	point3Z= (double)node["point3Z"];
	point3CR= (double)node["point3CR"];

	point4X= (double)node["point4X"];
	point4Y= (double)node["point4Y"];
	point4Z= (double)node["point4Z"];
	point4CR= (double)node["point4CR"];
}

std::vector<Axe> Axe::getAxes(const std::string filename){
	FileStorage fs;
	std::vector<Axe> res;
	fs.open(filename, FileStorage::READ);
	if (!fs.isOpened())
	{
		std::cerr << "Failed to open " << filename << std::endl;
	}else{
		Axe axeX,axeY,axeZ;
		fs["axeX"] >> axeX;
		fs["axeY"] >> axeY;
		fs["axeZ"] >> axeZ;

		res.push_back(axeX);
		res.push_back(axeY);
		res.push_back(axeZ);
	}
	return res;

}

void write(FileStorage& fs, const std::string&, const Axe& x)
{
	x.write(fs);
}

void read(const FileNode& node, Axe& x, const Axe& default_value)
{
	if(node.empty())
		x = default_value;
	else
		x.read(node);
}
