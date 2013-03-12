#include "VideoProcessor.h"
using namespace std;
using namespace cv;

class FeatureTracker : public FrameProcessor
{
	private:
		Mat gray; // Current gray image
		Mat gray_prev; // Previous gray image
		vector <Point2f> points[2]; // Tracked features
		vector <Point2f> initial; // Initial positions of the tracked points
		vector <Point2f> features; // Features which are being tracked
		int max_count; // Upper limit on the number of features to be tracked
		double qlevel; // Quality level for feature detection
		double minDist; // Minimum distance between 2 feature points
		vector <uchar> status; // Status of tracked features
		vector <float> err; // Error?
		int result[2]; // Count of the direction recorded: 0 is left, 1 is right
	public:
		FeatureTracker (); // Constructor
		void process (Mat& frame, Mat& output); // Function which processes the frames
		void detectFeaturePoints (); // Detect the points to be tracked
		bool addNewPoints (); // Do we need more points?
		bool acceptTrackedPoint (int i); // Whether a point should be tracked or not
		void handleTrackedPoints (Mat& frame, Mat& output); // Process the currently tracked points
};
