#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>
#include <vector>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "VideoProcessor.h"
using namespace cv;
using namespace std;

// Constructor setting all the default values
VideoProcessor::VideoProcessor ()
{
	callIt = false;
	delay = -1;
	fnumber = 0;
	stop = false;
	frameToStop = -1;
	process = 0;
	frameProcessor = 0;
}

// Read the next frame of the video
bool VideoProcessor::readNextFrame (Mat& frame)
{
	return capture.read (frame);
}

// Writing the next output frame
void VideoProcessor::writeNextFrame (Mat& frame)
{
	writer.write (frame);
}

// Input is a pre-recorded video - passing the file name
bool VideoProcessor::setInput (string filename)
{
	fnumber = 0; // Reset number of processed frames to 0
	capture.release (); // Flush any pre-existing resources
	return capture.open (filename);
}

// Input is taken LIVE! From the camera, of course
bool VideoProcessor::setInput (int id)
{
	fnumber = 0; // Reset number of processed frames to 0
	capture.release (); // Flush any pre-existing resources
	return capture.open (id);
}

// Setting the output video file
bool VideoProcessor::setOutput (const string& filename, int codec = 0, double framerate = 0.0, bool isColor = true)
{
	outputFile = filename; // Giving it a name
	if (framerate == 0.0)
	{
		framerate = getFrameRate (); // If frame rate isn't specified, we take the input file's frame rate
	}
	char c[4];
	if (codec == 0)
	{
		codec = getCodec (c); // If codec isn't specified, we take the input file's codec
	}
	return writer.open (outputFile, codec, framerate, getFrameSize (), isColor); // Open the output video with these specifications
}

// Set the callback function which will be called for each frame
void VideoProcessor::setFrameProcessor (void (*frameProcessingCallback) (Mat&, Mat&))
{
	frameProcessor = 0; // Dump the class interface object
	process = frameProcessingCallback; // Set the pointer
	callProcess (); // Start processing
}

// Using the class interface instead
void VideoProcessor::setFrameProcessor (FrameProcessor* frameProcessorPtr)
{
	process = 0; // Clear the callback function
	frameProcessor = frameProcessorPtr; // Set the pointer
	callProcess (); // Start processing
}

// Stop processing at this frame!
void VideoProcessor::stopAtFrameNo (long frame)
{
	frameToStop = frame;
}

// Processing yes!
void VideoProcessor::callProcess ()
{
	callIt = true;
}

// Processing no!
void VideoProcessor::dontCallProcess ()
{
	callIt = false;
}

// Display the input frames
void VideoProcessor::displayInput (string wn)
{
	windowNameInput = wn;
	namedWindow (windowNameInput); // Create a window
}

// Display the processed frames
void VideoProcessor::displayOutput (string wn)
{
	windowNameOutput = wn;
	namedWindow (windowNameOutput); // Create a window
}

// Don't display anything!
void VideoProcessor::dontDisplay ()
{
	destroyWindow (windowNameInput);
	destroyWindow (windowNameOutput);
	windowNameInput.clear (); // Flush the input stream
	windowNameOutput.clear (); // Flush the output stream
}

// Setting a delay between each frame. Also, 0 is wait indefinitely and negative number is don't wait
void VideoProcessor::setDelay (int d)
{
	delay = d;
}

// Number of frames which have been processed till now
long VideoProcessor::getNumberOfProcessedFrames ()
{
	return fnumber;
}

// Size a.k.a. resolution of the video
Size VideoProcessor::getFrameSize ()
{
	int width = static_cast <int> (capture.get (CV_CAP_PROP_FRAME_WIDTH)); // Frame width
	int height = static_cast <int> (capture.get (CV_CAP_PROP_FRAME_HEIGHT)); // Frame height
	return Size (width, height);
}

// Frame number of the next frame
long VideoProcessor::getFrameNumber ()
{
	long frameNo = static_cast <long> (capture.get (CV_CAP_PROP_POS_FRAMES));
	return frameNo;
}

// Return the current position in milliseconds
double VideoProcessor::getPositionMS ()
{
	double msecPos = capture.get (CV_CAP_PROP_POS_MSEC);
	return msecPos;
}

// Return the frame rate of the video (example: 60fps)
double VideoProcessor::getFrameRate ()
{
	double fps = capture.get (CV_CAP_PROP_FPS);
	return fps;
}

// Total number of frames in the video
long VideoProcessor::getTotalFrameCount ()
{
	long frameCount = capture.get (CV_CAP_PROP_FRAME_COUNT);
	return frameCount;
}

// Get the codec of the input video
int VideoProcessor::getCodec (char codec[4])
{
	union
	{
		int value;
		char code[4];
	}
	returned;
	returned.value = static_cast <int> (capture.get (CV_CAP_PROP_FOURCC));
	codec[0] = returned.code[0];
	codec[1] = returned.code[1];
	codec[2] = returned.code[2];
	codec[3] = returned.code[3];
	return returned.value;
}

// Go to this frame
bool VideoProcessor::setFrameNumber (long frameNo)
{
	return capture.set (CV_CAP_PROP_POS_FRAMES, frameNo);
}

// Go to this position in the video (in milliseconds)
bool VideoProcessor::setPositionMS (double msecPos)
{
	return capture.set (CV_CAP_PROP_POS_MSEC, msecPos);
}

// Position as a fraction of the total length of the video
bool VideoProcessor::setRelativePosition (double relPos)
{
	return capture.set (CV_CAP_PROP_POS_AVI_RATIO, relPos);
}

// Halt!
void VideoProcessor::stopIt ()
{
	stop = true;
}

// Have I stopped?
bool VideoProcessor::isStopped ()
{
	return stop;
}

// Did the video open?
bool VideoProcessor::isOpened ()
{
	return capture.isOpened ();
}

// Let's run this!
void VideoProcessor::run ()
{
	Mat frame; // Input current frame
	Mat output; // Current output frame
	if (!isOpened ()) // If video or the capture device cannot be opened, exit!
	{
		cout << "Cannot open capture device\n"; // A friendly error message
		return;
	}
	stop = false; // Don't stop now!
	while (!isStopped ())
	{
		if (!readNextFrame (frame)) // Read the next frame
		{
			break;
		}
		if (windowNameInput.length () != 0) // Show the input frame : Doesn't work when it is captured from a camera
		{
			imshow (windowNameInput, frame);
		}
		if (callIt)
		{
			if (process)
			{
				process (frame, output);
			}
			else if (frameProcessor)
			{
				frameProcessor -> process (frame, output);
			}
			fnumber++; // Increment number of frames processed
		}
		else
		{
			output = frame;
		}
		if (outputFile.length () != 0) // Write the output frame
		{
			writeNextFrame (output);
		}
		if (windowNameOutput.length () != 0) // Make a window
		{
			imshow (windowNameOutput, output);
		}
		if (delay >= 0 && waitKey (delay) >= 0) // If delay, stop
		{
			stopIt ();
		}
		if (frameToStop >= 0 && getFrameNumber () == frameToStop) // If video over, stop
		{
			stopIt ();
		}
	}
}
