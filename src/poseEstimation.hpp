#ifndef POSEESTIMATION_HPP
#define POSEESTIMATION_HPP

#include "filtre.hpp"
#include "axe.hpp"
#include "line.hpp"
#include <vector>
#include <utility>

class PoseEstimation : public Filtre {
	private:
		Mat& K;
		const std::vector<Axe>& axes;
		const std::vector<Line>& lines;	
		
		std::vector<std::pair<LinePoint, std::vector<double> > > correspondingPoints;
		void computeCorrespondingPoints();
		Mat computeProjectionMatrix();
		
	public:
		PoseEstimation(Mat& K, const std::vector<Axe>& paxes, const std::vector<Line>& lines);

		Mat render(Mat& img);
};

#endif

