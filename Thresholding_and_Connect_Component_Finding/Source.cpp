#include "stdafx.h"
#include "myHist.h"
#include "DisjointSet.h"
#include <iostream>
#include <vector>
#include <string>
#include <list>
#include <any>
int _tmain(int argc, _TCHAR* argv[]) {

	std::vector<int> RandomColor();

	cv::Mat mat = cv::imread("license_plate.bmp", 0); //grey-level image

	cv::namedWindow("license_plate", cv::WINDOW_AUTOSIZE);
	cv::imshow("license_plate", mat);
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
	for (int r = 0; r < mat.rows; r++) {
		for (int c = 0; c < mat.cols; c++) {
			int gray_value = mat.at<uchar>(r, c);
			in_hist[gray_value] = in_hist[gray_value] + 1;
		}
	}
	for (int i = 0; i < 256; i++) {
		in_hist[i] = in_hist[i] / (mat.rows * mat.cols);
	}


	float mu0, mu1;
	float P0 = 0.0;
	float P1 = 1.0;
	float Q0 = 0.0;
	float Q1 = 0.0;
	for (int i = 0; i < 256; i++) {
		Q1 += i * in_hist[i];
	}
	int t_star = 0;
	float max_bc_var = 0.0;

	for (int i = 0; i < 255; i++) {
		P0 = P0 + in_hist[i];
		P1 = 1.0 - P0;
		Q0 = Q0 + i * in_hist[i];
		Q1 = Q1 - i * in_hist[i];

		if (P0 <= 0.0 || P1 <= 0.0) {
			continue;
		}

		mu0 = Q0 / P0;
		mu1 = Q1 / P1;

		float bc_var = P0 * P1 * (mu0 - mu1) * (mu0 - mu1);
		if (bc_var >= max_bc_var) {
			t_star = i;
			max_bc_var = bc_var;
		}
	}

	printf("optimal threshold : %d", t_star);


		cv::Mat outmat = cv::Mat::zeros(mat.rows, mat.cols, CV_8UC1);

		for (int r = 0; r < mat.rows; r++) {
			for (int c = 0; c < mat.cols; c++) {
				if (mat.at<uchar>(r,c) <= t_star) {
					outmat.at<uchar>(r, c) = (uchar)255;
				}
				else {
					outmat.at<uchar>(r, c) = (uchar)0;
				}
			}

		}

		cv::namedWindow("license otsu", cv::WINDOW_AUTOSIZE);
		cv::imshow("license otsu", outmat);
		cv::waitKey(0);


		cv::Mat label_mat = cv::Mat::zeros(mat.rows, mat.cols, CV_8UC1);

		int new_label = 1;
		int upper_label, left_label;
		std::vector< std::vector< int> > equivalences;
		std::vector< int > aequivalences;

		for (int r = 1; r < outmat.rows; r++) {
			for (int c = 1; c < outmat.cols; c++) {
				if (outmat.at<uchar>(r, c) != 0) {
					std::vector< int > adjacent_labels;

					upper_label = label_mat.at<uchar>(r - 1, c);
					if (upper_label != 0) {
						adjacent_labels.push_back(upper_label);
					}
					left_label = label_mat.at<uchar>(r , c - 1);

					if (left_label != 0 && left_label != upper_label) {
						adjacent_labels.push_back(left_label);
					}
					if (adjacent_labels.size() == 0) {
						label_mat.at<uchar>(r, c) = (uchar)new_label;
						new_label = new_label + 1;
					}
					else {
						label_mat.at<uchar>(r, c) = (uchar)*min_element(adjacent_labels.begin(), adjacent_labels.end());
					}
					if (adjacent_labels.size() == 2) {
						aequivalences.push_back(adjacent_labels[0]);
						aequivalences.push_back(adjacent_labels[1]);
						//
						//aequivalences = { adjacent_labels[0],adjacent_labels[1] };
						equivalences.push_back(aequivalences);
					
					}
					
				}
			}

		}
		DisjointSet dis = DisjointSet();
		dis.makeSet(new_label);






		for (int i = 0; i < equivalences.size(); i++) {
			aequivalences = equivalences[i];
			dis.unionSet(aequivalences[0],aequivalences[1]);
		}



		int min_label;
		int temporal_label;
		for (int r = 1; r < outmat.rows; r++) {
			for (int c = 1; c < outmat.cols; c++) {
				if (label_mat.at<uchar>(r, c) != 0) {
					temporal_label = label_mat.at<uchar>(r, c);
					min_label = dis.find(temporal_label);
					label_mat.at<uchar>(r, c) = min_label;
			}
				
			}
		}

		
		
		std::vector< int > labels;
		labels.push_back(0);
		for (int r = 1; r < mat.rows; r++) {
			for (int c = 1; c < mat.cols; c++) {
				if (label_mat.at<uchar>(r, c) != 0) {
					
					if (std::find(labels.begin(), labels.end(), label_mat.at<uchar>(r, c)) == labels.end()) {


						labels.push_back((int)label_mat.at<uchar>(r, c));

					}
					auto it = find(labels.begin(), labels.end(), label_mat.at<uchar>(r, c));
					label_mat.at<uchar>(r, c) = it - labels.begin();
					
				}
			
			}
		}

		int n_regions = labels.size();
		printf("\nno.of regions : %d\n", n_regions);

		cv::Mat display_mat = cv::Mat::zeros(label_mat.rows, label_mat.cols, CV_8UC3);
		std::vector<std::vector< int > > color_plate;
		 
		 
		color_plate.push_back({ 255,255,255 });
		for (int i = 0; i < n_regions; i++) {
			int b, g, r;
			b = rand() % (255 - 0 + 1);
			g = rand() % (255 - 0 + 1);
			r = rand() % (255 - 0 + 1);
			color_plate.push_back({b,g,r});
		}
		
		

		for (int r = 1; r < mat.rows; r++) {
			for (int c = 1; c < mat.cols; c++) {
				std::vector< int > temp;

				int x = label_mat.at<uchar>(r, c);
				temp = color_plate[x];
				display_mat.at<cv::Vec3b>(r, c)[0] = temp[0];
				display_mat.at<cv::Vec3b>(r, c)[1] = temp[1];
				display_mat.at<cv::Vec3b>(r, c)[2] = temp[2];

			}
		}

		cv::namedWindow("license 2p", cv::WINDOW_AUTOSIZE);
		cv::imshow("license 2p", display_mat);
		cv::waitKey(0);

	return 0;




}
