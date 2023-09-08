// opencv_asg2.cpp : 此檔案包含 'main' 函式。程式會於該處開始執行及結束執行。
//
#define _USE_MATH_DEFINES
#include "stdafx.h"
#include "myHist.h"
#include <iostream>
#include <time.h>
#include <math.h>
#include <algorithm>
#include <random>
#include <limits>

//cv::Mat addGaussianNoise(cv::Mat src, double mu, double sigma);
double gaussionNoiseSignal(double m, double sigma);
int StageA(int Smax, int Ksize, int x, int y, cv::Mat mat);
int StageB(int Ksize, int x, int y, cv::Mat mat);
int Median(int kernelWidth, int x, int y, cv::Mat mat);
int Min(int Ksize, int x, int y, cv::Mat mat);
int Max(int Ksize, int x, int y, cv::Mat mat);
cv::Mat adaptiveMedian(cv::Mat mat);

int _tmain(int argc, _TCHAR* argv[]) {

	srand(time(NULL));
	double m = 0;
	double sigma = 3;
	double var = sigma * sigma;

	int row, col;

	double p = 0.1;



	cv::Mat mat = cv::imread("DIP-HW04.bmp", 0); //grey-level image

	cv::namedWindow("Lena", cv::WINDOW_AUTOSIZE);
	cv::imshow("Lena", mat);
	cv::waitKey(0);
	row = mat.rows;
	col = mat.cols;

	cv::Mat temp = cv::Mat::zeros(row, col, CV_8UC1);
	cv::Mat temp2 = cv::Mat::zeros(row, col, CV_8UC1);
	cv::Mat temp3 = cv::Mat::ones(row, col, CV_8UC1);
	cv::Mat uniform = cv::Mat::zeros(row, col, CV_8UC1);

	cv::Mat ADM1(mat.rows, mat.cols, CV_8UC1);
	cv::Mat ADM2(mat.rows, mat.cols, CV_8UC1);
	cv::Mat ADM3(mat.rows, mat.cols, CV_8UC1);


	cv::Mat canvasInput;
	canvasInput.create(cv::Size(512, 512), CV_8UC1);
	// show the histogram of lena and after equalization
	myHist* pHist = new myHist(myHist::Type::Image);

	// calculate the histogram of lena 
	pHist->CalcHist(mat);
	canvasInput = cv::Scalar::all(0);
	pHist->Show(canvasInput);
	cv::namedWindow("input hist", cv::WINDOW_AUTOSIZE);
	cv::imshow("input hist", canvasInput);
	cv::waitKey(0);

	
	


	/*
	int PixcelMax = 255, PixcelMin = 0;
	for (int x = 0; x < mat.rows - 1; x++) {
		for (int y = 0; y < mat.cols; y++) {
			/*double T = mat.at<uchar>(x, y);
			temp.at<uchar>(x, y) = T + distri;
			if (temp.at<uchar>(x, y) > PixcelMax)
				temp.at<uchar>(x, y) = PixcelMax;
			else if (temp.at<uchar>(x, y) < PixcelMin)
				temp.at<uchar>(x, y) = PixcelMin;
			if (temp.at<uchar>(x + 1, y) > PixcelMax)
				temp.at<uchar>(x + 1, y) = PixcelMax;
			else if (temp.at<uchar>(x + 1, y) < PixcelMin)
				temp.at<uchar>(x + 1, y) = PixcelMin;
				//double R = ((double)rand() / (RAND_MAX));
				//double P = (1 / (sigma * sqrt(2 * M_PI))) * (exp(-(R - m) * (R - m)) / (2 * (var)));
			double t = mat.at<uchar>(x, y) + rand() / 256 * sqrt(sigma) + m;
			double U1 = ((double)rand() / (RAND_MAX));//uniform distribution1
			double U2 = ((double)rand() / (RAND_MAX));//uniform distribution2
			double	z1 = sigma * cos(2 * M_PI * U2) * sqrt(-2 * log(U1));//gau dis1
			double  z2 = sigma * sin(2 * M_PI * U2) * sqrt(-2 * log(U1));//gau dis2
			temp.at<uchar>(x, y) = mat.at<uchar>(x, y) + z1;
			temp.at<uchar>(x + 1, y) = mat.at<uchar>(x + 1, y) + z2;



		}
	}*/
	int grow = mat.rows;

	int gcol = mat.cols;
	if (mat.isContinuous()) {
		gcol *= grow;
		grow = 1;
	}
	for (int i = 0; i < grow; i++)
	{
		for (int j = 0; j < gcol; j++)
		{
			int val = mat.ptr<uchar>(i)[j] + gaussionNoiseSignal(m, sigma);
			if (val < 0) {
				val = 0;
			}
			if (val > 255) {
				val = 255;
			}
			temp.ptr<uchar>(i)[j] = (uchar)val;
		}
	}
	
	cv::imshow("gau", temp);
	cv::waitKey(0);

	for (int x = 0; x < row; x++) {
		for (int y = 0; y < col; y++) {
			temp2.at<uchar>(x, y) = mat.at<uchar>(x, y);
		}
	}
		for (int x = 0; x < row; x++) {
			for (int y = 0; y < col; y++) {
				float r = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
				if (r < 2*p) {
					float r = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
					if (r < 0.5)
						temp2.at<uchar>(x, y) = 255;//salt
					else
						temp2.at<uchar>(x, y) = 0;


				}
			}
		}


		cv::imshow("salt & pepper", temp2);
		cv::waitKey(0);

		/*int a = -10;
		int b = 10;
		for (int x = 0; x < row; x++) {
			for (int z = 0; z < col; z++) {
				int uni = a + (b-a) * (rand() % (256));
				temp3.at<uchar>(x, z) = mat.at<uchar>(x, z) + uni;
			}
		}
		*/

		int a = -10;
		int b = 10;
		std::default_random_engine generator;
		generator.seed(1997);
		std::uniform_int_distribution<int> unif(a,b) ;
		for (int y = 0; y < row; y++)
		{
			for (int x = 0; x < col; x++)
			{	
				int ra = unif(generator) ;
				
				int val = mat.ptr<uchar>(y)[x] + ra;
				if (val < 0) {
					val = 0;
				}
				if (val > 255) {
					val = 255;
				}
				temp3.ptr<uchar>(y)[x] = (uchar)val;
			}
		}
		
		cv::imshow("uniform", temp3);
		cv::waitKey(0);


		cv::Mat canvasInput3;
		canvasInput3.create(cv::Size(512, 512), CV_8UC1);
		// show the histogram of lena and after equalization
		myHist* pHist3 = new myHist(myHist::Type::Image);

		// calculate the histogram of lena 
		pHist3->CalcHist(temp);
		canvasInput3 = cv::Scalar::all(0);
		pHist3->Show(canvasInput3);
		cv::namedWindow("temp", cv::WINDOW_AUTOSIZE);
		cv::imshow("temp", canvasInput3);
		cv::waitKey(0);






		cv::Mat canvasInput4;
		canvasInput4.create(cv::Size(512, 512), CV_8UC1);
		// show the histogram of lena and after equalization
		myHist* pHist4 = new myHist(myHist::Type::Image);

		// calculate the histogram of lena 
		pHist4->CalcHist(temp2);
		canvasInput4 = cv::Scalar::all(0);
		pHist4->Show(canvasInput4);
		cv::namedWindow("temp2", cv::WINDOW_AUTOSIZE);
		cv::imshow("temp2", canvasInput4);
		cv::waitKey(0);




		cv::Mat canvasInput5;
		canvasInput5.create(cv::Size(512, 512), CV_8UC1);
		// show the histogram of lena and after equalization
		myHist* pHist5 = new myHist(myHist::Type::Image);

		// calculate the histogram of lena 
		pHist5->CalcHist(temp3);
		canvasInput5 = cv::Scalar::all(0);
		pHist5->Show(canvasInput5);
		cv::namedWindow("temp3", cv::WINDOW_AUTOSIZE);
		cv::imshow("temp3", canvasInput5);
		cv::waitKey(0);




		
		ADM1 = adaptiveMedian(temp);
		cv::imshow("adm gau", ADM1);
		cv::waitKey(0);
		ADM2 = adaptiveMedian(temp2);
		cv::imshow("adm salt and pepper", ADM2);
		cv::waitKey(0);
		ADM3 = adaptiveMedian(temp3);
		cv::imshow("adm uniform", ADM3);
		cv::waitKey(0);

		cv::Mat canvasInput6;
		canvasInput6.create(cv::Size(512, 512), CV_8UC1);
		// show the histogram of lena and after equalization
		myHist* pHist6 = new myHist(myHist::Type::Image);

		// calculate the histogram of lena 
		pHist6->CalcHist(ADM1);
		canvasInput6 = cv::Scalar::all(0);
		pHist6->Show(canvasInput6);
		cv::namedWindow("ADM1", cv::WINDOW_AUTOSIZE);
		cv::imshow("ADM1", canvasInput6);
		cv::waitKey(0);

		cv::Mat canvasInput7;
		canvasInput7.create(cv::Size(512, 512), CV_8UC1);
		// show the histogram of lena and after equalization
		myHist* pHist7 = new myHist(myHist::Type::Image);

		// calculate the histogram of lena 
		pHist7->CalcHist(ADM2);
		canvasInput7 = cv::Scalar::all(0);
		pHist7->Show(canvasInput7);
		cv::namedWindow("ADM2", cv::WINDOW_AUTOSIZE);
		cv::imshow("ADM2", canvasInput7);
		cv::waitKey(0);

		cv::Mat canvasInput8;
		canvasInput8.create(cv::Size(512, 512), CV_8UC1);
		// show the histogram of lena and after equalization
		myHist* pHist8 = new myHist(myHist::Type::Image);

		// calculate the histogram of lena 
		pHist8->CalcHist(ADM3);
		canvasInput8 = cv::Scalar::all(0);
		pHist8->Show(canvasInput8);
		cv::namedWindow("ADM3", cv::WINDOW_AUTOSIZE);
		cv::imshow("ADM3", canvasInput8);
		cv::waitKey(0);

		return 0;

	}

	cv::Mat adaptiveMedian(cv::Mat mat) {
		cv::Mat out(mat.rows, mat.cols, CV_8UC1);

		int Smax = 15;

		// apply filter for each pixel of the image
		for (int i = 0; i < mat.rows; i++)
			for (int j = 0; j < mat.cols; j++)
				out.at<uchar>(i, j) = StageA(Smax, 1, i, j, mat);


		return out;
	}
	
	double gaussionNoiseSignal(double m, double sigma) {
		const double epsilon = std::numeric_limits<double>::min();
		static double z0, z1;
		static bool flag = false;
		flag = !flag;

		if (!flag) {
			return z1 * sigma + m;
		}
		double u1, u2;
		do {
			u1 = rand() * (1.0 / RAND_MAX);
			u2 = rand() * (1.0 / RAND_MAX);
		} while (u1 <= epsilon);
		z0 = sqrt(-2.0 * log(u1)) * cos(2 * CV_PI * u2);
		z1 = sqrt(-2.0 * log(u1)) * sin(2 * CV_PI * u2);
		return z0 * sigma + m;
	}

	/*cv::Mat addGaussianNoise(cv::Mat mat, double m, double sigma) {
		cv::Mat out = mat.clone();
		int row = out.rows;

		int col = out.cols ;
		if (out.isContinuous()) {
			col *= row;
			row = 1;
		}
		for (int i = 0; i < row; i++)
		{
			for (int j = 0; j < col; j++)
			{
				int val = out.ptr<uchar>(i)[j] + gaussionNoiseSignal(m, sigma) ;
				if (val < 0) {
					val = 0;
				}
				if (val > 255) {
					val = 255;
				}
				out.ptr<uchar>(i)[j] = (uchar)val;
			}
		}
		return out;
	}
	*/


int StageA(int Smax, int Ksize, int x, int y, cv::Mat mat)
{
	int a1 = Median(Ksize, x, y, mat) - Min(Ksize, x, y, mat), a2 = Median(Ksize, x, y, mat) - Max(Ksize, x, y, mat);

	if (a1 > 0 && a2 < 0)
		return StageB(Ksize, x, y, mat);
	else {
		if (Ksize == Smax)
			return Median(Ksize, x, y, mat);
		else
			return StageA(Smax, Ksize + 2, x, y, mat);
	}


}
int StageB(int Ksize, int x, int y, cv::Mat mat)
{
	int b1 = (int)mat.at<uchar>(x, y) - Min(Ksize, x, y, mat), b2 = (int)mat.at<uchar>(x, y) - Max(Ksize, x, y, mat);

	if (b1 > 0 && b2 < 0)
		return (int)mat.at<uchar>(x, y);
	else
		return Median(Ksize, x, y, mat);
}

int Median(int Ksize, int x, int y, cv::Mat mat)
{
	int* values = new int[Ksize * Ksize];
	int temp = 0;

	// gather the values that lying in the window
	for (int i = -Ksize / 2; i <= Ksize / 2; i++)
		for (int j = -Ksize / 2; j <= Ksize / 2; j++, temp++)
			if (x + i < 0 || y + j < 0 || x + i > mat.rows - 1 || y + j > mat.cols - 1)
				values[temp] = 0;
			else
				values[temp] = mat.at<uchar>(x + i, y + j);


	// sort the values
	std::sort(values, values + Ksize * Ksize);

	int retureV = values[Ksize * Ksize / 2];
	delete[] values;
	return retureV;
}

int Min(int Ksize, int x, int y, cv::Mat mat)
{
	int* values = new int[Ksize * Ksize];
	int temp = 0;

	// gather the values that lying in the window
	for (int i = -Ksize / 2; i <= Ksize / 2; i++)
		for (int j = -Ksize / 2; j <= Ksize / 2; j++, temp++)
			if (x + i < 0 || y + j < 0 || x + i > mat.rows - 1 || y + j > mat.cols - 1)
				values[temp] = 0;
			else
				values[temp] = mat.at<uchar>(x + i, y + j);

	// sort the values
	std::sort(values, values + Ksize * Ksize);

	int retureV = values[0];
	delete[] values;
	return retureV;
}

int Max(int Ksize, int x, int y, cv::Mat mat)
{
	int* values = new int[Ksize * Ksize];
	int temp = 0;

	// gather the values that lying in the window
	for (int i = -Ksize / 2; i <= Ksize / 2; i++)
		for (int j = -Ksize / 2; j <= Ksize / 2; j++, temp++)
			if (x + i < 0 || y + j < 0 || x + i > mat.rows - 1 || y + j > mat.cols - 1)
				values[temp] = 0;
			else
				values[temp] = mat.at<uchar>(x + i, y + j);

	// sort the values
	std::sort(values, values + Ksize * Ksize);

	int returnV = values[Ksize * Ksize - 1];
	delete[] values;

	return returnV;
}
// 執行程式: Ctrl + F5 或 [偵錯] > [啟動但不偵錯] 功能表
// 偵錯程式: F5 或 [偵錯] > [啟動偵錯] 功能表

// 開始使用的提示: 
//   1. 使用 [方案總管] 視窗，新增/管理檔案
//   2. 使用 [Team Explorer] 視窗，連線到原始檔控制
//   3. 使用 [輸出] 視窗，參閱組建輸出與其他訊息
//   4. 使用 [錯誤清單] 視窗，檢視錯誤
//   5. 前往 [專案] > [新增項目]，建立新的程式碼檔案，或是前往 [專案] > [新增現有項目]，將現有程式碼檔案新增至專案
//   6. 之後要再次開啟此專案時，請前往 [檔案] > [開啟] > [專案]，然後選取 .sln 檔案
