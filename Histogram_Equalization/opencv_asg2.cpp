// opencv_asg2.cpp : 此檔案包含 'main' 函式。程式會於該處開始執行及結束執行。
//

#include "stdafx.h"
#include "myHist.h"
#include <iostream>


int _tmain(int argc, _TCHAR* argv[]) {
	cv::Mat mat = cv::imread("lena.bmp", 0); //grey-level image

	cv::namedWindow("Lena", cv::WINDOW_AUTOSIZE);
	cv::imshow("Lena", mat);
	cv::waitKey(0);

	cv::Mat target = cv::imread("license_plate.bmp", 0);
	cv::namedWindow("license_plate", cv::WINDOW_AUTOSIZE);
	cv::imshow("license_plate", target);
	cv::waitKey(0);


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


	
	float in_hist[256]{};
	for (int r = 0; r < mat.rows;r++) {
		for (int c = 0; c < mat.cols;c++) {
			int gray_value = mat.at<uchar>(r, c);
			in_hist[gray_value] = in_hist[gray_value] + 1;
			
		}
		
	}
	for (int i = 0;i < 256;i++) {
		in_hist[i] = in_hist[i] / (mat.rows *  mat.cols);
	}
	int T[256]{};
	float acc_pr = 0.0;
	for (int x = 0;x < 256;x++) {
		acc_pr = acc_pr + in_hist[x];
		T[x] = int(255.0 * acc_pr);
	}
	cv::Mat out_mat;
	out_mat.create(cv::Size(512, 512), CV_8UC1);
	for (int r = 0; r < mat.rows;r++) {
		for (int c = 0; c < mat.cols;c++) {
			int gray_value = mat.at<uchar>(r, c);
			int out_value = T[gray_value];
			out_mat.at<uchar>(r, c) =out_value;
		}
		
	}
	cv::imshow("lena with histogram equalization not using equalizeHist", out_mat);
	cv::waitKey(0);

	cv::Mat canvasInput3;
	canvasInput3.create(cv::Size(512, 512), CV_8UC1);
	// show the histogram of lena and after equalization
	myHist* pHist3 = new myHist(myHist::Type::Image);

	// calculate the histogram of lena 
	pHist3->CalcHist(out_mat);
	canvasInput3 = cv::Scalar::all(0);
	pHist3->Show(canvasInput3);
	cv::namedWindow("input hist (not using equalizeHist)", cv::WINDOW_AUTOSIZE);
	cv::imshow("input hist (not using equalizeHist)", canvasInput3);
	cv::waitKey(0);

	


	

	cv::Mat canvasInput4;
	canvasInput4.create(cv::Size(512, 512), CV_8UC1);
	// show the histogram of lena and after equalization
	myHist* pHist4 = new myHist(myHist::Type::Image);

	// calculate the histogram of lena 
	pHist4->CalcHist(target);
	canvasInput4 = cv::Scalar::all(0);
	pHist4->Show(canvasInput4);
	cv::namedWindow("license hist", cv::WINDOW_AUTOSIZE);
	cv::imshow("license hist", canvasInput4);
	cv::waitKey(0);


	float tar_hist[256]{};
	for (int r = 0; r < target.rows;r++) {
		for (int c = 0; c < target.cols;c++) {
			int gray_value = target.at<uchar>(r, c);
			tar_hist[gray_value] = tar_hist[gray_value] + 1;
		}
	}
	for (int i = 0;i < 256;i++) {
		tar_hist[i] = tar_hist[i] / (target.rows * target.cols);
	}
	int G[256]{};
	float acc_pr2 = 0.0;
	for (int x = 0;x < 256;x++) {
		acc_pr2 = acc_pr2 + tar_hist[x];
		G[x] = int(255.0 * acc_pr2);
	}

	int G_inv[256]{};
	for (int y = 0;y < 256;y++) {
		G_inv[y] = -1;
		for (int x = 0;x < 256;x++) {
			if (G[x] == y) {
				G_inv[y] = x;
				break;
			}
				
		}
	}
	for (int y = 0;y < 256;y++) {
		if (G_inv[y] == -1) {
			for (int offset = 0;offset < 256;offset++) {
				if ((y - offset) >= 0 && G_inv[y - offset] != -1) {
					G_inv[y] = G_inv[y - offset];
					break;
				}
				if ((y + offset) < 255 && G_inv[y + offset] != -1) {
					G_inv[y] = G_inv[y + offset];
					break;
				}
			}
		}
		

	}


	int Z_hist[256]{};
	for (int x = 0;x < 256;x++) {
		int y = T[x];
		Z_hist[x] = G_inv[y];
	}
	
	cv::Mat out_mat2;
	out_mat2.create(cv::Size(512, 512), CV_8UC1);
	for (int r = 0; r < mat.rows;r++) {
		for (int c = 0; c < mat.cols;c++) {
			int gray_value = mat.at<uchar>(r, c);
			int out_value = Z_hist[gray_value];
			out_mat2.at<uchar>(r, c) = out_value;
		}

	}
	cv::imshow("lena with histogram matching with license", out_mat2);
	cv::waitKey(0);
	cv::Mat canvasInput5;
	canvasInput5.create(cv::Size(512, 512), CV_8UC1);
	// show the histogram of lena and after equalization
	myHist* pHist5 = new myHist(myHist::Type::Image);

	// calculate the histogram of lena 
	pHist5->CalcHist(out_mat2);
	canvasInput5 = cv::Scalar::all(0);
	pHist5->Show(canvasInput5);
	cv::namedWindow("lena license hist", cv::WINDOW_AUTOSIZE);
	cv::imshow("lena license hist", canvasInput5);
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
