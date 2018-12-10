#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

/*sources consulted for this code:

https://www.learnopencv.com/read-write-and-display-a-video-using-opencv-cpp-python/
https://docs.opencv.org/3.4/db/d28/tutorial_cascade_classifier.html

*/

//prototypes

Mat detectAndDisplay( Mat detection_frame,Mat draw_frame, Scalar color, CascadeClassifier* classifier );

int main(int argc, const char** argv){
    // Adding a command line parser
    CommandLineParser parser(argc, argv,
        "{help h usage ? | | show this message }"
        "{video          | | (required) path to input video}"
        "{model          | | (required) path to xml file containing classifier model}"
	"{model2         | | (optional) path to xml file containing second classifier model}"
    );
    bool multiple_models = false;
    CascadeClassifier model_cascade;
    CascadeClassifier model2_cascade;
    // show help if help is asked
    if (parser.has("help")){
        parser.printMessage();
        cerr << "Tip: use absolute paths to avoid any problems with the software" << endl;
        return 0;
    }
    // parse arguments and check if they are entered correctly
    string video_location(parser.get<string>("video"));
    string model_location(parser.get<string>("model"));
    string model2_location(parser.get<string>("model2"));
    if (video_location.empty()){
        cerr << "There must be something wrong with your arguments. Please check again." << endl;
        parser.printMessage();
        return -1;
    }
    if (model2_location.empty()){
        cerr << "No second model was given" << endl;
    }
    else{
    	cerr << "using second model as well" << endl;
	multiple_models = true;
	if(!model2_cascade.load(model_location)){
		cerr << "error loading second model" << endl;
		return -1;
	}
    }
    if( !model_cascade.load( model_location) ){
        cerr << "Error loading first model" << endl;
        return -1;
    };
    VideoCapture cap(video_location);
    // Check if camera opened successfully
    if(!cap.isOpened()){
      cerr << "Error opening video stream or file" << endl;
      return -1;
    }
    while(1){
      Mat frame;
      Mat frame_detection;
      // Capture frame-by-frame
      cap >> frame;
      // If the frame is empty, break immediately
      if (frame.empty()){
        cerr << "Frame was empty. Quitting..." << endl;
        break;
      }
      // Display the resulting frame (optional)
      // imshow( "Frame", frame );
      frame_detection = detectAndDisplay(frame,frame,Scalar(0,0,255),&model_cascade);
      if(multiple_models) frame_detection = detectAndDisplay(frame,frame_detection,Scalar(0,255,0),&model2_cascade);
      imshow( "Face detection", frame_detection );
      // Press  ESC on keyboard to exit
      char c=(char)waitKey(25);
      if(c==27)
        break;
    }
}
/*
 * This function detects objects:
 * Input:
 * 	- classifier: pointer to used CascadeClassifier
 * 	- color: scalar that represents color for drawing ellipses and text
 * 	- detection_frame: frame that the objectdetector uses to detect objects with given classifier
 * 	- draw_frame: frame that ellipses and text is drawn on.
 * Output:
 * 	- draw_frame with drawn detections and detection scores.
 */
Mat detectAndDisplay( Mat detection_frame,Mat draw_frame, Scalar color, CascadeClassifier* classifier )
{
    Mat frame_gray;
    cvtColor( detection_frame, frame_gray, COLOR_BGR2GRAY );
    equalizeHist( frame_gray, frame_gray );
    //-- Detect faces
    std::vector<Rect> faces;
    std::vector<int> detections;
    classifier->detectMultiScale( frame_gray, faces, detections );
    for ( size_t i = 0; i < faces.size(); i++ )
    {
        Point center( faces[i].x + faces[i].width/2, faces[i].y + faces[i].height/2 );
        ellipse( draw_frame, center, Size( faces[i].width/2, faces[i].height/2 ), 0, 0, 360, color, 4 );
        putText(draw_frame,to_string(detections[i]),Point(faces[i].x,faces[i].y),FONT_HERSHEY_COMPLEX,1,color);
    }
    //-- Show what you got
    return draw_frame;
}



