#include "poseEstimation.hpp"

PoseEstimation::PoseEstimation(Mat& pK, const vector<Axe>& paxes, const vector<Line>& plines):
K(pK),
axes(paxes),
lines(plines)
{
}

Mat PoseEstimation::render(Mat& img){
	Mat res(img.clone()), M, invK, Rt;
	computeCorrespondingPoints();
	M = computeProjectionMatrix();
	// Extract [R|t]
	invK = K.inv(DECOMP_CHOLESKY);
	Rt = invK * M;
	
	/*std::cout << Rt.at<double>(0,0) << std::endl;
	std::cout << Rt.at<double>(1,0) << std::endl;
	std::cout << Rt.at<double>(2,0) << std::endl;
	std::cout << Rt.at<double>(3,0) << std::endl;
	std::cout << Rt.at<double>(0,1) << std::endl;
	std::cout << Rt.at<double>(1,1) << std::endl;
	std::cout << Rt.at<double>(2,1) << std::endl;
	std::cout << Rt.at<double>(3,1) << std::endl;
	std::cout << Rt.at<double>(0,2) << std::endl;
	std::cout << Rt.at<double>(1,2) << std::endl;
	std::cout << Rt.at<double>(2,2) << std::endl;
	std::cout << Rt.at<double>(3,2) << std::endl;*/
	std::cout << Rt << std::endl;
	
	return res;
}

void PoseEstimation::computeCorrespondingPoints(){
	correspondingPoints.clear();

	// Creation de la correspondance des points
	for(unsigned indexAxe=0; indexAxe < 2; indexAxe++){
		const Axe& axe = axes[indexAxe];
		// On cherche la ligne correspondante
		Line line;
		bool found = false;
		unsigned index = 0;
		while(!found && index<lines.size()){
			if(lines[index].sameCrossRatio(axe.crossRatio, axe.epsilon)){
				line = lines[index];
				found = true;
			}
			index++;
		}
		
		if(found){
			// On fait la correspondance des points de cette lignes
			std::pair<LinePoint, double*> listePairs [3];
			for(int i=0; i<3; i++){
				double point3d[3] = {0.0,0.0,0.0};
				listePairs[i] = std::pair<LinePoint, double*>(*(line.getPts()[i]),point3d);
			}
		
			listePairs[0].second[0] = axe.point1X;
			listePairs[0].second[1] = axe.point1Y;
			listePairs[0].second[2] = axe.point1Z;
		
			listePairs[1].second[0] = axe.point2X;
			listePairs[1].second[1] = axe.point2Y;
			listePairs[1].second[2] = axe.point2Z;
		
			listePairs[2].second[0] = axe.point3X;
			listePairs[2].second[1] = axe.point3Y;
			listePairs[2].second[2] = axe.point3Z;
		
			for(std::pair<LinePoint, double*> el_pair:listePairs){
				correspondingPoints.push_back(el_pair);
			}
		}
	}
}

Mat PoseEstimation::computeProjectionMatrix(){
	// Init de la matrice de projection
	Mat M = Mat::zeros(3, 4, CV_32F);
	Mat A = Mat::zeros(2*correspondingPoints.size(), 11, CV_32F);
	Mat b = Mat::zeros(2*correspondingPoints.size(), 1, CV_32F);
	
	Mat x;
	
	Mat At;
	Mat invAtA;
	
	std::cout<<A<<std::endl;
	// Calcul de la matrice A et b
	int index=0;
	for(auto pair : correspondingPoints){
		A.at<double>(0, index) = pair.second[0];
		A.at<double>(1, index) = pair.second[1];
		A.at<double>(2, index) = pair.second[2];
		A.at<double>(3, index) = 1.0;
		A.at<double>(4, index) = 0.0;
		A.at<double>(5, index) = 0.0;
		A.at<double>(6, index) = 0.0;
		A.at<double>(7, index) = 0.0;
		A.at<double>(8, index) = -pair.first.getPos().x * pair.second[0];
		A.at<double>(9, index) = -pair.first.getPos().x * pair.second[1];
		A.at<double>(10, index) = -pair.first.getPos().x * pair.second[2];
		
		A.at<double>(0, index+1) = 0.0;
		A.at<double>(1, index+1) = 0.0;
		A.at<double>(2, index+1) = 0.0;
		A.at<double>(3, index+1) = 0.0;
		A.at<double>(4, index+1) = pair.second[0];
		A.at<double>(5, index+1) = pair.second[1];
		A.at<double>(6, index+1) = pair.second[2];
		A.at<double>(7, index+1) = 1.0;
		A.at<double>(8, index+1) = -pair.first.getPos().y * pair.second[0];
		A.at<double>(9, index+1) = -pair.first.getPos().y * pair.second[1];
		A.at<double>(10, index+1) = -pair.first.getPos().y * pair.second[2];
		
		b.at<double>(1,index) = pair.first.getPos().x;
		b.at<double>(1,index+1) = pair.first.getPos().y;
		std::cout<<A<<std::endl;
		index+=2;
	}
	std::cout<<A<<std::endl;
	std::cout<<b<<std::endl;
	
	Mat temp;
	
	// Calcul sur les matrices
	// calcul At
	//At = A.t();
	transpose(A,At);
	
	// calcul invAtA
	mulTransposed(A,temp,true);
	invert(temp,invAtA);
	
	// calcul M
	x = invAtA*At*b;

	M.at<double>(0,0) = x.at<double>(0,0);
	M.at<double>(1,0) = x.at<double>(0,1);
	M.at<double>(2,0) = x.at<double>(0,2);
	M.at<double>(3,0) = x.at<double>(0,3);
	
	M.at<double>(0,1) = x.at<double>(0,4);
	M.at<double>(1,1) = x.at<double>(0,5);
	M.at<double>(2,1) = x.at<double>(0,6);
	M.at<double>(3,1) = x.at<double>(0,7);
	
	M.at<double>(0,2) = x.at<double>(0,8);
	M.at<double>(1,2) = x.at<double>(0,9);
	M.at<double>(2,2) = x.at<double>(0,10);
	M.at<double>(3,2) = 1.0;
	
	return M;
}
