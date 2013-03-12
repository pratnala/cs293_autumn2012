#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/features2d/features2d.hpp>
#include <opencv2/video/tracking.hpp>
#include "FeatureTracker.h"
using namespace std;
using namespace cv;

int main ()
{
	VideoProcessor processor;
	FeatureTracker tracker;
	processor.setInput ("sample_video_1.mov"); // The file name which is to be given as input. Use 0 (without quotes) to take input from webcam
	processor.setFrameProcessor (&tracker); // Set the tracking interface
	processor.displayOutput ("Tracked Features"); // Show the output
	processor.setDelay (1000. / processor.getFrameRate ());
	processor.run ();
	waitKey ();
}
