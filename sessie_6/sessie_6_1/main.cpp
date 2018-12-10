#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

/*sources consulted for this code:

https://www.learnopencv.com/read-write-and-display-a-video-using-opencv-cpp-python/
https://docs.opencv.org/3.4/db/d28/tutorial_cascade_classifier.html

*/

//prototypes

Mat detectPedestrianAndDraw( Mat detection_frame,Mat draw_frame, Scalar color, HOGDescriptor hog);

int main(int argc, const char** argv){
    // Adding a command line parser
    CommandLineParser parser(argc, argv,
        "{help h usage ? | | show this message }"
        "{video          | | (required) path to input video}"
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
        cerr << "There must be something wrong with your arguments. Please check again." << endl;
        parser.printMessage();
        return -1;
    }
    VideoCapture cap(video_location);
    // Check if camera opened successfully
    if(!cap.isOpened()){
      cerr << "Error opening video stream or file" << endl;
      return -1;
    }
    HOGDescriptor hog = HOGDescriptor();
    hog.setSVMDetector(HOGDescriptor::getDefaultPeopleDetector());
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
      frame_detection = detectPedestrianAndDraw(frame,frame,Scalar(0,0,255),hog);
      imshow( "Pedestrian detection", frame_detection );
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
Mat detectPedestrianAndDraw( Mat detection_frame,Mat draw_frame, Scalar color, HOGDescriptor hog)
{
    Mat frame_gray;
    cvtColor( detection_frame, frame_gray, COLOR_BGR2GRAY );
    equalizeHist( frame_gray, frame_gray );
    //-- Detect faces
    std::vector<Rect> pedestrians;
    std::vector<double> detections;
    hog.detectMultiScale(frame_gray,pedestrians,detections);
    for ( size_t i = 0; i < pedestrians.size(); i++ )
    {
        rectangle( draw_frame,pedestrians[i], color, 4 );
        putText(draw_frame,to_string(detections[i]),Point(pedestrians[i].x,pedestrians[i].y),FONT_HERSHEY_COMPLEX,1,color);
    }
    //-- Show what you got
    return draw_frame;
}



