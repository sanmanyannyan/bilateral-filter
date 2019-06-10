#include <iostream>
#include <fstream>
#include <string>
#include <chrono>

#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

#include "bilateral-filter.h"

size_t bilateral_filter_iterations = 5;
int bilateral_filter_diameter = 5;
double bilateral_filter_sigma_color = 80.0;
double bilateral_filter_sigma_space = 80.0;

bool bench_opencv(std::string destination_image_file_path, std::string source_image_file_path){
	cv::Mat src;
	cv::Mat dst;
	cv::Mat buff;
	src = cv::imread(source_image_file_path.c_str());
	dst = src.clone();
	buff = src.clone();
	if(!src.data){
		std::cerr << "Could not open image file at " << source_image_file_path << "." << std::endl;
		return false;
	}

	auto t1 = std::chrono::high_resolution_clock::now();
	for(size_t iter = 0 ; iter < bilateral_filter_iterations ; ++iter){
		if(iter & 1) {
			cv::bilateralFilter(dst, buff, bilateral_filter_diameter, bilateral_filter_sigma_color, bilateral_filter_sigma_space);
		} else {
			cv::bilateralFilter(buff, dst, bilateral_filter_diameter, bilateral_filter_sigma_color, bilateral_filter_sigma_space);
		}
	}
	auto t2 = std::chrono::high_resolution_clock::now();
	auto exec_time = std::chrono::duration_cast< std::chrono::duration<double, std::milli> >(t2 - t1);
	std::cout << "exec time(ms):" << exec_time.count() << std::endl;
	if (bilateral_filter_iterations & 1){
		cv::imwrite(destination_image_file_path.c_str(), dst);
	} else {
		cv::imwrite(destination_image_file_path.c_str(), buff);
	}
	return true;
}

int main(int argc,char *argv[]){
	bench_opencv("test1.opencv.jpg", "test1.jpg");
	return 0;
}

