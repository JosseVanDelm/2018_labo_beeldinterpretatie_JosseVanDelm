#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

/*vvv | Error marked with FIXME below | vvv*/

/*Current error (@ Runtime):
 *
OpenCV(3.4.1) Error: Sizes of input arguments do not match (The operation is neither 'array op array' (where arrays have the same size and type), nor 'array op scalar', nor 'scalar op array') in binary_op, file /builddir/build/BUILD/opencv-3.4.1/modules/core/src/arithm.cpp, line 225
terminate called after throwing an instance of 'cv::Exception'
  what():  OpenCV(3.4.1) /builddir/build/BUILD/opencv-3.4.1/modules/core/src/arithm.cpp:225: error: (-209) The operation is neither 'array op array' (where arrays have the same size and type), nor 'array op scalar', nor 'scalar op array' in function binary_op

Process returned -1 (0xFFFFFFFF)   execution time : 10.267 s
Press ENTER to continue.
*/

///Program options: --matching_image="/home/student/Github/2018_labo_beeldinterpretatie_VanDelm_Josse/sessie_3/recht.jpg"
///                 --template="/home/student/Github/2018_labo_beeldinterpretatie_VanDelm_Josse/sessie_3/template.jpg"
///or:              --matching_image="/home/jossevandelm/beeldinterpretatie/2018_labo_beeldinterpretatie_VanDelm_Josse/sessie_3/recht.jpg"
///                 --template="/home/jossevandelm/beeldinterpretatie/2018_labo_beeldinterpretatie_VanDelm_Josse/sessie_3/template.jpg"
///LINKER: `pkg-config opencv --libs`

int main(int argc, const char** argv){
    ///TASK 1: USE TEMPLATE MATCHING TO FIND AN OBJECT IN THE INPUT IMAGE
    // Adding a command line parser
    CommandLineParser parser(argc, argv,
        "{help h usage ?                   | | show this message }"
        "{matching_image matching_image    | | (required) path to input matching image}"
        "{template template                | | (required) path to input image template in color }"
    );
    // show help if help is asked
    if (parser.has("help")){
        parser.printMessage();
        cerr << "Tip: use absolute paths to avoid any problems with the software" << endl;
        return 0;
    }
    // parse arguments and check if they are entered correctly
    string matching_image_location = parser.get<string>("matching_image");
    string template_location(parser.get<string>("template"));
    if (matching_image_location.empty()|| template_location.empty()){
        cerr << "There must be something wrong with your arguments. Please check again." << endl;
        parser.printMessage();
        return -1;
    }
    Mat matching_image = imread(matching_image_location);
    Mat template_image = imread(template_location);
    if(matching_image.empty() || template_image.empty()){
        cerr<<"One of the images is not found";
        return -1;
    }
    // display source images
    imshow("Source Image",matching_image);
    imshow("Template",template_image);
    waitKey();
    // Search for matches:
    Mat result;
    matchTemplate(matching_image.clone(),template_image,result, TM_SQDIFF);
    // Makes maxima from minima
    normalize(result,result,0,1,NORM_MINMAX,-1,Mat());
    result = 1-result;
    imshow("Match map",result);
    waitKey();
    // find the maximum
    double minVal,maxVal;
    Point minLoc,maxLoc,matchLoc;
    minMaxLoc( result, &minVal, &maxVal, &minLoc, &maxLoc, Mat() );
    // Display the maximum

    Mat display_map;
    result.copyTo(display_map);
    circle(display_map,maxLoc,6,1, 1);
    Mat display_image;
    matching_image.copyTo(display_image);
    Size template_size = template_image.size();
    rectangle(display_image,maxLoc,Point(maxLoc.x+template_size.width,maxLoc.y+template_size.height),Scalar(0,255,0),1);
    imshow("Template Matching",display_image);
    imshow("Match map with maximum",display_map);
    waitKey();
    /// TASK 2: MORE ADVANCED APPROACH
    // display source images
    imshow("Source Image Advanced",matching_image);
    imshow("Template Advanced",template_image);
    waitKey();
    //only use grayscale images to perform matching
    Mat matching_image_gs;
    Mat template_image_gs;
    cvtColor(matching_image,matching_image_gs,CV_BGR2GRAY);
    cvtColor(template_image,template_image_gs,CV_BGR2GRAY);
    imshow("Source image grayscale",matching_image_gs);
    imshow("Template image grayscale",template_image_gs);
    waitKey();
    // Search for matches:
    Mat result_gs;
    // Use TM_CCOEFF for better results
    matchTemplate(matching_image_gs.clone(),template_image_gs,result_gs, TM_CCOEFF);
    // Makes maxima from minima
    normalize(result_gs,result_gs,0,1,NORM_MINMAX,-1,Mat());
    //result_gs = 1-result_gs;
    imshow("Match map Grayscale",result_gs);
    waitKey();
    // find the different maxima
    Mat mask = Mat::zeros(Size(result_gs.cols,result_gs.rows),CV_8UC1);
    inRange(result_gs, 0.70, 1, mask);
    //threshold(result_gs,mask,0.70,1,THRESH_BINARY);
    imshow("Mask on matches",mask);
    waitKey();
    //Now use mask to find local maxima
    vector<vector<Point>> contours;
    vector<Point> maxima;
    double minValue,maxValue;
    Point minLocation,maxLocation;
    findContours(mask,contours,CV_RETR_EXTERNAL,CV_CHAIN_APPROX_NONE);
    //Loop over different blobs separately
    for(size_t i = 0; i < contours.size();i++)
    {
		// Create empty mask for each individual blob
        Mat individual_mask = Mat::zeros(Size(result_gs.cols,result_gs.rows),CV_8UC1);
        drawContours(individual_mask,contours,i,1,CV_FILLED);
        imshow("Various blobs",individual_mask*255);
        imshow("result_1C_gs",result_gs);
        waitKey(0);
        Mat masked = Mat::zeros(Size(result_gs.cols,result_gs.rows),CV_8UC1);
		// Apply mask on match map
		result_gs.copyTo(masked, individual_mask);
        imshow("masked",masked);
        waitKey(0);
		// These rows were added for debugging purposes
        cerr << individual_mask.rows << endl << individual_mask.cols << endl;
        cerr << "++++++" << endl;
        cerr << result_gs.rows << endl << result_gs.cols << endl;
        cerr << "======" << endl;
        waitKey(0);
		// Find local maximum for each blob
       	minMaxLoc(masked,&minValue,&maxValue,&minLocation,&maxLocation);
		// Add found position of local maximum to array
        maxima.push_back(maxLocation);
    }
	// Make final display image
	Mat display_multiple_result;
	matching_image.copyTo(display_multiple_result);
    for(size_t i = 0; i < maxima.size(); i++)
	{
		// Draw rectangles on found maximum positions
    	rectangle(display_multiple_result,maxima[i],Point(maxima[i].x+template_size.width,maxima[i].y+template_size.height),Scalar(0,255,0),1);
	}
	// Display final image
	imshow("Final image with all matches",display_multiple_result);
	waitKey();
}
