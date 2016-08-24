#include<cv.h>
#include<stdio.h>
#include<iostream>>
#include "opencv2/core.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/videoio/videoio.hpp"
using namespace std;
using namespace cv;

Mat img;
bool flag=0;
Rect selection;
Point original;
int track_object = 0;

void callbackButton(int state, void *pointer)
{
	cout << "Ok";
}

static void onMouse(int event, int x, int y, int flags, void* param)
{
	if (flag)
	{
		selection.x = MIN(x, original.x);
		selection.y = MIN(y, original.y);
		selection.width = abs(x - original.x);
		selection.height = abs(y - original.y);
		selection &= Rect(0, 0, img.cols, img.rows);
	}
	switch (event)
	{
	case EVENT_LBUTTONDOWN:
		original = Point(x, y);
		selection = Rect(x, y, 0, 0);
		flag = !flag;
		break;
	case EVENT_LBUTTONUP:
		flag = 0;
		if (selection.width > 0 && selection.height > 0)
			track_object = -1;
		break;
	}
	
}

static void FindPattern(Mat capture1, Mat templ, int search_metod, Mat *gray_result)
{
	const int full_angle = 360;
	double minVal; double maxVal; Point minLoc; Point maxLoс;
	Mat result,rot_mat;
	Point matchLoc;
	double angle=1;
	resize(capture1, capture1, Size(640, 480));
	//Поиск шаблона
	//for (int i = 0;i < full_angle;i++)
	//{
		matchTemplate(capture1, templ, result, search_metod);
		normalize(result, result, 0, 1, NORM_MINMAX, -1, Mat());
		minMaxLoc(result, &minVal, &maxVal, &minLoc, &maxLoс, Mat());
		//rot_mat=getRotationMatrix2D(Point(templ.cols/2, templ.rows/2), angle, 1);
		//warpAffine(templ, templ, rot_mat, templ.size());
 	//	angle = angle + 1;
	/*}*/
	//double min=21360, max=-21360;
	//int index_of_min=0, index_of_max = 0;
	//for (int i = 0;i < full_angle;i++)
	//{
	//	if (minVal[i] <= min)
	//	{
	//		min = minVal[i];
	//		index_of_min = i;
	//	}
	//	if (maxVal[i] >= max)
	//	{
	//		max = maxVal[i];
	//		index_of_max = i;
	//	}
	//}

	if (search_metod == CV_TM_SQDIFF || search_metod == CV_TM_SQDIFF_NORMED)
	{
		matchLoc = minLoc;
	}
	else
	{
		matchLoc = maxLoс;
	}
	rectangle(capture1, matchLoc, Point(matchLoc.x + templ.cols, matchLoc.y + templ.rows), Scalar::all(0), 2, 8, 0);
	rectangle(result, matchLoc, Point(matchLoc.x + templ.cols, matchLoc.y + templ.rows), Scalar::all(0), 2, 8, 0);
	for (;;)
	{
		/*imshow("Image", result);*/
		addText(capture1,to_string(matchLoc.x)+", "+to_string(matchLoc.y),matchLoc,
		imshow("Capture", capture1);
		char c = waitKey(500);
		if (c == 27)
			break;
	}
}



int main()
{
	Mat frame;
	img = imread("C:\\dev\\MyProjects\\Work\\Example\\img005.tif", IMREAD_GRAYSCALE);
	namedWindow("Image", 1);
	namedWindow("Option", 1);
	setMouseCallback("Image", onMouse, 0);
	resize(img, img, Size(640, 480));
	for (;;)
	{
		img.copyTo(frame);
		if (selection.width > 0 && selection.height > 0 && flag)
		{
			Mat roi(frame, selection);
			bitwise_not(roi, roi);
		}
		imshow("Image", frame);
		char c = waitKey(50);
		if (c==27)
		{
			imwrite("C:\\dev\\MyProjects\\Work\\Example\\pattern.png", frame(selection));
			break;
		}
	}
	Mat pattern = imread("C:\\dev\\MyProjects\\Work\\Example\\pattern.png", IMREAD_GRAYSCALE);
	for(int i=1;i<4;i++)
	FindPattern(imread("C:\\dev\\MyProjects\\Work\\Example\\img005_"+to_string(i)+".tif", IMREAD_GRAYSCALE), pattern, 1, &img);
	waitKey();
	return 0;
}