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

vector<vector<Mat>> initDetectPacman(string neutral_path,string open1_path, string open2_path);
Rect detectPacman(Mat frame,vector<vector<Mat>> patterns);

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
    vector<vector<Mat>> patterns = initDetectPacman(neutral_location,open1_location,open2_location);
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
      detection = detectPacman(cropped,patterns);
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
vector<vector<Mat>> initDetectPacman(string neutral_path,string open1_path, string open2_path){
	// Create variable containing all the patterns
	vector<vector<Mat>> patterns(FACES,vector<Mat>(DIRECTIONS));
	// Read in all the template images (these must be oriented right)
	// For neutral file
	Mat neutral = imread(neutral_path);
	for(int i = 0; i < DIRECTIONS; i++){
		patterns[NEUTRAL][i] = neutral;
	}
	// For open1 file
	Mat open1 = imread(open1_path);
	patterns[OPEN1][RIGHT] = open1; // >>
	// also add other rotations
	rotate(open1,open1,ROTATE_90_COUNTERCLOCKWISE);
	patterns[OPEN1][UP] = open1; // ^^
	rotate(open1,open1,ROTATE_90_COUNTERCLOCKWISE);
	patterns[OPEN1][LEFT] = open1; // <<
	rotate(open1,open1,ROTATE_90_COUNTERCLOCKWISE);
	patterns[OPEN1][DOWN] = open1; // vv
	// For open2 file
	Mat open2 = imread(open2_path);
	patterns[OPEN2][RIGHT] = open2; // >>
	// also add other rotations
	rotate(open2,open2,ROTATE_90_COUNTERCLOCKWISE);
	patterns[OPEN2][UP] = open2; // ^^
	rotate(open2,open2,ROTATE_90_COUNTERCLOCKWISE);
	patterns[OPEN2][LEFT] = open2; // <<
	rotate(open2,open2,ROTATE_90_COUNTERCLOCKWISE);
	patterns[OPEN2][DOWN] = open2; // vv
	// now convert them to grayscale
	vector<vector<Mat>> patterns_gs;
	for(size_t i = 0; i < patterns.size(); i++){
		for(size_t j = 0; j < patterns[i].size();j++){
			cvtColor(patterns[i][j],patterns_gs[i][j],CV_BGR2GRAY);
		}
	}
	return patterns_gs;
}

Rect detectPacman(Mat frame,vector<vector<Mat>> patterns)
{
	// Create matching_image and template_image in grayscale
	Mat mi_gs;
	cvtColor(frame,mi_gs,CV_BGR2GRAY);
	for(size_t i = 0; i < patterns.size(); i++){
		for(size_t j = 0; j < patterns[i].size();j++){
			Mat result_gs;
			// use cross correlation to match templates
			matchTemplate(mi_gs,patterns[i][j],result_gs,TM_CCOEFF);
			normalize(result_gs,result_gs,0,1,NORM_MINMAX,-1,Mat());
		}
	}
	Rect returned = Rect(1,2,3,4);
	return returned;
}
