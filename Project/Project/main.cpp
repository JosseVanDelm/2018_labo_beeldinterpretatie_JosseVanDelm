#include <iostream>
#include <opencv2/opencv.hpp>

#define DIRECTIONS 4
#define LEFT 0
#define RIGHT 1
#define UP 2
#define DOWN 3 

#define FACES 3
#define NEUTRAL 0
#define OPEN1 1
#define OPEN2 2

using namespace std;
using namespace cv;

void initDetectPacman(string neutral_path,string open1_path, string open2_path);
Rect detectPacman(Mat frame);

int main(int argc, const char** argv){ 
	//Command line parser
	CommandLineParser parser(argc, argv,
        "{help h usage ? | | show this message }"
        "{video          | | (required) path to input video footage of pacman}"
	"{neutral_temp   | | (required) path to pacman template of neutral face (aka full circle)}"
	"{open1_temp     | | (required) path to pacman template of open 1 (<) (aka half open)}"
	"{open2_temp     | | (required) path to pacman template of open 2 (<) (aka fully open)}"
    	);
    	// show help if help is asked
    	if (parser.has("help")){
	       	parser.printMessage();
		cerr << "Tip: use absolute paths to avoid any problems with the software" << endl;
		cerr << "The templates of the image have to be in the right direction (with the mouth of pacman like this: < )" << endl;
        return 0;
    }
    	// parse arguments and check if they are entered correctly
    	string video_location(parser.get<string>("video"));
	string neutral_location(parser.get<string>("neutral_temp"));
	string open1_location(parser.get<string>("open1_temp"));
	string open2_location(parser.get<string>("open2_temp"));
    	if (video_location.empty()||neutral_location.empty()||open1_location.empty()||open2_location.empty()){
    	    cerr << "There is something wrong with your arguments. Please check again." << endl;
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
    // This region removes black borders and bottom with lives displayed;
    // The bottom would interfere with pacman detection
    Rect CropReg = Rect(101,1,280,340);
    while(!frame.empty()){
      // Display the resulting frame (optional)
      // imshow( "PacPacman", frame );
      /* Do operations on the frame*/
      // 1: Crop the frame
      Mat cropped = frame(CropReg);
      // Display the cropped frame (optional)
      imshow("PacPacman",cropped);
      // 2: Detect pacman figure
      Rect detection;
      detection = detectPacman(cropped);
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

Mat rotate(Mat src, double angle)
{
    // Only works for 90,180 and 270 degrees!
    Mat dst;
    Point2f pt(src.cols/2., src.rows/2.);
    Mat r = getRotationMatrix2D(pt, angle, 1.0);
    if(angle == 90 || angle == 270){
	    warpAffine(src, dst, r, Size(src.rows,src.cols));
    }
    else if(angle == 180){
	    warpAffine(src,dst, r, Size(src.cols,src.rows));
    }
    return dst;
}

void initDetectPacman(string neutral_path,string open1_path, string open2_path){
	// Create all necessary rotation matrices
	
	// Create variable containing all the patterns
	Mat patterns[FACES][DIRECTIONS];
	// Read in all the template images (these must be oriented right)
	// For neutral file
	Mat neutral = imread(neutral_path);
	patterns[NEUTRAL][0] = neutral;
	// For open1 file
	Mat open1 = imread(open1_path);
	patterns[NEUTRAL][RIGHT] = open1;
}
	
Rect detectPacman(Mat frame)
{
	// Create matching_image and template_image in grayscale
	Mat mi_gs;
	cvtColor(frame,mi_gs,CV_BGR2GRAY);
	Mat ti_gs;
	//ti_gs =
	Rect returned = Rect(1,2,3,4);
	return returned;
}
