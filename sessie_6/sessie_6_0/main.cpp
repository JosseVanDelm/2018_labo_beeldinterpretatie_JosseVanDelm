#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

/*sources consulted for this code:

https://www.learnopencv.com/read-write-and-display-a-video-using-opencv-cpp-python/
https://docs.opencv.org/3.4/db/d28/tutorial_cascade_classifier.html

*/

//prototypes
void detectAndDisplay( Mat frame );

CascadeClassifier face_cascade;

int main(int argc, const char** argv){
    // Adding a command line parser
    CommandLineParser parser(argc, argv,
        "{help h usage ? | | show this message }"
        "{video          | | (required) path to input video}"
        "{model          | | (required) path to xml file containing classifier model}"
    );
    // show help if help is asked
    if (parser.has("help")){
        parser.printMessage();
        cerr << "Tip: use absolute paths to avoid any problems with the software" << endl;
        return 0;
    }
    // parse arguments and check if they are entered correctly
    string video_location(parser.get<string>("video"));
    string model_location(parser.get<string>("model"));
    if (video_location.empty()){
        cerr << "There must be something wrong with your arguments. Please check again." << endl;
        parser.printMessage();
        return -1;
    }
    if( !face_cascade.load( model_location) ){
        cerr << "Error loading face cascade" << endl;
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
      // Capture frame-by-frame
      cap >> frame;
      // If the frame is empty, break immediately
      if (frame.empty()){
        cerr << "Frame was empty. Quitting..." << endl;
        break;
      }
      // Display the resulting frame
      imshow( "Frame", frame );
      detectAndDisplay(frame);
      // Press  ESC on keyboard to exit
      char c=(char)waitKey(25);
      if(c==27)
        break;
    }
}

void detectAndDisplay( Mat frame )
{
    Mat frame_gray;
    cvtColor( frame, frame_gray, COLOR_BGR2GRAY );
    equalizeHist( frame_gray, frame_gray );
    //-- Detect faces
    std::vector<Rect> faces;
    std::vector<int> detections;
    face_cascade.detectMultiScale( frame_gray, faces, detections );
    for ( size_t i = 0; i < faces.size(); i++ )
    {
        Point center( faces[i].x + faces[i].width/2, faces[i].y + faces[i].height/2 );
        ellipse( frame, center, Size( faces[i].width/2, faces[i].height/2 ), 0, 0, 360, Scalar( 255, 0, 255 ), 4 );
        putText(frame,to_string(detections[i]),Point(faces[i].x,faces[i].y),FONT_HERSHEY_COMPLEX,1,Scalar(0,255,0));
    }
    //-- Show what you got
    imshow( "Face detection", frame );
}



