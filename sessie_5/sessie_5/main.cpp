#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;
using namespace cv::ml;

/* Program options:
--training_image="/media/josse/Fedora/home/jossevandelm/beeldinterpretatie/2018_labo_beeldinterpretatie_VanDelm_Josse/sessie_5/strawberry1.tif"
--inference_image="/media/josse/Fedora/home/jossevandelm/beeldinterpretatie/2018_labo_beeldinterpretatie_VanDelm_Josse/sessie_5/strawberry2.tif"
*/
///LINKER: `pkg-config opencv --libs`
// in param de lijst meegeven

void addPoint(vector<Point2d> *Pointsptr,int x, int y){
    cerr << "Point (" << x << ","<< y << ") added" << endl;
    Pointsptr->push_back(Point(x,y));
}

void deletePoint(vector<Point2d> *Pointsptr){
    if(Pointsptr->size() == 0)
        cerr << "No (more) points to delete" << endl;
    else
    {
        cerr << "Latest point deleted" << endl;
        Pointsptr->pop_back();
    }
}

void showPoints(vector<Point2d>  *Pointsptr){
    vector<Point2d> Points = *Pointsptr;
    cerr << "Showing selected points" << endl << "=======================" << endl;
    for(size_t i = 0; i < Points.size(); i++){
        cerr << i << ". (" << Points[i].x << "," << Points[i].y << ")" << endl;
    }
}

static void onMouse(int event,int x, int y, int, void* param){
    vector<Point2d> *Points = static_cast<vector<Point2d> *>(param);
    switch(event) {
    case EVENT_LBUTTONDOWN : addPoint(Points,x,y);    break;
    case EVENT_RBUTTONDOWN : deletePoint(Points);  break;
    case EVENT_MBUTTONDOWN : showPoints(Points);   break;
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
    // create vectors containing selected points
    vector<Point2d> foregroundPoints;
    vector<Point2d> backgroundPoints;
    imshow("Add training data",training_image);
    setMouseCallback("Add training data",onMouse,&foregroundPoints);
    cerr << "Press any key to continue in program" << endl;
    cerr << "******************" << endl << "SELECT FOREGROUND" << endl << "******************" << endl;
    waitKey(0);
    setMouseCallback("Add training data",onMouse,&backgroundPoints);
    cerr << "******************" << endl << "SELECT BACKGROUND" << endl << "******************" << endl;
    waitKey(0);

    // Add gaussian blur to the image, to reduce effects of outliers (pits of strawberries)
    Mat img_gaussian;
    GaussianBlur(training_image,img_gaussian,Size(11,11),0,0);
    /*imshow("Filtered image",img_gaussian);
    waitKey(0);*/
    Mat img_hsv;
    cvtColor(img_gaussian,img_hsv,CV_BGR2HSV);

    //Prepare foreground training data using the HSV values as a descriptor
    Mat trainingDataForeground(foregroundPoints.size(),3,CV_32FC1);
    Mat labels_fg = Mat::ones(foregroundPoints.size(),1,CV_32SC1);

    for (size_t i = 0; i < foregroundPoints.size(); i++) {
        Vec3b descriptor = img_hsv.at<Vec3b>(foregroundPoints[i].y,foregroundPoints[i].x);
        trainingDataForeground.at<float>(i,0) = descriptor[0];
        trainingDataForeground.at<float>(i,1) = descriptor[1];
        trainingDataForeground.at<float>(i,2) = descriptor[2];
    }

    //Prepare background training data using the HSV values as a descriptor
    Mat trainingDataBackground(backgroundPoints.size(),3,CV_32FC1); //strawberry zijn de punten geselecteerd.
    Mat labels_bg = Mat::zeros(backgroundPoints.size(),1,CV_32SC1);

    for (size_t i = 0; i < backgroundPoints.size(); i++) {
        Vec3b descriptor = img_hsv.at<Vec3b>(backgroundPoints[i].y,backgroundPoints[i].x);
        trainingDataBackground.at<float>(i,0) = descriptor[0];
        trainingDataBackground.at<float>(i,1) = descriptor[1];
        trainingDataBackground.at<float>(i,2) = descriptor[2];
    }

    // group foreground and background
    Mat trainingData, labels;
    vconcat(trainingDataForeground,trainingDataBackground,trainingData);
    vconcat(labels_fg,labels_bg,labels);
    cerr << endl;
    //Now train different classifiers
    /// Nearest Neighbor classifier
    Ptr<KNearest> KNN = KNearest::create();
    KNN->setIsClassifier(true);                 // use classifier instead of regression
    KNN->setAlgorithmType(KNearest::BRUTE_FORCE); // we use brute force instead of KDTREE
    KNN->setDefaultK(3);                        // use 3 classes
    Ptr<TrainData> trainDataKNN = TrainData::create(trainingData, ROW_SAMPLE, labels); //trainingdata is stored as column vector
    cerr << "Training K Nearest Neighbors Classifier...  ";
    KNN->train(trainDataKNN);
    cerr << "done" << endl;

    /// Normal Bayes classifier
    Ptr<NormalBayesClassifier> NB = NormalBayesClassifier::create();
    cerr << "Training Normal Bayes Classifier...  ";
    NB->train(trainingData, ROW_SAMPLE,labels);
    cerr << "done" << endl;

    /// Support Vector Machine classifier
    Ptr<SVM> SVM = SVM::create();
    SVM->setType(SVM::C_SVC); //we choose C-Support Vector Classification
    SVM->setKernel(SVM::LINEAR);
    SVM->setTermCriteria(TermCriteria(TermCriteria::MAX_ITER,100,1e-6));
    cerr << "Training Support Vector Machine Classifier...  ";
    SVM->train(trainingData,ROW_SAMPLE,labels);
    cerr << "done" << endl << endl;

    //after training we want the classifiers to label each indivudal pixel of the image;

    Size imageSize = Size(training_image.cols,training_image.rows);
    Mat mask_KNN = Mat::zeros(imageSize,CV_8UC1);
    Mat mask_NB  = Mat::zeros(imageSize,CV_8UC1);
    Mat mask_SVM = Mat::zeros(imageSize,CV_8UC1);
    Mat labels_KNN, labels_NB, labels_SVM;
    cerr << "Classifying individual pixels...  ";
    for(int row = 0; row < imageSize.height ; row++){
        for(int col = 0; col < imageSize.width; col++){
            // store individual pixel in CV_32FC1 matrix)
            Vec3b pixel = img_hsv.at<Vec3b>(row,col);
            Mat pixel_new(1,3,CV_32FC1);
            pixel_new.at<float>(0,0) = pixel[0];
            pixel_new.at<float>(0,1) = pixel[1];
            pixel_new.at<float>(0,2) = pixel[2];
            KNN->findNearest(pixel_new, KNN->getDefaultK(), labels_KNN);
             NB->predict(pixel_new, labels_NB );
            SVM->predict(pixel_new, labels_SVM);
            // Now use label as masks
            mask_KNN.at<uchar>(row,col) = labels_KNN.at<float>(0,0);
            mask_NB.at<uchar>(row,col) =  labels_NB.at<int>(0,0); //nb uses ints!
            mask_SVM.at<uchar>(row,col) = labels_SVM.at<float>(0,0);
        }
    }
    cerr << "done" << endl;
    imshow("Mask k Nearest Neighbor Classifier",mask_KNN*255);
    imshow("Mask Normal Bayes Classifier",mask_NB*255);
    imshow("Mask Support Vector Machine Classifier",mask_SVM*255);
    waitKey(0);
    destroyAllWindows(); // close older windows
    // apply masks on original image
    Mat img_masked_KNN,img_masked_NB, img_masked_SVM;
    training_image.copyTo(img_masked_KNN,mask_KNN);
    training_image.copyTo(img_masked_NB,mask_NB);
    training_image.copyTo(img_masked_SVM,mask_SVM);
    imshow("k Nearest Neighbors Classifier",img_masked_KNN);
    imshow("Normal Bayes Classifier",img_masked_NB);
    imshow("Support Vector Machine Classifier",img_masked_SVM);
    waitKey(0);
    destroyAllWindows(); // close older windows
    //Now do the same classification on inference image
    GaussianBlur(inference_image,img_gaussian,Size(11,11),0,0);
    cvtColor(img_gaussian,img_hsv,CV_BGR2HSV);
    imageSize = Size(inference_image.cols,inference_image.rows);
    mask_KNN = Mat::zeros(imageSize,CV_8UC1);
    mask_NB  = Mat::zeros(imageSize,CV_8UC1);
    mask_SVM = Mat::zeros(imageSize,CV_8UC1);
    cerr << endl << "|Inference| " << endl << endl;
    cerr << "Classifying individual pixels...  ";
    for(int row = 0; row < imageSize.height ; row++){
        for(int col = 0; col < imageSize.width; col++){
            // store individual pixel in CV_32FC1 matrix)
            Vec3b pixel = img_hsv.at<Vec3b>(row,col);
            Mat pixel_new(1,3,CV_32FC1);
            pixel_new.at<float>(0,0) = pixel[0];
            pixel_new.at<float>(0,1) = pixel[1];
            pixel_new.at<float>(0,2) = pixel[2];
            KNN->findNearest(pixel_new, KNN->getDefaultK(), labels_KNN);
             NB->predict(pixel_new, labels_NB );
            SVM->predict(pixel_new, labels_SVM);
            // Now use label as masks
            mask_KNN.at<uchar>(row,col) = labels_KNN.at<float>(0,0);
            mask_NB.at<uchar>(row,col) =  labels_NB.at<int>(0,0); //nb uses ints!
            mask_SVM.at<uchar>(row,col) = labels_SVM.at<float>(0,0);
        }
    }
    cerr << "done" << endl;
    imshow("Mask k Nearest Neighbor Classifier",mask_KNN*255);
    imshow("Mask Normal Bayes Classifier",mask_NB*255);
    imshow("Mask Support Vector Machine Classifier",mask_SVM*255);
    waitKey(0);
    destroyAllWindows(); // close older windows
    // apply masks on original image
    inference_image.copyTo(img_masked_KNN,mask_KNN);
    inference_image.copyTo(img_masked_NB,mask_NB);
    inference_image.copyTo(img_masked_SVM,mask_SVM);
    imshow("k Nearest Neighbors Classifier",img_masked_KNN);
    imshow("Normal Bayes Classifier",img_masked_NB);
    imshow("Support Vector Machine Classifier",img_masked_SVM);
    waitKey(0);
    return 0;
}
