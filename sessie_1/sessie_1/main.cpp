#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;




int main(int argc, const char** argv)
{
    // Adding a command line parser
    CommandLineParser parser(argc, argv,
        "{help h usage ?  | | show this message }"
        "{image @image    | | (required) path to input image}"
        "{mode m          | | (required) mode 0(2) (advanced) skin segmentation 1 text segmentation}"
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
        return -1;
    }
    // check mode
    int mode(parser.get<int>("mode"));
    if ((mode != 0) && (mode != 1) && (mode!= 2)){
        cerr << "You did not enter a valid mode. Please select a mode of 0,1 or 2." << endl;
        return -1;
    }

    /// MODE 0: Skin segmentation
    if (mode == 0){
        cerr<< "SKIN SEGMENTATION" <<endl;
        imshow("Original image",image);
        waitKey(0);
        //split channels for processing
        Mat separatechannels[3];
        split(image,separatechannels);
        Mat Blue = separatechannels[0];
        Mat Green = separatechannels[1];
        Mat Red = separatechannels[2];
        //create zero matrix for mask
        Size imagesize = image.size();
        Mat mask = Mat::zeros(imagesize,CV_8UC1);
        //use matrix operations to define mask
        mask =  (
                (Red>95) & (Green>40) & (Blue>20) &
                ((max(Red,max(Green,Blue)) - min(Red,min(Green,Blue)))>15) &
                (abs(Red-Green)>15) & (Red>Green) & (Red>Blue)
                ); //use this equation from literature to segment skin
        imshow("Mask",(mask*255)); //multiply values of 1 by 255 to make them white
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
        cerr<< "TEXT SEGMENTATION" <<endl;
        imshow("Original image",image);
        waitKey(0);
        //OTSU thresholding
        Mat image_gray;
        cvtColor(image,image_gray, COLOR_BGR2GRAY);
        Mat thresholded;
        threshold(image_gray, thresholded, 0, 255, CV_THRESH_BINARY | CV_THRESH_OTSU);
        imshow("Otsu Thresholding",thresholded);
        waitKey(0);
        //Histogram equalization for improving thresholding?
        Mat equalized;
        equalizeHist( image_gray, equalized);
        imshow("Histogram equalization",equalized);
        Mat thresholded_eq;
        // now try thresholding with otsu ==> image doesn't differ mucht from previous
        threshold(equalized, thresholded_eq, 0, 255, CV_THRESH_BINARY | CV_THRESH_OTSU);
        imshow("Otsu Thresholding With Histogram Equalization",thresholded_eq);
        waitKey(0);
        //CLAHE for improving thresholding?
        Mat equalized_clahe;
        Ptr<CLAHE> clahe = createCLAHE();
        clahe->setClipLimit(1);
        clahe->apply(image_gray,equalized_clahe);
        imshow("CLAHE",equalized_clahe);
        Mat thresholded_clahe;
        //now try thresholding with otsu ==> much better!
        threshold(equalized_clahe, thresholded_clahe, 0, 255, CV_THRESH_BINARY | CV_THRESH_OTSU);
        imshow("Otsu Thresholding With CLAHE",thresholded_clahe);
        waitKey(0);
        return 0;
    }
    ///OPDRACHT 1.2
    /// MODE 2: Skin segmentation with extras
    if (mode == 2){
        cerr<< "SKIN SEGMENTATION WITH EXTRAS" <<endl;
        imshow("Original image",image);
        waitKey(0);
        //split channels for processing
        Mat separatechannels[3];
        split(image,separatechannels);
        Mat Blue = separatechannels[0];
        Mat Green = separatechannels[1];
        Mat Red = separatechannels[2];
        //create zero matrix for mask
        Size imagesize = image.size();
        Mat mask = Mat::zeros(imagesize,CV_8UC1);
        //use matrix operations to define mask
        mask =  (
                (Red>95) & (Green>40) & (Blue>20) &
                ((max(Red,max(Green,Blue)) - min(Red,min(Green,Blue)))>15) &
                (abs(Red-Green)>15) & (Red>Green) & (Red>Blue)
                ); //use this equation from literature to segment skin
        imshow("Mask",(mask*255)); //multiply values of 1 by 255 to make them white
        waitKey(0);
        //Extra operations on mask for noise removal
        erode(mask,mask,Mat()); //remove white dots with erosion
        imshow("Erosion",mask);
        waitKey(0);
        //somewhat undo erosion with dilation
        dilate(mask,mask,Mat());
        imshow("Dilation after erosion = opening",mask)
        ;
        waitKey(0);
        //dilate again to join body parts
        dilate(mask,mask,Mat(),Point(-1,-1),5);// use more iterations of dilation
        imshow("Dilated again",mask);
        waitKey(0);
        //erode again to "unbold" the image
        erode(mask,mask,Mat(),Point(-1,-1),5); //eroda equally as much
        imshow("Dilation and then erosion = closing",mask);
        waitKey(0);
        //detect contours in the mask for convex hull algorithm
        vector<vector<Point>> contours;
        vector<Vec4i> hierarchy;
        findContours(mask,contours,hierarchy, CV_RETR_LIST,CV_CHAIN_APPROX_SIMPLE);
        //draw contours
        Mat drawed_contours = Mat::zeros(mask.size(),CV_8UC1);
        drawContours(drawed_contours,contours,-1,255); //-1 to draw all contours, set color to 255
        imshow("Contours",drawed_contours); //*255 to visualize ones
        waitKey(0);
        //convexhull approximation of the detected contours
        vector<vector<Point>> hulls;
        for(long unsigned int i = 0; i < (contours.size()); i++){ //long unsigned int for suppressing warning
            vector<Point> hull;
            convexHull(contours[i],hull);
            hulls.push_back(hull);
        }
        mask = Mat::zeros(mask.size(),CV_8UC1); //reset mask and draw hulls on mask
        drawContours(mask,hulls,-1,255,CV_FILLED); //use CV_FILLED to draw fill instead of line
        imshow("Convex Hull approximation of mask",mask);
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
}
/* SET PROGRAMS ARGUMENTS in Code::Blocks
--image="/home/jossevandelm/beeldinterpretatie/2018_labo_beeldinterpretatie_VanDelm_Josse/sessie_1/imageColor.jpg" --mode=0
--image="/home/jossevandelm/beeldinterpretatie/2018_labo_beeldinterpretatie_VanDelm_Josse/sessie_1/imageBimodal.jpg" --mode=1
--image="/home/jossevandelm/beeldinterpretatie/2018_labo_beeldinterpretatie_VanDelm_Josse/sessie_1/imageColorAdapted.jpg" --mode=2
*/
