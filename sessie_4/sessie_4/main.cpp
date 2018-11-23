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
    for (size_t i = 0; i < count; i++) {
      // find max
      // find min (not zero)
      // reject points not smaller than min*3
    }
    //beste match = max kleinste distance en vice versa
    //thresholden distances (alles verder dan 3 keer min distance weggooien)
    //distances (goede matches) gebruiken om punten te definieren en daarmee homografie te berekenen
    //findHomography
    //doe homographt met perspective transform
    //teken punten op match image!
}
