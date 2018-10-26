#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;



int main(int argc, const char** argv)
{
    ///OPDRACHT 1.1
    // Adding a command line parser
    CommandLineParser parser(argc, argv,
        "{help h usage ?  | | show this message }"
        "{image @image    | | (required) path to input image}"
        "{mode m          | | (required) mode 0 skin segmentation 1 text segmentation}"
    );
    // show help if help is asked
    if (parser.has("help")){
        parser.printMessage();
        cerr << "Tip: use absolute paths to avoid any problems with the software" << endl;
        return 0;
    }
    // parse arguments and check if they are entered correctly
    // check image location
    string image_location(parser.get<string>("image"));
    if (image_location.empty()){
        cerr << "You did not enter a correct image location" << endl;
        return -1;
    }
    // check image
    Mat image = imread(image_location);
    if (image.empty()){
        cerr << "The image was read as empty. Maybe something is wrong with your image?" << endl;
    }
    // check mode
    int mode(parser.get<int>("mode"));
    if ((mode != 0) && (mode != 1)){
        cerr << "You did not enter a valid mode. Please select a mode of 0 or 1." << endl;
        return -1;
    }

    /// MODE 0: Skin segmentation
    if (mode == 0){
        cerr<< "SKIN SEGMENTATION" <<endl;
        imshow("Original image",image);
        waitKey(0);
        //split channels
        Mat separatechannels[3];
        split(image,separatechannels);
        Mat Blue = separatechannels[0];
        Mat Green = separatechannels[1];
        Mat Red = separatechannels[2];
        //create zero matrix for mask
        Size imagesize = image.size();
        Mat mask = Mat::zeros(imagesize,CV_8UC1);
        //use matrix operations to define mask
        mask = ((Red>95) & (Green>40) & (Blue>20) & ((max(Red,max(Green,Blue)) - min(Red,min(Green,Blue)))>15
        & (abs(Red-Green)>15)) & (Red>Green) & (Red>Blue));
        imshow("mask",(mask*255));
        waitKey(0);
        //use splitted channels and apply mask on these separatly
        vector<Mat> masked_image;
        masked_image.push_back(Blue & mask);
        masked_image.push_back(Green & mask);
        masked_image.push_back(Red & mask);
        Mat mixed;
        merge(masked_image, mixed);
        //show masked image in color;
        imshow("Masked image",mixed);
        waitKey(0);
        return 0;
    }
    /// MODE 1: Text segmentation
    if (mode == 1){
        //OTSU tresholding
        //Histogram equalization
        //CLAHE
    }
    ///OPDRACHT 1.2



}
