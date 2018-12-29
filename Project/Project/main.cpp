#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

int main(int argc, const char** argv){ 
	//Command line parser
	CommandLineParser parser(argc, argv,
        "{help h usage ? | | show this message }"
        "{video          | | (required) path to input video footage of pacman}"
    	);
    	// show help if help is asked
    	if (parser.has("help")){
	       	parser.printMessage();
		cerr << "Tip: use absolute paths to avoid any problems with the software" << endl;
        return 0;
    }
    	// parse arguments and check if they are entered correctly
    	string video_location(parser.get<string>("video"));
    	if (video_location.empty()){
    	    cerr << "There is something wrong with your video path argument. Please check again." << endl;
    	    parser.printMessage();
        return -1;
    }
    VideoCapture cap(video_location);
    // Check if camera opened successfully
    if(!cap.isOpened()){
      cerr << "Error opening video stream or file" << endl;
      return -1;
    }
    // Initialize first frame
    Mat frame;
    cap >> frame;
    // Initialize cropping region
    // This region removes black borders and bottom with lives displayed
    Rect CropReg = Rect(101,1,280,340);
    while(!frame.empty()){
      // Display the resulting frame (optional)
      // imshow( "PacPacman", frame );
      /* Do operations on the frame*/
      // 1: Crop the frame
      Mat cropped = frame(CropReg);
      // Display the cropped frame (optional)
      // imshow("PacPacman",cropped);
      // 2: Detect pacman figure
      Rect detection;

      // Press  ESC on keyboard to exit
      // use waitkey with 25 seconds to wait for 25 seconds (to show smooth video)
      // TODO replace waitkey(0)
      char c=(char)waitKey(1);
      if(c==27){
	      break;
      }
      // read next frame
      cap >> frame;
    }
    if(frame.empty()){
	    cerr << "Frame was empty. Quitting..." << endl;
    }
    // if the frame was not empty but the loop was stopped, the user interrupted execution.
    else{
	    cerr << "User interrupted execution. Quitting..." << endl;
    }
}

Rect detectPacman(Mat frame)
{
	Rect returned = Rect(1,2,3,4)
	return returned
}
