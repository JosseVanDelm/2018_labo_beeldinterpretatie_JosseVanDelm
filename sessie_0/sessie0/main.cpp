#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

int main(int argc, const char** argv)
{
    // Adding a command line parser
    CommandLineParser parser(argc, argv,
        "{help h usage ? | | show this message }"
        "{@image_gray    | | (required) path to input image in grayscale }"
        "{@image_color   | | (required) path to input image in color }"
    );
    // show help if help is asked
    if (parser.has("help")){
        parser.printMessage();
        cerr << "Tip: use absolute paths to avoid any problems with the software" << endl()
        return 0;
    }

    // parse arguments
    string image_gray_location(parser.get<string>("image_gray"));
    string image_color_location(parser.get<string>("image_color"));
    if (imgage_gray_location.empty()|| image_color_location.empty()){
        cerr << "There must be something wrong with your arguments. Please check again." << endl();
        parser.printMessage();
        return -1;
    }
    // load and show grayscale image
    Mat image;
    image = imread(image_gray_location);
    if(image.empty()){
        cerr<<"image not found";
        return -1;
    }
    imshow("Grayscale Image",image)
    waitKey(0);
    // load and show bgr image

    // split channels and show bgr image

}
