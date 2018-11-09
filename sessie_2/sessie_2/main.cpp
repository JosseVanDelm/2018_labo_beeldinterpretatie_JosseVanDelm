#include <iostream>
#include <opencv2/opencv.hpp>
#include <string>

///LINKER: `pkg-config opencv --libs`

using namespace std;
using namespace cv;

int main(int argc, const char** argv)
{
    // Adding a command line parser
    CommandLineParser parser(argc, argv,
        "{help h usage ?  | | show this message }"
        "{image1 @image1    | | (required) path to input image 1}"
        "{image2 @image2    | | (required) path to input image 2}"
        "{image3 @image3    | | (required) path to input image 3}"
        "{image4 @image4    | | (required) path to input image 4}"
    );
    // show help if help is asked
    if (parser.has("help")){
        parser.printMessage();
        cerr << "Tip: use absolute paths to avoid any problems with the software" << endl;
        return 0;
    }
    // parse arguments and check if they are entered correctly
    vector<Mat> Images;
    for (int i=1; i<=4;i++){
        // check image location
        stringstream temp;
        temp << "image" << i;
        string current_image_str = temp.str();
        string image_location(parser.get<string>(current_image_str));
        if (image_location.empty()){
            cerr << "You did not enter a correct image location for image " << i << endl;
            return -1;
        }
        Mat image = imread(image_location);
        // check image
        if (image.empty()){
            cerr << "The image was read as empty. Maybe something is wrong with image "<< i << "?"<< endl;
            return -1;
        }
        // Add images to vector
        Images.push_back(image);
        //Option: uncomment tot show loaded images
        //imshow(current_image_str,image);
    }
    waitKey(0);
    Mat image = Images[0];

    vector<Mat> separatechannels;
    split(image,separatechannels);
    Mat Blue = separatechannels[0];
    Mat Green = separatechannels[1];
    Mat Red = separatechannels[2];
    //Make mask
    Size imagesize = image.size();
    Mat mask = Mat::zeros(imagesize,CV_8UC1);
    mask = (Red>100)& (Blue<100) & (Green<70);

    Mat mask_merge;

    vector<Mat> mask_3C;
    mask_3C.push_back(mask);
    mask_3C.push_back(mask);
    mask_3C.push_back(mask);
    merge(mask_3C,mask_merge);

    image = image & mask_merge;
    imshow("segmentation",image);
    waitKey();
}
