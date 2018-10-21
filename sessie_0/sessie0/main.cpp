#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

int main(int argc, const char** argv)
{
    /*TASK 1: make a command line parser*/
    // Adding a command line parser
    CommandLineParser parser(argc, argv,
        "{help h usage ? | | show this message }"
        "{image_gray @image_gray    | | (required) path to input image in grayscale }"
        "{image_color @image_color   | | (required) path to input image in color }"
    );

    // show help if help is asked
    if (parser.has("help")){
        parser.printMessage();
        cerr << "Tip: use absolute paths to avoid any problems with the software" << endl;
        return 0;
    }

    // parse arguments and check if they are entered correctly
    string image_gray_location = parser.get<string>("image_gray");
    string image_color_location(parser.get<string>("image_color"));
    if (image_gray_location.empty()|| image_color_location.empty()){
        cerr << "There must be something wrong with your arguments. Please check again." << endl;
        parser.printMessage();
        return -1;
    }
    /*TASK 2: Load and show images test.png and testColor.png*/
    // load and show grayscale image and first test if the image file contains any data
    Mat image;
    image = imread(image_gray_location);
    if(image.empty()){
        cerr<<"grayscale image is not found";
        return -1;
    }
    imshow("Grayscale Image",image);
    // load and show bgr image and test if the image file contains any data
    Mat colorimage;
    colorimage = imread(image_color_location);
    if(colorimage.empty()){
        cerr<<"color image file is not found";
        return -1;
    }
    imshow("Color Image",colorimage);
    waitKey(0);

    /*TASK 3: Split color image in separate color channels and show them individually*/
    // split channels and show separate bgr
    // no check on colorimage necessary,already done cf. supra
    Mat separatechannels[3];
    split(colorimage,separatechannels);
    // separate channels are stored in B G R order
    // now show each separate channel
    imshow("B Channel",separatechannels[0]);
    imshow("G Channel",separatechannels[1]);
    imshow("R Channel",separatechannels[2]);
    waitKey(0);

    /*TASK 4: Convert color image to grayscale*/
    //convert colorimage to grayscale image with cvtColor function
    Mat convertedimage;
    cvtColor(colorimage,convertedimage,COLOR_BGR2GRAY);
    imshow("Color image converted to grayscale image",convertedimage);
    waitKey(0);

    /*TASK 5: Loop through image values and print them out*/
    //now loop through image values of converted image
    Size imagesize = convertedimage.size();
    for(int i = 0; i < imagesize.height; i++){
        for(int j = 0; j < imagesize.width; j++){
            cout << (int)convertedimage.at<uchar>(i,j) << "|"; //print 8-bit gray values as numbers (ints) and space values with |
        }
        cout << endl;
    }
    waitKey(0);

    /*TASK 6: Make a canvas and draw something*/
    //Draw some stuff on a black canvas;
    int width = 100;
    int height = 100;
    // define a canvas
    Mat canvas = Mat::zeros(width,height,CV_8UC3);
    // draw a red circle in the middle with a radius of width/4 (thickness -1 sets filled circle) with default linetupe
    circle(canvas,Point(width/2,height/2),width/4,Scalar(0,0,255),-1);
    // draw a green rectangle filling 1/4 of the screen with a thickness of 5
    rectangle(canvas,Point(0,0),Point(width/2,height/2),Scalar(0,255,0),5);
    imshow("Canvas with drawings",canvas);
    waitKey(0);

}
