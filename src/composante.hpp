/*!
 * \file main.cpp
 * \brief Declaration de l'objet composante, qui contient les points d'une composante connexe d'une image
 * \author Chevalier_Pierre
 * \version 0.1
 */
 
#ifndef COMPOSANTE_HPP
#define COMPOSANTE_HPP

#include <opencv2/highgui/highgui.hpp>
#include <vector>

using namespace cv;

class Composante{
	private:
		std::vector<Point> points;
		Point pos;
		
		void computeMeanPos();
		static void computeAComposante(Mat& copy, Point pointCur, Composante& cur);
		
	public:
		Composante();
		
		static std::vector<Composante> getCompostantes(const Mat& src);
			
		void addPoint(Point point){
			points.push_back(point);
		}
		
		Point getPosition(){
			return pos;
		}	
};

#endif
