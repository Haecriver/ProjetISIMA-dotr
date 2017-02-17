#include "poseEstimation.hpp"

PoseEstimation::PoseEstimation(Mat& pK, const std::vector<Axe>& paxes, const std::vector<Line>& plines):
K(pK),
axes(paxes),
lines(plines)
{
}

Mat PoseEstimation::render(Mat& img){
	Scalar color;
	int fontFace = CV_FONT_HERSHEY_COMPLEX_SMALL;
	double fontScale = 0.6;
	int thickness = 1;
	std::string str;
	
	double rotX,rotY,rotZ;
	Mat res(img.clone()), M, invK, Rt;
	computeCorrespondingPoints();
	M = computeProjectionMatrix();
	// Extract [R|t]
	invK = K.inv(DECOMP_CHOLESKY);
	//std::cout << "K" << invK << std::endl;
	Rt = invK * M;

	//std::cout << Rt << std::endl;
	
	rotX=atan2(Rt.at<double>(3,2),Rt.at<double>(3,3)) * 180.0/M_PI;
	rotY=atan2(Rt.at<double>(3,1),sqrt(Rt.at<double>(3,2)*Rt.at<double>(3,2)+Rt.at<double>(3,3)*Rt.at<double>(3,3))) * 180.0/M_PI;
	rotZ=atan2(Rt.at<double>(2,1),Rt.at<double>(1,1)) * 180.0/M_PI;
	
	//std::cout << "rotX:" << rotX << " rotY:" << rotY << " rotZ:" << rotZ  << std::endl;
	
	putText(res, "AngleX:" + std::to_string(rotX),  Point(10,20), fontFace, fontScale,
	    Scalar(255, 0, 255), thickness);
	putText(res, "AngleY:" + std::to_string(rotY),  Point(10,35), fontFace, fontScale,
	    Scalar(255, 0, 255), thickness);
	putText(res, "AngleZ:" + std::to_string(rotZ),  Point(10,50), fontFace, fontScale,
	    Scalar(255, 0, 255), thickness);
	
	// Test
	Mat point =  Mat::zeros(4, 1, CV_64F);
	point.at<double>(3,0) = 1;
	//std::cout << "test" << point << std::endl;
	Mat test = M * point;
	//std::cout << test << std::endl;
	circle(res, Point(test.at<double>(0,0),test.at<double>(1,0)), 5, Scalar(255,0,255),-5);
	
	return res;
}

void PoseEstimation::computeCorrespondingPoints(){
	correspondingPoints.clear();

	// Creation de la correspondance des points
	for(unsigned indexAxe=0; indexAxe < 3; indexAxe++){
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
			std::pair<LinePoint,std::vector<double> > listePairs [3];
			for(int i=0; i<3; i++){
				std::vector<double> point3d({0.0,0.0,0.0});
				listePairs[i] = std::pair<LinePoint, std::vector<double>>(LinePoint(*(line.getPts()[i+1])),point3d);
			}
		
			listePairs[0].second[0] = axe.point2X;
			listePairs[0].second[1] = axe.point2Y;
			listePairs[0].second[2] = axe.point2Z;
		
			listePairs[1].second[0] = axe.point3X;
			listePairs[1].second[1] = axe.point3Y;
			listePairs[1].second[2] = axe.point3Z;
		
			listePairs[2].second[0] = axe.point4X;
			listePairs[2].second[1] = axe.point4Y;
			listePairs[2].second[2] = axe.point4Z;
		
			for(std::pair<LinePoint, std::vector<double> > el_pair:listePairs){
				correspondingPoints.push_back(el_pair);
			}
		}
	}
}

Mat PoseEstimation::computeProjectionMatrix(){
	// Init de la matrice de projection
	Mat M; M = Mat(3, 4, CV_64F);
	Mat A; A = Mat(2*correspondingPoints.size(), 11, CV_64F);
	Mat b; b = Mat(2*correspondingPoints.size(), 1, CV_64F);
	
	Mat x;
	
	Mat At;
	Mat invAtA;
	
	/*for(int index = 0; index<A.rows; index+=2){
		std::pair<LinePoint, std::vector<double> > pair = correspondingPoints[index/2];
		std::cout << "u:"<<pair.first.getPos().x<<" v:"<<pair.first.getPos().y
			<<" X:" << pair.second[0]<<" Y:" << pair.second[1]<<" Z:" << pair.second[2]<<std::endl;
	}*/
	
	// Calcul de la matrice A et b
	unsigned index = 0;
	for(std::pair<LinePoint, std::vector<double> > pair : correspondingPoints){		
	// TODO: REMPLIR A CORRECTEMENT
		A.at<double>(index,0) = pair.second[0];
		A.at<double>(index,1) = pair.second[1];
		A.at<double>(index,2) = pair.second[2];
		A.at<double>(index,3) = 1.0;
		A.at<double>(index,4) = 0.0;
		A.at<double>(index,5) = 0.0;
		A.at<double>(index,6) = 0.0;
		A.at<double>(index,7) = 0.0;
		A.at<double>(index,8) = -pair.first.getPos().x * pair.second[0];
		A.at<double>(index,9) = -pair.first.getPos().x * pair.second[1];
		A.at<double>(index,10) = -pair.first.getPos().x * pair.second[2];
		
		A.at<double>(index+1,0) = 0.0;
		A.at<double>(index+1,1) = 0.0;
		A.at<double>(index+1,2) = 0.0;
		A.at<double>(index+1,3) = 0.0;
		A.at<double>(index+1,4) = pair.second[0];
		A.at<double>(index+1,5) = pair.second[1];
		A.at<double>(index+1,6) = pair.second[2];
		A.at<double>(index+1,7) = 1.0;
		A.at<double>(index+1,8) = -pair.first.getPos().y * pair.second[0];
		A.at<double>(index+1,9) = -pair.first.getPos().y * pair.second[1];
		A.at<double>(index+1,10) = -pair.first.getPos().y * pair.second[2];

		b.at<double>(index,1) = pair.first.getPos().x;
		b.at<double>(index+1,1) = pair.first.getPos().y;
	
		index+=2;
	}
	//std::cout<<A<<std::endl;
	//std::cout<<b<<std::endl;
	
	Mat temp;
	
	// Calcul sur les matrices
	// calcul At
	//At = A.t();
	transpose(A,At);
	
	//std::cout << At << std::endl;
	
	// calcul invAtA
	invert(At*A,invAtA,DECOMP_SVD);
	//std::cout << invAtA << std::endl;
	
	// calcul M
	x = invAtA*At*b;
	
	//std::cout << x << std::endl;

	M.at<double>(0,0) = x.at<double>(0,0);
	M.at<double>(0,1) = x.at<double>(0,1);
	M.at<double>(0,2) = x.at<double>(0,2);
	M.at<double>(0,3) = x.at<double>(0,3);
	
	M.at<double>(1,0) = x.at<double>(0,4);
	M.at<double>(1,1) = x.at<double>(0,5);
	M.at<double>(1,2) = x.at<double>(0,6);
	M.at<double>(1,3) = x.at<double>(0,7);
	
	M.at<double>(2,0) = x.at<double>(0,8);
	M.at<double>(2,1) = x.at<double>(0,9);
	M.at<double>(2,2) = x.at<double>(0,10);
	M.at<double>(2,3) = 1.0;
	
	//std::cout << "M" << M << std::endl;
	
	return M;
}
