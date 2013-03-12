#include <iostream>
#include <string>
#include <vector>
#include <typeinfo>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/features2d/features2d.hpp>
#include <opencv2/video/tracking.hpp>
#include "FeatureTracker.h"
using namespace cv;
using namespace std;

// Constructor
FeatureTracker::FeatureTracker ()
{
	max_count = 500;
	qlevel = 0.01;
	minDist = 10;
}

// Function which processes the frames
void FeatureTracker::process (Mat& frame, Mat& output)
{
	result[0] = 0; // Reset the left count
	result[1] = 0; // and right count to zero
	cvtColor (frame, gray, CV_BGR2GRAY); // Convert to grayscale
	frame.copyTo (output);
	if (addNewPoints ()) // Do we need more points?
	{
		detectFeaturePoints ();
		points[0].insert (points[0].end (), features.begin (), features.end ()); // Add detected points to the current vector which is storing it
		initial.insert (initial.end (), features.begin (), features.end ());
	}
	// First frame needs initialisation
	if (gray_prev.empty ())
	{
		gray.copyTo (gray_prev);
	}
	calcOpticalFlowPyrLK (gray_prev, gray, points[0], points[1], status, err); // Track the features
	// gray_prev and gray - compare these 2 frames; points[0]: input; points[1]: output; status: success??; err: error?
	int k = 0;
	// Reject the unnecessary points - quality control
	for (int i = 0; i < points[1].size (); i++)
	{
		if (acceptTrackedPoint (i)) // Is it ok?
		{
			initial[k] = initial[i]; // Keep it
			points[1][k++] = points[1][i];
		}
	}
	points[1].resize (k); // Resize the vectors
	initial.resize (k);
	handleTrackedPoints (frame, output); // Process these points
	if (result[0] != 0 && result[1] != 0) // If there has been a movement -->
	{
		if (result[0] > result[1]) // Left?
		{
			cout << "Object has moved left\n";
		}
		else if (result[0] < result[1]) // Right?
		{
			cout << "Object has moved right\n";
		}
		else
		{
			cout << "Inconclusive\n";
		}
		cout << "Left count: " << result[0] << endl;
		cout << "Right count: " << result[1] << endl;
	}
	std::swap (points[1], points[0]); // Present becomes the old for the next iteration
	cv::swap (gray_prev, gray);
}

// Detecting the points
void FeatureTracker::detectFeaturePoints ()
{
	goodFeaturesToTrack (gray, features, max_count, qlevel, minDist); //In-built function of OpenCV
	// gray: the current frame; features: output; max_count: upper bound; qlevel: quality control; minDist: some distance between the features
}

// Should we add more points?
bool FeatureTracker::addNewPoints ()
{
	return points[0].size () <= 10; // Too less points to be tracked!
}

bool FeatureTracker::acceptTrackedPoint (int i)
{
	return status[i] && ((abs (points[0][i].x - points[1][i].x) + abs (points[0][i].y - points[1][i].y)) > 2); // Is the point good?
}

// Process all these points
void FeatureTracker::handleTrackedPoints (Mat& frame, Mat& output)
{
	for (int i = 0; i < points[1].size (); i++)
	{
		line (output, initial[i], points[1][i], Scalar (255, 255, 255)); // Draw a line showing the connection
		if (points[0][i].x < points[1][i].x)
		{
			result[0]++;
		}
		else
		{
			result[1]++;
		}
		circle (output, points[1][i], 3, Scalar (255, 255, 255), -1);
	}
}
