#include<opencv2/core/core.hpp>
#include<opencv2/highgui/highgui.hpp>
#include<opencv2/imgproc/imgproc.hpp>
#include<iostream>
#include <stdio.h>
#include "opencv2/opencv.hpp"
#include <wiringPi.h>
#include <softPwm.h>
using namespace std;
using namespace cv;
/*
TRACK A YELLOW BALL - OBJECT DETECTION METHOD USING COLOR SEPERATION OPEN CV 3.1.0
author - Rachit Gulati
*/

int main() {
	wiringPiSetup();
	pinMode(1, OUTPUT);
	pinMode(4, OUTPUT);
	pinMode(5, OUTPUT);
	pinMode(6, OUTPUT);
	pinMode(29, OUTPUT);
	pinMode(28, OUTPUT);
	softPwmCreate(1, 0, 100);	
	softPwmCreate(28, 0, 100);
	double x;
//	printf("\n....");
	cv::VideoCapture capWebcam(0);		// declare a VideoCapture object to associate webcam, 0 means use 1st (default) webcam
	printf ("\nstart\n");
	printf("nn");
//	if (capWebcam.isOpened() == false)	 //  To check if object was associated to webcam successfully
//	{				
//		std::cout << "error: Webcam connect unsuccessful\n";	// if not then print error message
///		return(0);												// and exit program
//	}
	printf ("<<<<<");
	cv::Mat imgOriginal;		// Input image
	cv::Mat hsvImg;				// HSV Image
	cv::Mat threshImg;			// Thresh Image
	
	std::vector<cv::Vec3f> v3fCircles;		// 3 element vector of floats, this will be the pass by reference output of HoughCircles()
	std::vector<std::vector<cv::Point>> contours;
	std::vector<cv::Vec4i> hierarchy;
//	char charCheckForEscKey = 0;

	int lowH = 0;							// Set Hue
	int highH = 70;

	int lowS = 100;							// Set Saturation
	int highS = 255;

	int lowV = 150;							// Set Value
	int highV = 255;
	// HUE for YELLOW is 21-30.
	// Adjust Saturation and Value depending on the lighting condition of the environment as well as the surface of the object.
//	printf ("><");
	while ( capWebcam.isOpened()) {				// until the Esc is pressed or webcam connection is lost
		bool blnFrameReadSuccessfully = capWebcam.read(imgOriginal);		// get next frame
		
		if (!blnFrameReadSuccessfully || imgOriginal.empty()) {				// if frame read unsuccessfully
			std::cout << "error: frame can't read \n";						// print error message
			break;															// jump out of loop
		}
		resize(imgOriginal, imgOriginal, cv:: Size(100, 100), 0, 0 );
	//	imshow("axe", imgOriginal); 
		cv::cvtColor(imgOriginal, hsvImg, CV_BGR2HSV);						// Convert Original Image to HSV Thresh Image

		cv::inRange(hsvImg, cv::Scalar(lowH, lowS, lowV), cv::Scalar(highH, highS, highV), threshImg);

		cv::GaussianBlur(threshImg, threshImg, cv::Size(3, 3), 0);			//Blur Effect
		cv::dilate(threshImg, threshImg, 0);								// Dilate Filter Effect
		cv::erode(threshImg, threshImg, 0);									// Erode Filter Effect
		
		// fill circles vector with all circles in processed image
		// cv::HoughCircles(threshImg,v3fCircles,CV_HOUGH_GRADIENT,2,threshImg.rows / 4,100,50,10,800);  // algorithm for detecting circles		
		cv::HoughCircles(threshImg, v3fCircles, CV_HOUGH_GRADIENT, 1, threshImg.rows/8, 200, 20, 0, 0 );
                Mat kernel = getStructuringElement(MORPH_ELLIPSE,Size(9,9));
                morphologyEx(threshImg,threshImg,MORPH_OPEN,kernel);	
		
		double Area = 1; 
		
		findContours ( threshImg, contours,hierarchy, RETR_EXTERNAL, CV_CHAIN_APPROX_NONE );
		if ( contours.size()>0 )Area = contourArea( contours[0] );
//	cout << "Area ="<< Area << endl;
//	if(Area > 0 )cout << "perc = " << (Area) / 10  << "\n";
		if ( Area / 100 > 20 ) { 
				softPwmWrite(28, 50);  
                                softPwmWrite(1, 50);
                                digitalWrite(29,1);
                                digitalWrite(6, 1);
                                digitalWrite(5, 1);
                                digitalWrite(4, 1);

				continue;
		}
	
	int sumx = 0;
	int sumy = 0;
	if ( contours.size() != 0 ) {
		for ( int i = 0; i < contours[0].size();i++ ) {
			sumx += contours [0][i].x; 
	//		sumy += contours [0][i].y;
		}
		sumx /= contours [0].size();
	//	sumy /= contours [0].size();
		x = sumx;
	}
	if ( contours.size() == 0) {
		if ( x < 30 ) {
		softPwmWrite(28,45);  
                                softPwmWrite(1, 14);
                                digitalWrite(29, 1);
                                digitalWrite(6, 0);
                                digitalWrite(5, 0);
                                digitalWrite(4, 1);

		}
		if ( x > 70 ) {
		
		softPwmWrite(28, 14);  
                                softPwmWrite(1, 45);
                                digitalWrite(29, 1);
                                digitalWrite(6, 0);
                                digitalWrite(5, 0);
                                digitalWrite(4, 1);
		}
		
		continue;
	}
//	for (int i = 0; i < v3fCircles.size(); i++) {						// for each circle
			// std::cout << "Ball position X = "<< v3fCircles[i][0] << "\n";												
			// std::cout << "Ball position X = "<< v3fCircles[i][0]			// x position of center point of circle
			// 	<<",\tY = "<< v3fCircles[i][1]								// y position of center point of circle
			// 	<<",\tRadius = "<< v3fCircles[i][2]<< "\n";					// radius of circle
		//	cout << "<>\n";
			// std::cout << "1"<< "\n";			
																// draw small green circle at center of object detected
		//	cv::circle(imgOriginal,												// draw on original image
		//		cv::Point((int)v3fCircles[i][0], (int)v3fCircles[i][1]),		// center point of circle
		//		3,																// radius of circle in pixels
		//		cv::Scalar(0, 255, 0),											// draw green
		//		CV_FILLED);														// thickness

																				// draw red circle around object detected 
		//	cv::circle(imgOriginal,												// draw on original image
		//		cv::Point((int)v3fCircles[i][0], (int)v3fCircles[i][1]),		// center point of circle
		//		(int)v3fCircles[i][2],											// radius of circle in pixels
		//		cv::Scalar(0, 0, 255),											// draw red
		//		3);	
																			// thickness
//			 cout << x<< "\n";
					
		//	x = v3fCircles[i][0];
//			y = v3fCircles [ i ] [ 1 ]; 
//			cout << x << "\n"; 		
//			if ( x >45 && x < 55 && y > 45 && y < 55 ) {
//				digitalWrite(29, 1);
  //                              digitalWrite(6, 1);
    //                            digitalWrite(5, 1);
      //                          digitalWrite(4, 1);
	//		}
			int left = 40, right = 60;
			if(x < right && x > left){
//				std::cout << "boro jolo   " << x << "\n";
				softPwmWrite(28, 100);	
				softPwmWrite(1, 100);
				digitalWrite(29, 1);
				digitalWrite(6, 0);
				digitalWrite(5, 0);
				digitalWrite(4, 1);
			}
			else if(x <= left){
//				std::cout << "boro rast   " << x << "\n";
				softPwmWrite(28,100);	
				softPwmWrite(1, 0);
				digitalWrite(29, 1);
				digitalWrite(6, 0);
				digitalWrite(4, 1);
				digitalWrite(5, 0);
			}
			else if(x > right){
//				std::cout << "boro chap   " << x << "\n";
				softPwmWrite(28, 0);	
				softPwmWrite(1, 100);
				digitalWrite(29, 1);
				digitalWrite(6, 0);
				digitalWrite(4, 1);
				digitalWrite(5, 0);
			}
//		}	
		
		// declare windows
//		cv::namedWindow("imgOriginal", CV_WINDOW_AUTOSIZE);
//		cv::namedWindow("threshImg", CV_WINDOW_AUTOSIZE);
//		cv::resizeWindow("threshImg", 100, 100);	
   //             cv::resizeWindow("imgOriginal", 100, 100);
//	    /* Create trackbars in "threshImg" window to adjust according to object and environment.*/
//		cv::createTrackbar("LowH", "threshImg", &lowH, 179);	//Hue (0 - 179)
//		cv::createTrackbar("HighH", "threshImg", &highH, 179);

//		cv::createTrackbar("LowS", "threshImg", &lowS, 255);	//Saturation (0 - 255)
//		cv::createTrackbar("HighS", "threshImg", &highS, 255);

//		cv::createTrackbar("LowV", "threshImg", &lowV, 255);	//Value (0 - 255)
//		cv::createTrackbar("HighV", "threshImg", &highV, 255);

//		Mat kernel = getStructuringElement(MORPH_ELLIPSE,Size(9,9));
//		morphologyEx(threshImg,threshImg,MORPH_OPEN,kernel);

		cv::imshow("imgOriginal", imgOriginal);					// show windows
		cv::imshow("threshImg", threshImg);
 		waitKey(1);
//		charCheckForEscKey = cv::waitKey(1);					// delay and get key press
	}
	return(0);											
}
