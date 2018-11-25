#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

/* Program options:
--matching_image="/home/student/Github/2018_labo_beeldinterpretatie_VanDelm_Josse/sessie_4/kinderbueno_image.png"
--template="/home/student/Github/2018_labo_beeldinterpretatie_VanDelm_Josse/sessie_4/kinderbueno_object.png"
*/
///LINKER: `pkg-config opencv --libs`

int main(int argc, const char** argv){
    ///TASK 1: DETECT LOCAL FEATURES IN THE INPUT IMAGE
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
    waitKey(0);
    /// Search for features with ORB, BRISK and AKAZE.:
    // TODO AKAZE/BRISK
    /// ORB -------------------------------------------
    // Construct detectors,keypointvectors,declare descriptors and images
    Ptr<ORB> detector_ORB = ORB::create();
    vector<KeyPoint> keypoints_scene_ORB, keypoints_object_ORB;
    Mat descriptors_scene_ORB, descriptors_object_ORB;
    Mat template_image_draw_keypoints_ORB, matching_image_draw_keypoints_ORB;
    // Detect keypoints and compute descriptors (Mat() for no mask)
    detector_ORB->detectAndCompute(template_image,Mat(),keypoints_object_ORB,descriptors_object_ORB);
    detector_ORB->detectAndCompute(matching_image,Mat(),keypoints_scene_ORB,descriptors_scene_ORB);
    // Draw detected Keypoints
    drawKeypoints(template_image,keypoints_object_ORB,template_image_draw_keypoints_ORB);
    drawKeypoints(matching_image,keypoints_scene_ORB,matching_image_draw_keypoints_ORB);
    // Match the keypoints with bruteforce
    BFMatcher matcher(NORM_L2);
    vector<DMatch> matches_ORB;
    matcher.match(descriptors_scene_ORB,descriptors_object_ORB,matches_ORB);
    // Draw matches
    Mat img_matches_ORB;
    drawMatches(matching_image ,keypoints_scene_ORB,template_image,keypoints_object_ORB,matches_ORB, img_matches_ORB);

    imshow("Key Points (ORB) - object",template_image_draw_keypoints_ORB);
    imshow("Key Points (ORB) - scene ",matching_image_draw_keypoints_ORB);
    imshow("Matches    (ORB)",img_matches_ORB);
    waitKey(0);

    //BRISK en AKAZE kan fout geven bij bruteforce matching met L2_NORM
    Ptr<BRISK> detector_BRISK = BRISK::create();
    Ptr<AKAZE> detector_AKAZE = AKAZE::create();

    //  Early rejection on distances
    // set first distance as min and max, unless minimum is 0
    float minimal_distance,maximal_distance;
    minimal_distance = matches_ORB[0].distance;
    if(minimal_distance == 0)
    {
	    minimal_distance = 1;
    }
    maximal_distance = matches_ORB[0].distance;
    for (size_t i = 1; i < matches_ORB.size(); i++)
    {
	float distance = matches_ORB[i].distance;
	if(distance > maximal_distance)
	{
		maximal_distance = distance;
	}
	else if(distance < minimal_distance && distance !=0)
	{
		minimal_distance = distance;
	}
    }
    // Treshold the found distances (only keep smaller than 3*minimum_distance)
    // for early rejection of matches.
    vector<DMatch> approved_matches;
    for (size_t i = 0; i < matches_ORB.size(); i++)
    {
	   if(matches_ORB[i].distance <= minimal_distance*3)
	   {
		   approved_matches.push_back(matches_ORB[i]);
	   }
    }
    vector<Point2f> object_points;
    vector<Point2f> scene_points;
    for (size_t i = 0; i < approved_matches.size();i++)
    {
	    int queryIndex = approved_matches[i].queryIdx;
	    int trainIndex = approved_matches[i].trainIdx;
	    object_points.push_back(keypoints_object_ORB[queryIndex].pt);
	    scene_points.push_back(keypoints_scene_ORB[trainIndex].pt);
    }
    //find the Homography of the object image on the scene image
    Mat Homography = findHomography(object_points,scene_points,RANSAC);
    // use the corners of the object image to map a rectangle on the scene image:
    vector<Point2f> object_corners;
    object_corners.push_back(Point(0,0));
    object_corners.push_back(Point(0,template_image.rows));
    object_corners.push_back(Point(template_image.cols,template_image.rows));
    object_corners.push_back(Point(template_image.cols,0));
    vector<Point2f> scene_corners;
    // Apply homography on object corners to find the corners in the scene
    perspectiveTransform(object_corners,scene_corners,Homography);
    // Draw transformed points on match image (slightly translate images to set them on the first image only)
    Scalar green = Scalar(0,255,0);
    for(size_t i = 0; i < object_corners.size(); i++)
    {
        cerr << object_corners[i] << endl;
    }
    for(size_t i = 0; i < object_corners.size(); i++)
    {
        cerr << scene_corners[i] << endl;
    }
    line(matching_image,scene_corners[0],scene_corners[1],green);
    line(matching_image,scene_corners[1],scene_corners[2],green);
    line(matching_image,scene_corners[2],scene_corners[3],green);
    line(matching_image,scene_corners[3],scene_corners[0],green);
    imshow("We found it !",matching_image);
    waitKey(0);
}
