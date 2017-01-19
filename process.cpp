#include <vector>
#include <string>
#include <iostream>
#include <chrono>
#include <algorithm>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
//hsv values
int hsv_low[] = {30,100,100};
int hsv_high[] = {90,255,255};
//main method
int main(int argc, char** argv){
	int cam_port;
	//default camera port is 1 currently (/dev/video1) if there is no argument
	if (argc==1) cam_port = 1; 
	//otherwise the port is what is specified in the single argument
	else cam_port = std::stoi(argv[1]);
	//camera input
	cv::VideoCapture cap(cam_port);
	std::cout << "INITIALLIZING" << std::endl;
	//variables for calculating the dt (how many milliseconds each loop takes)
	std::chrono::steady_clock::time_point start_timestamp, end_timestamp;
	std::chrono::milliseconds dt;
	//vision loop
	while(true/*cap.isOpened()*/){
		start_timestamp = std::chrono::steady_clock::now();
		//get current timestamp
		//3 image containers for camera feed, hsv, and filtered hsv
		static cv::Mat frame;
		static cv::Mat hsv;		
		static cv::Mat mask;
		//take in camera input into the first mat
		frame = cv::imread("./TestImages/1ftH4ftD0Angle0Brightness.jpg",CV_LOAD_IMAGE_COLOR);//cap >> frame;
		//convert the raw feed into hsv and store in hsv mat
		cv::cvtColor(frame,hsv,CV_RGB2HSV);	
		//filter the hsv for the hsv constants and store into the threshholded mask
		cv::inRange(hsv,cv::Scalar(hsv_low[0],hsv_low[1],hsv_low[2]),cv::Scalar(hsv_high[0],hsv_high[1],hsv_high[2]),mask);
		//vector to store the contours
		std::vector<std::vector<cv::Point> > contours;
		//find all the contours and store them in the vector
		cv::findContours(mask, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_TC89_KCOS);	
		//now we have to filter all the contours to find the largest two
		double max_contour_index_1, max_contour_index_2;
		double max_area = 0;
		//find largest contour index (index is needed to display a specific contour later)
		for(int i=0;i<contours.size();i++){
			double area = cv::contourArea(contours[i], false);	
			if (area > max_area){
				max_area = area;
				max_contour_index_1 = i;
			}
		}	
		//reset variables and find largest contour that is not the previous one found
		max_area = 0;
		for(int i=0;i<contours.size();i++){
			double area = cv::contourArea(contours[i], false);
			if (area > max_area && i != max_contour_index_1){
				max_area = area;
				max_contour_index_2 = i;
			}
		}
		//draw the two largest contours found using the contour indexces
		cv::drawContours(frame, contours, max_contour_index_1, (0,0,255),4, 0);
		cv::drawContours(frame, contours, max_contour_index_2, (0,0,255),4, 0);
		//show the camera output	
		cv::imshow("frame", frame);
		cv::imshow("hsv filter", hsv);
		cv::imshow("mask", mask);

		end_timestamp = std::chrono::steady_clock::now();
		//difference in time (end time - start time)
		dt = std::chrono::duration_cast<std::chrono::milliseconds>(end_timestamp-start_timestamp);
		std::cout << "RUNNING\tDT (ms): " << dt.count() << std::endl;
		if (cv::waitKey(50)>=0) break;
	}
	std::cout << "DONE" << std::endl;
	return 0;
}
