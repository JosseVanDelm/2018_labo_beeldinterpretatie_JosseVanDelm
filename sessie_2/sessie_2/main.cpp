#include <iostream>
#include <opencv2/opencv.hpp>
#include "opencv2/highgui.hpp"
#include <string>

///LINKER: `pkg-config opencv --libs`

using namespace std;
using namespace cv;

//contains values set for HSV sliders
const int H_slider_max = 179;
const int SV_slider_max = 255;
int H_max,H_min;
int S,V;

int imageselector;                          //contains selected imagge
Mat image_Hue,image_Saturation,image_Value; //contains HSV channels of selected image
Size imagesize;                             //contains size of selected image
Mat image,image_hsv;                        //contains image and hsv version of image

vector<Mat> separatechannels_hsv;           //vector that contains splitted channels of hsv_image
vector<Mat> Images;                         //vector that contains all images

static void on_trackbar(int, void*);        //function prototype declaration

int main(int argc, const char** argv)
{
    // Adding a command line parser
    // Read all images at once:
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
    for (int i=1; i<=4;i++){
        // check image location for each image
        stringstream temp;
        temp << "image" << i;
        string current_image_str = temp.str();
        string image_location(parser.get<string>(current_image_str));
        if (image_location.empty()){
            cerr << "You did not enter a correct image location for image " << i << endl;
            return -1;
        }
        Mat image = imread(image_location);
        // check each image
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

    for(size_t i = 0; i < Images.size(); i++)
    {
        image = Images[i];
        ///Part 1: BGR segmentation
        // split channels to perform individual operations
        vector<Mat> separatechannels;
        split(image,separatechannels);
        Mat Blue = separatechannels[0];
        Mat Green = separatechannels[1];
        Mat Red = separatechannels[2];
        //Make mask
        imagesize = image.size();
        Mat mask = Mat::zeros(imagesize,CV_8UC1);
        mask = (Red>100)& (Blue<100) & (Green<70);
        //merge mask to apply over all channels in one operation
        Mat mask_merge;
        vector<Mat> mask_3C;
        mask_3C.push_back(mask);
        mask_3C.push_back(mask);
        mask_3C.push_back(mask);
        merge(mask_3C,mask_merge);
        Mat image_seg;
        // apply image mask and show image
        image_seg = image & mask_merge;
        imshow("segmentation",image_seg);
        waitKey();
        ///Part 2: HSV segmentation
        // Convert image into HSV space
        Mat image_hsv;
        cvtColor(image,image_hsv,CV_BGR2HSV);
        // split channels to perform individual operations
        split(image_hsv,separatechannels_hsv);
        Mat Hue = separatechannels_hsv[0];
        Mat Saturation = separatechannels_hsv[1];
        Mat Value = separatechannels_hsv[2];
        //Make mask
        Mat mask_hsv = Mat::zeros(imagesize,CV_8UC1);
        mask_hsv = ((Hue<9) | (Hue>157)) & (Saturation>108) & (Value>128); //opencv uses hue degrees divided by 2
        imshow("HSV mask",mask_hsv*255);
        waitKey();
        //merge mask channels
        Mat mask_merge_hsv;
        vector<Mat> mask_3C_hsv;
        mask_3C_hsv.push_back(mask_hsv);
        mask_3C_hsv.push_back(mask_hsv);
        mask_3C_hsv.push_back(mask_hsv);
        merge(mask_3C_hsv,mask_merge_hsv);
        Mat image_seg_hsv;
        //apply mask
        image_seg_hsv = image & mask_merge_hsv;
        imshow("Segmentation with HSV-color space",image_seg_hsv);
        waitKey();
    }
    ///Part 3: Segmentation with trackbars
    //first convert image to hsv-space
    cvtColor(image,image_hsv,CV_BGR2HSV);
    //vector<Mat> separatechannels_hsv;
    split(image_hsv,separatechannels_hsv);
    image_Hue = separatechannels_hsv[0];
    image_Saturation = separatechannels_hsv[1];
    image_Value = separatechannels_hsv[2];

    namedWindow("HSV segmentation", WINDOW_AUTOSIZE); // Create Window
    //create sliders
    createTrackbar( "H max", "HSV segmentation", &H_max, H_slider_max, on_trackbar );
    createTrackbar( "H min", "HSV segmentation", &H_min, H_slider_max, on_trackbar );
    createTrackbar( "S", "HSV segmentation", &S, SV_slider_max, on_trackbar );
    createTrackbar( "V", "HSV segmentation", &V, SV_slider_max, on_trackbar );
    createTrackbar( "Image", "HSV segmentation", &imageselector,3,on_trackbar);
    on_trackbar(0,0);
    waitKey();
    return 0;
}

static void on_trackbar(int, void*)
{
    /*
        GOOD VALUES FOR SEGMENTATION
        Hmax = 9
        Hmin = 157
        S = 108
        V = 128
    */
    image = Images[imageselector];
    //first convert image to hsv-space
    cvtColor(image,image_hsv,CV_BGR2HSV);
    //separate the channels to perform operations
    split(image_hsv,separatechannels_hsv);
    image_Hue = separatechannels_hsv[0];
    image_Saturation = separatechannels_hsv[1];
    image_Value = separatechannels_hsv[2];
    imagesize = image.size();
    Mat mask_hsv = Mat::zeros(imagesize,CV_8UC1);
    mask_hsv = ((image_Hue<H_max) | (image_Hue>H_min)) & (image_Saturation>S) & (image_Value>V); //opencv uses hue degrees divided by 2
    Mat mask_merge_hsv;
    vector<Mat> mask_3C_hsv;
    mask_3C_hsv.push_back(mask_hsv);
    mask_3C_hsv.push_back(mask_hsv);
    mask_3C_hsv.push_back(mask_hsv);
    merge(mask_3C_hsv,mask_merge_hsv);
    Mat image_seg_hsv;
    image_seg_hsv = image & mask_merge_hsv;
    imshow("HSV segmentation",image_seg_hsv);

    ///Part 4: post-processing on mask
    // Perform some binary operations on mask to remove noise
    erode(mask_hsv,mask_hsv,Mat(),Point(-1,-1),5);
    dilate(mask_hsv,mask_hsv,Mat(),Point(-1,-1),5);
    //now join separated parts
    dilate(mask_hsv,mask_hsv,Mat(),Point(-1,-1),5);
    erode(mask_hsv,mask_hsv,Mat(),Point(-1,-1),5);
    //Approximate with convexhull based on contour retrieval
    vector<vector<Point>> contours;
    findContours(mask_hsv,contours, CV_RETR_EXTERNAL,CV_CHAIN_APPROX_NONE);
    // look for largest contour area
    double largest_contour_area = 0.0;
    int largest_contour_index = 0;
    for(size_t i = 0; i < contours.size();i++)
    {
        double a = contourArea(contours[i],false); //false, do not use orientation
        if(a > largest_contour_area)
        {
            largest_contour_area  = a;
            largest_contour_index = i;
        }
    }
    vector<vector<Point>> hulls;
    vector<Point> hull;
    convexHull(contours[largest_contour_index],hull);
    hulls.push_back(hull);
    drawContours(mask_hsv,hulls,-1,255,CV_FILLED);
    // Merge mask in 3-channel vector to apply on BGR (3 channel) image
    // but first clear mask_3C_hsv vector
    mask_3C_hsv.clear();
    mask_3C_hsv.push_back(mask_hsv);
    mask_3C_hsv.push_back(mask_hsv);
    mask_3C_hsv.push_back(mask_hsv);
    merge(mask_3C_hsv,mask_merge_hsv);
    image_seg_hsv = image & mask_merge_hsv;
    imshow("HSV segmentation after postprocessing Mask",image_seg_hsv);
}
// If codeblocks decides to crash again, set these as programs arguments:
/*
--image1="/media/josse/Fedora/home/jossevandelm/beeldinterpretatie/2018_labo_beeldinterpretatie_VanDelm_Josse/sessie_2/sign.jpg"
--image2="/media/josse/Fedora/home/jossevandelm/beeldinterpretatie/2018_labo_beeldinterpretatie_VanDelm_Josse/sessie_2/sign2.jpg"
--image3="/media/josse/Fedora/home/jossevandelm/beeldinterpretatie/2018_labo_beeldinterpretatie_VanDelm_Josse/sessie_2/sign3.jpg"
--image4="/media/josse/Fedora/home/jossevandelm/beeldinterpretatie/2018_labo_beeldinterpretatie_VanDelm_Josse/sessie_2/sign4.jpg"
*/
