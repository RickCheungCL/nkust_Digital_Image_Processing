// opencv_asg3.cpp : 此檔案包含 'main' 函式。程式會於該處開始執行及結束執行。
//

// opencv_asg2.cpp : 此檔案包含 'main' 函式。程式會於該處開始執行及結束執行。
//

#include "stdafx.h"
#include <iostream>
#include "myHist.h"

int _tmain(int argc, _TCHAR* argv[]) {
	cv::Mat mat = cv::imread("lena.bmp", 0); //grey-level image

	cv::namedWindow("Lena", cv::WINDOW_AUTOSIZE);
	cv::imshow("Lena", mat);
	cv::waitKey(0);
	double mean = 0;//boxfilter
	double final = 0;//boxfilter
	int row, col;
	int window[9];//median
	row = mat.rows;
	col = mat.cols;
	cv::Mat temp = cv::Mat::zeros(row-1, col-1, CV_8UC1);//boxfilter
	cv::Mat temp2 = cv::Mat::zeros(row - 1, col - 1, CV_8UC1);//median
	cv::Mat temp3 = cv::Mat::zeros(row - 1, col - 1, CV_8UC1);//Laplace

	for ( int a = 1; a < row - 1; a++) {
		for (int z = 1; z < col - 1; z++) {
			mean = 0.0;
			// kernal
			for (int x = -1; x <= 1; x++) {
				for (int y = -1; y <= 1; y++) {
					mean += mat.at<uchar>(a + x, z + y);
				}
			}

			mean = mean / (3 * 3);
			final = round(mean);

			temp.at<uchar>(a - 1, z - 1) = (uchar)final;
		}
	}
	cv::imshow("Lena box filter", temp);
	cv::waitKey(0);
	
	for (int y = 1; y < row - 1; y++) {
		for (int x = 1; x < col - 1; x++) {

			// Pick up window element

			window[0] = mat.at<uchar>(y - 1, x - 1);
			window[1] = mat.at<uchar>(y, x - 1);
			window[2] = mat.at<uchar>(y + 1, x - 1);
			window[3] = mat.at<uchar>(y - 1, x);
			window[4] = mat.at<uchar>(y, x);
			window[5] = mat.at<uchar>(y + 1, x);
			window[6] = mat.at<uchar>(y - 1, x + 1);
			window[7] = mat.at<uchar>(y, x + 1);
			window[8] = mat.at<uchar>(y + 1, x + 1);

			// sort the window to find median
			int temps;
			for (int k = 0;k < 9;k++) {
				temps = window[k];
				int j = k - 1;
				while (j >= 0 && temps <= window[j]) {
					window[j + 1] = window[j];
					j = j - 1;
				}
				window[j + 1] = temps;
			}

			// assign the median to centered element of the matrix
			temp2.at<uchar>(y, x) = (uchar)window[4];
		}
	}
	cv::imshow("Lena median filter", temp2);
	cv::waitKey(0);


	for (int y = 1; y < row - 1; y++) {
		for (int x = 1; x < col - 1; x++) {
			int sum = 5 * mat.at<uchar>(y, x) - mat.at<uchar>(y - 1, x)
				- mat.at<uchar>(y + 1, x)
				- mat.at<uchar>(y, x - 1)
				- mat.at<uchar>(y, x + 1)
				;

			temp3.at<uchar>(y, x) = cv::saturate_cast<uchar>(sum);
		}
	}
	cv::imshow("Lena Laplacian", temp3);
	cv::waitKey(0);

	cv::Mat out;
	cv::Laplacian(mat, out, CV_8UC1, 1);
	cv::imshow("Lena Laplacian2 using function laplacian", out);
	cv::waitKey(0);

	


	
	return 0;

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
