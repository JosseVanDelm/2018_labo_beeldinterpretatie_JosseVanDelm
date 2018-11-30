#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

/* Program options:
--training_image=""
--inference_image=""
*/
///LINKER: `pkg-config opencv --libs`
// in param de lijst meegeven
static void onMouse(int event,int x, int y, int, void* param){
    switch(event) {
    case EVENT_LBUTTONDOWN : addPoint( param);cerr << "(" << x << ","<< y << ") L" << endl; break;
    case EVENT_RBUTTONDOWN : cerr << "(" << x << ","<< y << ") R" << endl; break;
    case EVENT_MBUTTONDOWN : cerr << "(" << x << ","<< y << ") M" << endl; break;
    }
}

vector<Point2d> addPoint(vector<Point2d> Points,int x, int y){
    cerr << "Point (" << x << ","<< y << ") added" << endl;
    Points.push_back(Point(x,y));
    return Points;
}

vector<Point2d> deletePoint(vector<Point2d> Points){
    cerr << "Latest point deleted" << endl;
    Points.pop_back();
    return Points;
}

void showPoints(vector<Point2d> Points){
    cerr << "Showing selected points" << endl << "=======================" << endl;
    for(size_t i = 0; i < Points.size(); i++){
        cerr << "(" << Points[i].x << "," << Points[i].y << ")" << endl;
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

    //gaussian blur toevoegen om minder afhankelijk te zijn van pitjes
    // kies voor  knn 3 classen

    //retrieving the hsv pixel values for each position
    Mat img_hsv
    cvtColor ...

    //Prepare foreground training data using the HSV values as a descriptor

    Mat trainingDataForeground(strawberry.size(),3,CV_32FC1); //strawberry zijn de punten geselecteerd.
    Mat labels_fg = Mat::ones(strawberry.size(),1,CV_32SC1);

    for (int i = 0; i < strawberry.size(); i++) {
        Vec3b descriptor = img_hsv.at<Vec3b>(strawberry[i].y,strawberry[i].x);
        trainingDataForeground.at<float>(i,0) = descriptor[0];
        trainingDataForeground.at<float>(i,0) = descriptor[0];
        trainingDataForeground.at<float>(i,0) = descriptor[0];
    }

    //Prepare background training data using the HSV values as a descriptor
    Mat trainingDataBackground(strawberry.size(),3,CV_32FC1); //strawberry zijn de punten geselecteerd.
    Mat labels_fg = Mat::ones(strawberry.size(),1,CV_32SC1);

    for (int i = 0; i < strawberry.size(); i++) {
        Vec3b descriptor = img_hsv.at<Vec3b>(strawberry[i].y,strawberry[i].x);
        trainingDataBackground.at<float>(i,0) = descriptor[0];
        trainingDataBackground.at<float>(i,0) = descriptor[0];
        trainingDataBackground.at<float>(i,0) = descriptor[0];
    }

    // group foreground and background
    Mat trainingData, labels;
    vconcat(trainingDataForeground,trainingDataBackground,trainingData);
    vconcat(labels_fg,labels_bg,labels);

}
