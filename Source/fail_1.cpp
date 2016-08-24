#include <stdio.h>
#include <iostream>
#include<math.h>
#include "opencv2/core.hpp"
#include "opencv2/features2d.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/xfeatures2d.hpp"

using namespace std;
using namespace cv;
using namespace cv::xfeatures2d;

struct Drawback
{
	double dx;
	double dy;
};


int main(int argc, char** argv)
{
	double width=0, higth=0;
	//Получение изображений
	Mat img_1 = imread("C:\\dev\\MyProjects\\Work\\Example\\pic1.png", IMREAD_GRAYSCALE);
	cout << "Write original width and higth of photo\n";
	cin >> width >> higth;

	//cin.get();
	
	Mat img_2 = imread("C:\\dev\\MyProjects\\Work\\Example\\pic2_1.png", IMREAD_GRAYSCALE);

	if (!img_1.data || !img_2.data)
	{
		std::cout << " --(!) Error reading images " << std::endl; return -1;
	}

	//Обнаружение основных точек
	int minHessian = 100;
	
	Ptr<SurfFeatureDetector> detector=SurfDescriptorExtractor::create(minHessian);

	std::vector<KeyPoint> keypoints_1, keypoints_2;

	detector->detect(img_1, keypoints_1);
	detector->detect(img_2, keypoints_2);

	//Обнаружение основных точек на двух изображениях
	Ptr<SurfDescriptorExtractor> extractor= SurfDescriptorExtractor::create(minHessian);

	Mat descriptors_1, descriptors_2;
	//img_1.copyTo(descriptors_1);
	//img_2.copyTo(descriptors_2);
	extractor->compute(img_1, keypoints_1, descriptors_1);
	extractor->compute(img_2, keypoints_2, descriptors_2);

	//Сопастовление основных точек используя функцию FLANN
	FlannBasedMatcher matcher;
	std::vector< DMatch > matches;
	matcher.match(descriptors_1, descriptors_2, matches);

	double max_dist = 0; double min_dist = 100;
	
	//Вычисление максимального расстояния между основными точками
	for (int i = 0; i < descriptors_1.rows; i++)
	{
		double dist = matches[i].distance;
		if (dist < min_dist) min_dist = dist;
		if (dist > max_dist) max_dist = dist;
	}



	//-- Draw only "good" matches (i.e. whose distance is less than 2*min_dist,
	//-- or a small arbitary value ( 0.02 ) in the event that min_dist is very
	//-- small)
	//-- PS.- radiusMatch can also be used here.
	std::vector< DMatch > good_matches;

	for (int i = 0; i < descriptors_1.rows; i++)
	{
		if (matches[i].distance <= max(2 * min_dist, 0.02))
		{
			good_matches.push_back(matches[i]);
		}
	}

	//Отображение наиболее совподающих точек
	Mat img_matches;
	drawMatches(img_1, keypoints_1, img_2, keypoints_2,
		good_matches, img_matches, Scalar::all(-1), Scalar::all(-1),
		vector<char>(), DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS);

	//Отображение задетектированных пар
	imshow("Good Matches", img_matches);
	int a=good_matches.size();
	Drawback mist[69+2];
	for (int i = 0; i < (int)good_matches.size(); i++)
	{
		mist[i].dx = abs(keypoints_2[good_matches[i].queryIdx].pt.x - keypoints_1[good_matches[i].trainIdx].pt.x);
		mist[i].dy = abs(keypoints_2[good_matches[i].queryIdx].pt.y - keypoints_1[good_matches[i].trainIdx].pt.y);
		/*cout << mist[i].dx << " " << mist[i].dy<<" ";*/
		if (i == 67)
			a = 10;
	}
	////Расчет угла
	//double dangle = 0;
	//double y2 = keypoints_1[good_matches[1].queryIdx].pt.y, y1 = keypoints_1[good_matches[1].queryIdx].pt.y,
	//	x2 = keypoints_1[good_matches[1].queryIdx].pt.x, x1 = keypoints_1[good_matches[1].queryIdx].pt.x,
	//	_y2 = keypoints_2[good_matches[1].trainIdx].pt.y, _y1 = keypoints_2[good_matches[1].trainIdx].pt.y,
	//	_x2 = keypoints_2[good_matches[1].trainIdx].pt.x, _x1 = keypoints_2[good_matches[1].trainIdx].pt.x;
	//if ((y2 - y1)*(_y2 - _y1) / ((x2 - x1)*(_x2 - _x1)) != 1)
	//	dangle = atan(((y2 - y1) / (x2 - x1) - (_y2 - _y1) / (_x2 - _x1)) / 1 + ((y2 - y1) / (x2 - x1))*((_y2 - _y1) / (_x2 - _x1)));
	/*cout << dangle;*/
	waitKey(0);
	return 0;
}
