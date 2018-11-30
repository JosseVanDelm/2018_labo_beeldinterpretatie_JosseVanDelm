#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

/* Program options:
--training_image=""
--inference_image=""
*/
///LINKER: `pkg-config opencv --libs`
static void onMouse(int event,int x, int y, int, void*){
    switch(event) {
    case EVENT_LBUTTONDOWN : cerr << "(" << x << ","<< y << ") L" << endl; break;
    case EVENT_RBUTTONDOWN : cerr << "(" << x << ","<< y << ") R" << endl; break;
    case EVENT_MBUTTONDOWN : cerr << "(" << x << ","<< y << ") M" << endl; break;
    }
}

int main(int argc, const char** argv){
    // Adding a command line parser
    CommandLineParser parser(argc, argv,
        "{help h usage ?                   | | show this message }"
        "{training_image training_image    | | (required) path to input matching image}"
        "{inference_image inference_image  | | (required) path to input image template in color }"
    );
    // show help if help is asked
    if (parser.has("help")){
        parser.printMessage();
        cerr << "Tip: use absolute paths to avoid any problems with the software" << endl;
        return 0;
    }
    // parse arguments and check if they are entered correctly
    string training_image_location = parser.get<string>("training_image");
    string inference_image_location(parser.get<string>("inference_image"));
    if (training_image_location.empty()|| inference_image_location.empty()){
        cerr << "There must be something wrong with your arguments. Please check again." << endl;
        parser.printMessage();
        return -1;
    }
    Mat training_image = imread(training_image_location);
    Mat inference_image = imread(inference_image_location);
    if(training_image.empty() || inference_image.empty()){
        cerr<<"One of the images is not found";
        return -1;
    }
    imshow("Add training data",training_image);
    setMouseCallback("Add training data",onMouse,0);
    waitKey(0);
}
