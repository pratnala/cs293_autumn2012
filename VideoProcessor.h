using namespace cv;
using namespace std;

// Used as an interface for frames
class FrameProcessor
{
	public:
		// The processing method
		virtual void process (Mat& input, Mat& output) = 0;
};

// Main class which does video processing
class VideoProcessor
{
	private:
		VideoCapture capture; // This is the object which captures the video either from the camera or a pre-recorded video
		void (*process) (Mat&, Mat&); // This is the callback function pointer called for processing each frame
		FrameProcessor *frameProcessor; // Pointer to the object of the FrameProcessor class
		bool callIt; // This variable determines if the process callback will be called in the first place
		string windowNameInput; // Window name of the input video
		string windowNameOutput; // Window name of the output video (after processing)
		int delay; // Delay between the frames processed
		long fnumber; // Number of processed frames
		long frameToStop; // The frame at which we should stop
		bool stop; // Boolean which is responsible for the processing to stop
		VideoWriter writer; // Object which will write the video
		string outputFile; // File name of the output file
		bool readNextFrame (Mat& frame); // Reading the next frame from the camera or a pre-recorded video
		void writeNextFrame (Mat& frame); // Writing the next frame to an output file
	public:
		VideoProcessor (); // Constructor setting all the default values
		bool setInput (string filename); // This function is called when input is a pre-recorded video
		bool setInput (int id); // This function is called when input is taken live - from a camera
		bool setOutput (const string& filename, int codec, double framerate, bool isColor); // Setting up the output file to write processed video
		void setFrameProcessor (void (*frameProcessingCallback) (Mat&, Mat&)); // Callback function for each frame
		void setFrameProcessor (FrameProcessor* frameProcessorPtr); // When the class interface is implemented
		void stopAtFrameNo (long frame); // Stop streaming at this frame
		void callProcess (); // Process callback is called
		void dontCallProcess (); // Process callback isn't called
		void displayInput (string wn); // Display the input frames
		void displayOutput (string wn); // Display the processed frames
		void dontDisplay (); // Don't display anything
		void setDelay (int d); // Delay between each frame
		long getNumberOfProcessedFrames (); // Number of frames which have been processed
		Size getFrameSize (); // The size a.k.a. resolution of the video
		long getFrameNumber (); // Frame number of the next frame to be processed
		double getPositionMS (); // Return the current position in milliseconds
		double getFrameRate (); // Frame rate of the video
		long getTotalFrameCount (); // Total number of frames in the video
		int getCodec (char codec[4]); // Codec used in the video
		bool setFrameNumber (long frameNo); // Go to a particular frame
		bool setPositionMS (double msecPos); // Go to this position (in milliseconds)
		bool setRelativePosition (double relPos); // Position is expressed as a fraction of the total length
		void stopIt (); // Stop processing
		bool isStopped (); // Has the processing stopped?
		bool isOpened (); // Has the video opened?
		void run (); // Main function!!
};
