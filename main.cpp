#include "libs/libvision/PreProcess.h"
#include "libs/libobjects/Robo.h"
#include "libs/libobjects/Ball.h"
#include "libs/globals/Global.h"
#include "libs/libvision/Colors.h"
#include "libs/helpers/helper.h"

#include "libs/interfaces/PreProcessInterface.hpp"
#include "libs/interfaces/SegmentationInterface.hpp"
#include "libs/interfaces/ExtractionInterface.hpp"
#include "libs/interfaces/SegExInterface.hpp"

#include "classes/Pipe1/PreProcess1/PreProcessPipe1.hpp"
#include "classes/Pipe1/SegEx/SegexPipe1.hpp"
#include "classes/Pipe1/SegEx/HSVSegmentation/SegmentationPipe1.hpp"
#include "classes/Pipe1/SegEx/Extraction/ExtractionPipe1.hpp"

#include <string>
#include <iostream>

#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

using namespace cv;

void setHDefMin(int pos, void *data);
void setSDefMin(int pos, void *data);

void setHDefMax(int pos, void *data);
void setSDefMax(int pos, void *data);

void setHDirectMin(int pos, void *data);
void setSDirectMin(int pos, void *data);

void setHDirectMax(int pos, void *data);
void setSDirectMax(int pos, void *data);

void robot1TrackbarsCreator(std::string window_name, void *data);
void robot2TrackbarsCreator(std::string window_name, void *data);
void robot3TrackbarsCreator(std::string window_name, void *data);

void enemyTrackbarsCreator(std::string window_name, void *data);

void trackObjs();

struct CallbackData
{
    int id;
    Mat *original;
    Mat *outDef;
    Mat *outDir;
};

int H1DefMin = 0, H1DefMax = 179;
int S1DefMin = 0, S1DefMax = 255;
int V1DefMin = 0, V1DefMax = 255;
int H1DirMin = 0, H1DirMax = 179;
int S1DirMin = 0, S1DirMax = 255;
int V1DirMin = 0, V1DirMax = 255;

int H2DefMin = 0, H2DefMax = 179;
int S2DefMin = 0, S2DefMax = 255;
int V2DefMin = 0, V2DefMax = 255;
int H2DirMin = 0, H2DirMax = 179;
int S2DirMin = 0, S2DirMax = 255;
int V2DirMin = 0, V2DirMax = 255;

int H3DefMin = 0, H3DefMax = 179;
int S3DefMin = 0, S3DefMax = 255;
int V3DefMin = 0, V3DefMax = 255;
int H3DirMin = 0, H3DirMax = 179;
int S3DirMin = 0, S3DirMax = 255;
int V3DirMin = 0, V3DirMax = 255;

const std::string original_window = "Original";

const std::string window_ally_1 = "Ally 1";
const std::string out_dir_1 = "Ally Dir 1";

/* const std::string window_ally_2 = "Ally 2";
const std::string out_dir_2 = "Ally Dir 2";

const std::string window_ally_3 = "Ally 3";
const std::string out_dir_3 = "Ally Dir 3";

const std::string window_enemy_1 = "Enemy 1";
const std::string window_enemy_2 = "Enemy 2";
const std::string window_enemy_3 = "Enemy 3"; */

Colors *colors;

int val = 0;

int main()
{
    VideoCapture cap;

    Mat original, hsvImage, filteredImg;

    Mat allyDefOut1, allyDirOut1;

    std::vector<cv::Scalar> draw_colors = {cv::Scalar(233,66,245), cv::Scalar(40,115,72), cv::Scalar(118,145,48)};

    /* 
    Mat allyDefOut2, allyDirOut2;
    Mat allyDefOut3, allyDirOut3;

    Mat enemyOut1, enemyOut2, enemyOut3 
    */

    std::string videoPath = "../videos/video_robos.webm";

    cap.open(videoPath);

    if (!cap.isOpened())
    {
        std::cout << "Erro ao abrir vídeo" << std::endl;
        exit(-1);
    }

    /* INICIALIZAÇÕES */

    std::vector<CallbackData *> ally_data = {new CallbackData}; //, new CallbackData, new CallbackData};
    //std::vector<CallbackData *> enemy_data = {new CallbackData, new CallbackData, new CallbackData};

    colors = Global::getColors();

    //auto enemies = Global::getEnemyRobots();
    //auto ball = Global::getBall();

    allyDefOut1 = Mat::zeros(original.size(), CV_8UC1);
    allyDirOut1 = Mat::zeros(original.size(), CV_8UC1);

    /*
    allyDefOut2 = Mat::zeros(original.size(), CV_8UC1);
    allyDirOut2 = Mat::zeros(original.size(), CV_8UC1);

    allyDefOut3 = Mat::zeros(original.size(), CV_8UC1);
    allyDirOut3 = Mat::zeros(original.size(), CV_8UC1);

    enemyOut1 = Mat::zeros(original.size(), CV_8UC1);
    enemyOut2 = Mat::zeros(original.size(), CV_8UC1);
    enemyOut3 = Mat::zeros(original.size(), CV_8UC1);
    */

    for (int i = 0; i < 1; i++)
    {
        ally_data[i]->original = &hsvImage;
        ally_data[i]->id = i;
    }

    ally_data[0]->outDef = &allyDefOut1;
    ally_data[0]->outDir = &allyDirOut1;

    //ally_data[1]->outDef = &allyDefOut2;
    //ally_data[1]->outDir = &allyDirOut2;

    //ally_data[2]->outDef = &allyDefOut3;
    //ally_data[2]->outDir = &allyDirOut3;

    /* FIM INICIALIZAÇÕES */

    /* JANELAS */
    namedWindow(original_window, WINDOW_NORMAL);

    namedWindow(window_ally_1, WINDOW_NORMAL);
    //namedWindow(window_ally_2);
    //namedWindow(window_ally_3);

    namedWindow(out_dir_1, WINDOW_NORMAL);
    //namedWindow(out_dir_2);
    //namedWindow(out_dir_3);

    namedWindow("Run Track");
    /*
    namedWindow(window_enemy_1);
    namedWindow(window_enemy_2);
    namedWindow(window_enemy_3);
    */
    /* FIM JANELAS */

    // Chamando as funções
    robot1TrackbarsCreator(window_ally_1, (void *)ally_data[0]);
    /* robot2TrackbarsCreator(window_ally_2, (void*) ally_data[1]);
    robot3TrackbarsCreator(window_ally_3, (void*) ally_data[2]); */

    trackObjs();

    while (1)
    {

        cap >> original;

        imshow(window_ally_1, allyDefOut1);
        imshow(out_dir_1, allyDirOut1);

        //imshow(window_ally_2, allyDefOut2);
        //imshow(out_dir_2, allyDirOut2);

        //imshow(window_ally_3, allyDefOut3);
        //imshow(out_dir_3, allyDirOut3);

        if (val)
        {
            PreProcessPipe1 preprocess1 = PreProcessPipe1();
            SegmentationPipe1 segmentation1 = SegmentationPipe1();
            ExtractionPipe1 extraction1 = ExtractionPipe1();
            SegexPipe1 segex1 = SegexPipe1(segmentation1, extraction1);

            preprocess1.execute(original, filteredImg);
            segex1.execute(filteredImg);

            for (int i = 0; i < 1; i++)
            {
                Robo *r = Global::getAlliedRobots()[i];

                helpers::drawObject(r->getPosX(), r->getPosY(), cv::Scalar(100,0,255), original);
            }
            
        }
        else
        {
            //calibragem

            PreProcess::applyBlur(original, original, 3, PreProcess::smoothType::AVERAGE);

            cvtColor(original, hsvImage, COLOR_BGR2HSV);
        }

        imshow(original_window, original);

        waitKey(10);
    }

    destroyAllWindows();

    return 0;
}

void trackObjs()
{

    createTrackbar("track", "Run Track", &val, 1);
}

void enemyTrackbarsCreator(std::string window_name, void *data)
{
    static int HEnemyDefMin = 0, HEnemyDefMax = 179;
    static int SEnemyDefMin = 0, SEnemyDefMax = 255;
    static int VEnemyDefMin = 0, VEnemyDefMax = 255;

    createTrackbar("H Def Min", window_name, &HEnemyDefMin, 255, setHDefMin, data);
    createTrackbar("H Def Max", window_name, &HEnemyDefMax, 179, setHDefMax, data);
    createTrackbar("S Def Min", window_name, &SEnemyDefMin, 255, setSDefMin, data);
    createTrackbar("S Def Max", window_name, &SEnemyDefMax, 255, setSDefMax, data);
}

void robot1TrackbarsCreator(std::string window_name, void *data)
{

    createTrackbar("H Def Min", window_name, &H1DefMin, 179, setHDefMin, data);
    createTrackbar("H Def Max", window_name, &H1DefMax, 179, setHDefMax, data);
    createTrackbar("S Def Min", window_name, &S1DefMin, 255, setSDefMin, data);
    createTrackbar("S Def Max", window_name, &S1DefMax, 255, setSDefMax, data);

    createTrackbar("H Dir Min", window_name, &H1DirMin, 179, setHDirectMin, data);
    createTrackbar("H Dir Max", window_name, &H1DirMax, 179, setHDirectMax, data);
    createTrackbar("S Dir Min", window_name, &S1DirMin, 255, setSDirectMin, data);
    createTrackbar("S Dir Max", window_name, &S1DirMax, 255, setSDirectMax, data);
}

void robot2TrackbarsCreator(std::string window_name, void *data)
{

    createTrackbar("H Def Min", window_name, &H2DefMin, 179, setHDefMin, data);
    createTrackbar("H Def Max", window_name, &H2DefMax, 179, setHDefMax, data);
    createTrackbar("S Def Min", window_name, &S2DefMin, 255, setSDefMin, data);
    createTrackbar("S Def Max", window_name, &S2DefMax, 255, setSDefMax, data);

    createTrackbar("H Dir Min", window_name, &H2DirMin, 179, setHDirectMin, data);
    createTrackbar("H Dir Max", window_name, &H2DirMax, 179, setHDirectMax, data);
    createTrackbar("S Dir Min", window_name, &S2DirMin, 255, setSDirectMin, data);
    createTrackbar("S Dir Max", window_name, &S2DirMax, 255, setSDirectMax, data);
}

void robot3TrackbarsCreator(std::string window_name, void *data)
{

    createTrackbar("H Def Min", window_name, &H3DefMin, 179, setHDefMin, data);
    createTrackbar("H Def Max", window_name, &H3DefMax, 179, setHDefMax, data);
    createTrackbar("S Def Min", window_name, &S3DefMin, 255, setSDefMin, data);
    createTrackbar("S Def Max", window_name, &S3DefMax, 255, setSDefMax, data);

    createTrackbar("H Dir Min", window_name, &H3DirMin, 179, setHDirectMin, data);
    createTrackbar("H Dir Max", window_name, &H3DirMax, 179, setHDirectMax, data);
    createTrackbar("S Dir Min", window_name, &S3DirMin, 255, setSDirectMin, data);
    createTrackbar("S Dir Max", window_name, &S3DirMax, 255, setSDirectMax, data);
}

/* Seta cor do time */

/* Mínimos cor do time */

void setHDefMin(int pos, void *data)
{
    CallbackData *cb_data;
    Scalar hsv;

    if (data == NULL)
    {
        std::cout << "Error: Dado Null" << std::endl;
        exit(1);
    }

    cb_data = (CallbackData *)data;

    hsv = colors->getAllyMin();

    hsv[0] = pos;

    colors->setAllyMin(hsv);

    Scalar maxRange;

    maxRange = colors->getAllyMax();

    inRange(*cb_data->original, hsv, maxRange, *cb_data->outDef);
}

void setSDefMin(int pos, void *data)
{
    CallbackData *cb_data;
    Scalar hsv;

    if (data == NULL)
    {
        std::cout << "Error: Dado Null" << std::endl;
        exit(1);
    }

    cb_data = (CallbackData *)data;

    hsv = colors->getAllyMin();

    hsv[1] = pos;

    colors->setAllyMin(hsv);

    Scalar maxRange;

    maxRange = colors->getAllyMax();

    inRange(*cb_data->original, hsv, maxRange, *cb_data->outDef);
}

/* Máximos cor do time */

void setHDefMax(int pos, void *data)
{
    CallbackData *cb_data;
    Scalar hsv;

    if (data == NULL)
    {
        std::cout << "Error: Dado Null" << std::endl;
        exit(1);
    }

    cb_data = (CallbackData *)data;

    hsv = colors->getAllyMax();

    hsv[0] = pos;

    colors->setAllyMax(hsv);

    Scalar minRange;

    minRange = colors->getAllyMin();

    inRange(*cb_data->original, minRange, hsv, *cb_data->outDef);
}

void setSDefMax(int pos, void *data)
{
    CallbackData *cb_data;
    Scalar hsv;

    if (data == NULL)
    {
        std::cout << "Error: Dado Null" << std::endl;
        exit(1);
    }

    cb_data = (CallbackData *)data;

    hsv = colors->getAllyMax();

    hsv[1] = pos;

    colors->setAllyMax(hsv);

    Scalar minRange;

    minRange = colors->getAllyMin();

    inRange(*cb_data->original, minRange, hsv, *cb_data->outDef);
}

/* Seta cor do papel do robo */

/* Mínimos */
void setHDirectMin(int pos, void *data)
{
    CallbackData *cb_data;
    Scalar hsv;

    if (data == NULL)
    {
        std::cout << "Error: Dado Null" << std::endl;
        exit(1);
    }

    cb_data = (CallbackData *)data;

    hsv = colors->getRobotColorMin(cb_data->id);

    hsv[0] = pos;

    colors->setRobotColorMin(hsv, cb_data->id);

    Scalar maxRange;

    maxRange = colors->getRobotColorMax(cb_data->id);

    inRange(*cb_data->original, hsv, maxRange, *cb_data->outDir);
}

void setSDirectMin(int pos, void *data)
{
    CallbackData *cb_data;
    Scalar hsv;

    if (data == NULL)
    {
        std::cout << "Error: Dado Null" << std::endl;
        exit(1);
    }

    cb_data = (CallbackData *)data;

    hsv = colors->getRobotColorMin(cb_data->id);

    hsv[1] = pos;

    colors->setRobotColorMin(hsv, cb_data->id);

    Scalar maxRange;

    maxRange = colors->getRobotColorMax(cb_data->id);

    inRange(*cb_data->original, hsv, maxRange, *cb_data->outDir);
}

/* Máximos */
void setHDirectMax(int pos, void *data)
{
    CallbackData *cb_data;
    Scalar hsv;

    if (data == NULL)
    {
        std::cout << "Error: Dado Null" << std::endl;
        exit(1);
    }

    cb_data = (CallbackData *)data;

    hsv = colors->getRobotColorMax(cb_data->id);

    hsv[0] = pos;

    colors->setRobotColorMax(hsv, cb_data->id);

    Scalar minRange;

    minRange = colors->getRobotColorMin(cb_data->id);

    inRange(*cb_data->original, minRange, hsv, *cb_data->outDir);
}

void setSDirectMax(int pos, void *data)
{
    CallbackData *cb_data;
    Scalar hsv;

    if (data == NULL)
    {
        std::cout << "Error: Dado Null" << std::endl;
        exit(1);
    }

    cb_data = (CallbackData *)data;

    hsv = colors->getRobotColorMax(cb_data->id);

    hsv[1] = pos;

    colors->setRobotColorMax(hsv, cb_data->id);

    Scalar minRange;

    minRange = colors->getRobotColorMin(cb_data->id);

    inRange(*cb_data->original, minRange, hsv, *cb_data->outDir);
}
