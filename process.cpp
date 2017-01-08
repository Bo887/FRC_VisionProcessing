#include <vector>
#include <string>
#include <iostream>
#include <chrono>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
//hsv values
//need to tune
int hsv_low[] = {30,100,100};
int hsv_high[] = {90,255,255};
//main method
int main(int argc, char** argv){
	int cam_port;
	//default camera port is 0 currently (/dev/video0) if there is no argument
	if (argc==1) cam_port = 0; 
	//otherwise the port is what is specified in the argument
	else cam_port = std::stoi(argv[1]);
	//camera input
	cv::VideoCapture cap(cam_port);
	std::cout << "INITIALLIZING" << std::endl;
	//variables for calculating the dt (how many milliseconds each loop takes)
	std::chrono::steady_clock::time_point start_timestamp, end_timestamp;
	std::chrono::milliseconds dt;
	while(cap.isOpened()){
		start_timestamp = std::chrono::steady_clock::now();
		//get current timestamp
		//3 image containers for camera feed, hsv, and filtered hsv
		static cv::Mat raw_input;		
		static cv::Mat hsv;		
		static cv::Mat threshhold;
		//take in camera input into the first mat
		cap >> raw_input;
		//convert the raw feed into hsv and store in hsv mat
		cv::cvtColor(raw_input,hsv,CV_RGB2HSV);	
		//filter the hsv for the hsv constants and store into the threshhold mat
		cv::inRange(hsv,cv::Scalar(hsv_low[0],hsv_low[1],hsv_low[2]),cv::Scalar(hsv_high[0],hsv_high[1],hsv_high[2]),threshhold);
		//show the camera output	
		cv::imshow("camera input",raw_input);
		cv::imshow("hsv filter", hsv);
		cv::imshow("threshhold", threshhold);
		end_timestamp = std::chrono::steady_clock::now();
		//difference in time (end time - start time)
		dt = std::chrono::duration_cast<std::chrono::milliseconds>(end_timestamp-start_timestamp);
		std::cout << "RUNNING\tDT (ms): " << dt.count() << std::endl;
		if (cv::waitKey(50)>=0) break;
	}
	std::cout << "DONE" << std::endl;
	return 0;
}
