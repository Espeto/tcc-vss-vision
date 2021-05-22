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
#include "classes/Pipe1/PreProcess2/PreProcess2Pipe1.hpp"
#include "classes/Pipe1/PreProcess3/PreProcess3Pipe1.hpp"

#include "classes/Pipe1/SegEx/SegexPipe1.hpp"
#include "classes/Pipe1/SegEx/HSSegmentation/SegmentationPipe1.hpp"
#include "classes/Pipe1/SegEx/Extraction/ExtractionPipe1.hpp"

#include <string>
#include <iostream>
#include <memory>

#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

using namespace cv;

void setHDefMin(int pos, void *data);
void setSDefMin(int pos, void *data);
void setVDefMin(int pos, void *data);

void setHDefMax(int pos, void *data);
void setSDefMax(int pos, void *data);
void setVDefMax(int pos, void *data);

void setHDirectMin(int pos, void *data);
void setSDirectMin(int pos, void *data);
void setVDirectMin(int pos, void *data);

void setHDirectMax(int pos, void *data);
void setSDirectMax(int pos, void *data);
void setVDirectMax(int pos, void *data);

void setBallHMin(int pos, void *data);
void setBallSMin(int pos, void *data);
void setBallVMin(int pos, void *data);

void setBallHMax(int pos, void *data);
void setBallSMax(int pos, void *data);
void setBallVMax(int pos, void *data);

void setHEnemyMin(int pos, void *data);
void setSEnemyMin(int pos, void *data);
void setVEnemyMin(int pos, void *data);

void setHEnemyMax(int pos, void *data);
void setSEnemyMax(int pos, void *data);
void setVEnemyMax(int pos, void *data);

void robot1TrackbarsCreator(std::string window_name, void *data);
void robot2TrackbarsCreator(std::string window_name, void *data);
void robot3TrackbarsCreator(std::string window_name, void *data);

void enemyTrackbarsCreator(std::string window_name, void *data);

void ballTrackbarsCreator(std::string window_name, void *data);

void trackObjs();

struct CallbackData
{
    int id;
    Mat *original;
    Mat *outDef;
    Mat *outDir;
};

// Team
int HDefMin = 0, HDefMax = 180;
int SDefMin = 0, SDefMax = 255;
int VDefMin = 0, VDefMax = 255;

// Players
int H1DirMin = 0, H1DirMax = 180;
int S1DirMin = 0, S1DirMax = 255;
int V1DirMin = 0, V1DirMax = 255;

int H2DirMin = 0, H2DirMax = 180;
int S2DirMin = 0, S2DirMax = 255;
int V2DirMin = 0, V2DirMax = 255;

int H3DirMin = 0, H3DirMax = 180;
int S3DirMin = 0, S3DirMax = 255;
int V3DirMin = 0, V3DirMax = 255;

int ballHMin = 0, ballHMax = 180;
int ballSMin = 0, ballSMax = 255;
int ballVMin = 0, ballVMax = 255;

const std::string original_window = "Original";

const std::string window_ally_1 = "Ally 1";
const std::string out_dir_1 = "Ally Dir 1";

const std::string out_dir_2 = "Ally Dir 2";

const std::string out_dir_3 = "Ally Dir 3";

const std::string ball_window = "Ball Window";

const std::string window_enemy = "Enemy";


Colors *colors;

int val = 0;

int main()
{
    VideoCapture cap;

    Mat original, hsvImage, filteredImg;
    Mat allyDefOut1, allyDirOut1;
    Mat allyDefOut2, allyDirOut2;
    Mat allyDefOut3, allyDirOut3;
    Mat ballOut;
    Mat enemyOut; 

    CallbackData ballCallback, enemy_data;
   
   int phc = 1;

    std::string videoPath = "../videos/escurecido.webm";

    cap.open(videoPath);

    if (!cap.isOpened())
    {
        std::cout << "Erro ao abrir vídeo" << std::endl;
        exit(-1);
    }

    cap >> original;

    /* INICIALIZAÇÕES */

    // PreProcessPipe1 preprocess = PreProcessPipe1();
    // PreProcess2Pipe1 preprocess = PreProcess2Pipe1();
    PreProcess3Pipe1 preprocess = PreProcess3Pipe1();
    auto segmentation1 = std::make_unique<SegmentationPipe1>();
    auto extraction1 = std::make_unique<ExtractionPipe1>();
    SegexPipe1 segex1 = SegexPipe1(segmentation1.get(), extraction1.get());

    std::vector<CallbackData *> ally_data = {new CallbackData, new CallbackData, new CallbackData};

    colors = Global::getColors();

    allyDefOut1 = Mat::zeros(original.size(), CV_8UC1);
    allyDirOut1 = Mat::zeros(original.size(), CV_8UC1);

    allyDirOut2 = Mat::zeros(original.size(), CV_8UC1);

    allyDirOut3 = Mat::zeros(original.size(), CV_8UC1);

    ballOut = Mat::zeros(original.size(), CV_8UC1);
    
    enemyOut = Mat::zeros(original.size(), CV_8UC1);
   

    for (int i = 0; i < 3; i++)
    {
        ally_data[i]->original = &hsvImage;
        ally_data[i]->id = i;
    }

    ally_data[0]->outDef = &allyDefOut1;
    ally_data[0]->outDir = &allyDirOut1;

    ally_data[1]->outDef = &allyDefOut2;
    ally_data[1]->outDir = &allyDirOut2;

    ally_data[2]->outDef = &allyDefOut3;
    ally_data[2]->outDir = &allyDirOut3;

    ballCallback.original = &hsvImage;
    ballCallback.outDef = &ballOut;

    enemy_data.original = &hsvImage;
    enemy_data.outDef = &enemyOut;

    /* FIM INICIALIZAÇÕES */

    /* JANELAS */
    namedWindow(original_window, WINDOW_NORMAL);

    namedWindow(window_ally_1, WINDOW_NORMAL);

    namedWindow(out_dir_1, WINDOW_NORMAL);
    namedWindow(out_dir_2, WINDOW_NORMAL);
    namedWindow(out_dir_3, WINDOW_NORMAL);

    namedWindow("Run Track");
    
    namedWindow(ball_window, WINDOW_NORMAL);
    namedWindow(window_enemy, WINDOW_NORMAL);
    
    /* namedWindow("PreP img");
    namedWindow("Temp1", WINDOW_NORMAL);
    namedWindow("Temp2", WINDOW_NORMAL);
    namedWindow("ThresholdSegBall", WINDOW_NORMAL);
    namedWindow("ThresholdSegEnemy", WINDOW_NORMAL);
    namedWindow("ThresholdSegTeam", WINDOW_NORMAL);
    namedWindow("ThresholdSegPlayer1", WINDOW_NORMAL);
    namedWindow("ThresholdSegPlayer2", WINDOW_NORMAL);
    namedWindow("ThresholdSegPlayer3", WINDOW_NORMAL); */

    
   
    /* FIM JANELAS */

    // Chamando as funções
    robot1TrackbarsCreator(window_ally_1, (void *)ally_data[0]);
    robot2TrackbarsCreator(out_dir_2, (void *)ally_data[1]);
    robot3TrackbarsCreator(out_dir_3, (void *)ally_data[2]);

    ballTrackbarsCreator(ball_window, (void *)&ballCallback);

    enemyTrackbarsCreator(window_enemy, (void *)&enemy_data);

    trackObjs();

    while (1)
    {

        cap >> original;

        if (original.empty())
            break;

        if (val)
        {
            double t = (double)getTickCount();
            preprocess.execute(original, filteredImg);
            //imshow("PreP img", filteredImg);
            segex1.execute(filteredImg);
            t = ((double)getTickCount() - t) / getTickFrequency();

            std::cout << "Tempo: [" << t << "]" << std::endl;
        }
        else
        {
            //calibragem
            while (!val)
            {

                preprocess.execute(original, hsvImage);

                imshow(window_ally_1, allyDefOut1);
                imshow(out_dir_1, allyDirOut1);

                imshow(out_dir_2, allyDirOut2);

                imshow(out_dir_3, allyDirOut3);

                imshow(window_enemy, enemyOut);

                imshow(ball_window, ballOut);

                imshow(original_window, original);

                waitKey(10);
            }
        }

        for (int i = 0; i < 3; i++)
        {
            Robo *r = Global::getAlliedRobots()[i];
            Robo *enemy = Global::getEnemyRobots()[i];
            cv::Scalar point_color, enemy_color;

            switch (i)
            {
            case 0:
                point_color = cv::Scalar(0, 0, 255);
                enemy_color = cv::Scalar(125,123,12);
                break;

            case 1:
                point_color = cv::Scalar(0, 255, 0);
                enemy_color = cv::Scalar(4,82,4);
                break;

            case 2:
                point_color = cv::Scalar(255, 0, 0);
                enemy_color = cv::Scalar(3,38,71);
                break;
            }

            helpers::drawObject(r->getPosX(), r->getPosY(), point_color, original);
            helpers::drawObject(enemy->getPosX(), enemy->getPosY(), enemy_color, original);
        }

        auto *b = Global::getBall();

        helpers::drawObject(b->getPosX(), b->getPosY(), cv::Scalar(7,5,82), original);

        imshow(original_window, original);

        // helpers::createImageFile(original, phc++, "original/frame");

        waitKey(15);
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
    static int HEnemyDefMin = 0, HEnemyDefMax = 180;
    static int SEnemyDefMin = 0, SEnemyDefMax = 255;
    static int VEnemyDefMin = 0, VEnemyDefMax = 255;

    createTrackbar("H Def Min", window_name, &HEnemyDefMin, 180, setHEnemyMin, data);
    createTrackbar("H Def Max", window_name, &HEnemyDefMax, 180, setHEnemyMax, data);
    createTrackbar("S Def Min", window_name, &SEnemyDefMin, 255, setSEnemyMin, data);
    createTrackbar("S Def Max", window_name, &SEnemyDefMax, 255, setSEnemyMax, data);
    createTrackbar("V Def Min", window_name, &VEnemyDefMin, 255, setVEnemyMin, data);
    createTrackbar("V Def Max", window_name, &VEnemyDefMax, 255, setVEnemyMax, data);
}

void robot1TrackbarsCreator(std::string window_name, void *data)
{

    createTrackbar("H Def Min", window_name, &HDefMin, 180, setHDefMin, data);
    createTrackbar("H Def Max", window_name, &HDefMax, 180, setHDefMax, data);
    createTrackbar("S Def Min", window_name, &SDefMin, 255, setSDefMin, data);
    createTrackbar("S Def Max", window_name, &SDefMax, 255, setSDefMax, data);
    createTrackbar("V Def Min", window_name, &VDefMin, 255, setVDefMin, data);
    createTrackbar("V Def Max", window_name, &VDefMax, 255, setVDefMax, data);

    createTrackbar("H Dir Min", window_name, &H1DirMin, 180, setHDirectMin, data);
    createTrackbar("H Dir Max", window_name, &H1DirMax, 180, setHDirectMax, data);
    createTrackbar("S Dir Min", window_name, &S1DirMin, 255, setSDirectMin, data);
    createTrackbar("S Dir Max", window_name, &S1DirMax, 255, setSDirectMax, data);
    createTrackbar("V Dir Min", window_name, &V1DirMin, 255, setVDirectMin, data);
    createTrackbar("V Dir Max", window_name, &V1DirMax, 255, setVDirectMax, data);
}

void robot2TrackbarsCreator(std::string window_name, void *data)
{

    createTrackbar("H Dir Min", window_name, &H2DirMin, 180, setHDirectMin, data);
    createTrackbar("H Dir Max", window_name, &H2DirMax, 180, setHDirectMax, data);
    createTrackbar("S Dir Min", window_name, &S2DirMin, 255, setSDirectMin, data);
    createTrackbar("S Dir Max", window_name, &S2DirMax, 255, setSDirectMax, data);
    createTrackbar("V Dir Min", window_name, &V2DirMin, 255, setVDirectMin, data);
    createTrackbar("V Dir Max", window_name, &V2DirMax, 255, setVDirectMax, data);
}

void robot3TrackbarsCreator(std::string window_name, void *data)
{

    createTrackbar("H Dir Min", window_name, &H3DirMin, 180, setHDirectMin, data);
    createTrackbar("H Dir Max", window_name, &H3DirMax, 180, setHDirectMax, data);
    createTrackbar("S Dir Min", window_name, &S3DirMin, 255, setSDirectMin, data);
    createTrackbar("S Dir Max", window_name, &S3DirMax, 255, setSDirectMax, data);
    createTrackbar("V Dir Min", window_name, &V3DirMin, 255, setVDirectMin, data);
    createTrackbar("V Dir Max", window_name, &V3DirMax, 255, setVDirectMax, data);
}

void ballTrackbarsCreator(std::string window_name, void *data)
{
    createTrackbar("H Ball Min", window_name, &ballHMin, 180, setBallHMin, data);
    createTrackbar("H Ball Max", window_name, &ballHMax, 180, setBallHMax, data);
    createTrackbar("S Ball Min", window_name, &ballSMin, 255, setBallSMin, data);
    createTrackbar("S Ball Max", window_name, &ballSMax, 255, setBallSMax, data);
    createTrackbar("V Ball Min", window_name, &ballVMin, 255, setBallVMin, data);
    createTrackbar("V Ball Max", window_name, &ballVMax, 255, setBallVMax, data);
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

void setVDefMin(int pos, void *data)
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

    hsv[2] = pos;

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

void setVDefMax(int pos, void *data)
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

    hsv[2] = pos;

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

void setVDirectMin(int pos, void *data)
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

    hsv[2] = pos;

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

void setVDirectMax(int pos, void *data)
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

    hsv[2] = pos;

    colors->setRobotColorMax(hsv, cb_data->id);

    Scalar minRange;

    minRange = colors->getRobotColorMin(cb_data->id);

    inRange(*cb_data->original, minRange, hsv, *cb_data->outDir);
}

/* Bola */

/* Mínimos */
void setBallHMin(int pos, void *data)
{

    CallbackData *cb_data;
    Scalar hsv;

    if (data == NULL)
    {
        std::cout << "Error: Dado Null" << std::endl;
        exit(1);
    }

    cb_data = (CallbackData *)data;

    auto *b = Global::getBall();

    hsv = b->getHSVMin();

    hsv[0] = pos;

    b->setHSVMin(hsv);

    Scalar maxRange;

    maxRange = b->getHSVMax();

    inRange(*cb_data->original, hsv, maxRange, *cb_data->outDef);
}

void setBallSMin(int pos, void *data)
{

    CallbackData *cb_data;
    Scalar hsv;

    if (data == NULL)
    {
        std::cout << "Error: Dado Null" << std::endl;
        exit(1);
    }

    cb_data = (CallbackData *)data;

    auto *b = Global::getBall();

    hsv = b->getHSVMin();

    hsv[1] = pos;

    b->setHSVMin(hsv);

    Scalar maxRange;

    maxRange = b->getHSVMax();

    inRange(*cb_data->original, hsv, maxRange, *cb_data->outDef);
}

void setBallVMin(int pos, void *data)
{

    CallbackData *cb_data;
    Scalar hsv;

    if (data == NULL)
    {
        std::cout << "Error: Dado Null" << std::endl;
        exit(1);
    }

    cb_data = (CallbackData *)data;

    auto *b = Global::getBall();

    hsv = b->getHSVMin();

    hsv[2] = pos;

    b->setHSVMin(hsv);

    Scalar maxRange;

    maxRange = b->getHSVMax();

    inRange(*cb_data->original, hsv, maxRange, *cb_data->outDef);
}

/* Máximos */
void setBallHMax(int pos, void *data)
{
    CallbackData *cb_data;
    Scalar hsv;

    if (data == NULL)
    {
        std::cout << "Error: Dado Null" << std::endl;
        exit(1);
    }

    cb_data = (CallbackData *)data;

    auto *b = Global::getBall();

    hsv = b->getHSVMax();

    hsv[0] = pos;

    b->setHSVMax(hsv);

    Scalar minRange;

    minRange = b->getHSVMin();

    inRange(*cb_data->original, minRange, hsv, *cb_data->outDef);
}

void setBallSMax(int pos, void *data)
{
    CallbackData *cb_data;
    Scalar hsv;

    if (data == NULL)
    {
        std::cout << "Error: Dado Null" << std::endl;
        exit(1);
    }

    cb_data = (CallbackData *)data;

    auto *b = Global::getBall();

    hsv = b->getHSVMax();

    hsv[1] = pos;

    b->setHSVMax(hsv);

    Scalar minRange;

    minRange = b->getHSVMin();

    inRange(*cb_data->original, minRange, hsv, *cb_data->outDef);
}

void setBallVMax(int pos, void *data)
{
      CallbackData *cb_data;
    Scalar hsv;

    if (data == NULL)
    {
        std::cout << "Error: Dado Null" << std::endl;
        exit(1);
    }

    cb_data = (CallbackData *)data;

    auto *b = Global::getBall();

    hsv = b->getHSVMax();

    hsv[2] = pos;

    b->setHSVMax(hsv);

    Scalar minRange;

    minRange = b->getHSVMin();

    inRange(*cb_data->original, minRange, hsv, *cb_data->outDef);
}

/* Adversários */

/* Mínimos */

void setHEnemyMin(int pos, void *data)
{
    CallbackData *cb_data;
    Scalar hsv;

    if (data == NULL)
    {
        std::cout << "Error: Dado Null" << std::endl;
        exit(1);
    }

    cb_data = (CallbackData *)data;

    hsv = colors->getEnemyMin();

    hsv[0] = pos;

    colors->setEnemyMin(hsv);

    Scalar maxRange;

    maxRange = colors->getEnemyMax();

    inRange(*cb_data->original, hsv, maxRange, *cb_data->outDef);
}

void setSEnemyMin(int pos, void *data)
{
    CallbackData *cb_data;
    Scalar hsv;

    if (data == NULL)
    {
        std::cout << "Error: Dado Null" << std::endl;
        exit(1);
    }

    cb_data = (CallbackData *)data;

    hsv = colors->getEnemyMin();

    hsv[1] = pos;

    colors->setEnemyMin(hsv);

    Scalar maxRange;

    maxRange = colors->getEnemyMax();

    inRange(*cb_data->original, hsv, maxRange, *cb_data->outDef);
}

void setVEnemyMin(int pos, void *data)
{
    CallbackData *cb_data;
    Scalar hsv;

    if (data == NULL)
    {
        std::cout << "Error: Dado Null" << std::endl;
        exit(1);
    }

    cb_data = (CallbackData *)data;

    hsv = colors->getEnemyMin();

    hsv[2] = pos;

    colors->setEnemyMin(hsv);

    Scalar maxRange;

    maxRange = colors->getEnemyMax();

    inRange(*cb_data->original, hsv, maxRange, *cb_data->outDef);
}

/* Máximos cor do time */

void setHEnemyMax(int pos, void *data)
{
    CallbackData *cb_data;
    Scalar hsv;

    if (data == NULL)
    {
        std::cout << "Error: Dado Null" << std::endl;
        exit(1);
    }

    cb_data = (CallbackData *)data;

    hsv = colors->getEnemyMax();

    hsv[0] = pos;

    colors->setEnemyMax(hsv);

    Scalar minRange;

    minRange = colors->getEnemyMin();

    inRange(*cb_data->original, minRange, hsv, *cb_data->outDef);
}

void setSEnemyMax(int pos, void *data)
{
    CallbackData *cb_data;
    Scalar hsv;

    if (data == NULL)
    {
        std::cout << "Error: Dado Null" << std::endl;
        exit(1);
    }

    cb_data = (CallbackData *)data;

    hsv = colors->getEnemyMax();

    hsv[1] = pos;

    colors->setEnemyMax(hsv);

    Scalar minRange;

    minRange = colors->getEnemyMin();

    inRange(*cb_data->original, minRange, hsv, *cb_data->outDef);
}

void setVEnemyMax(int pos, void *data)
{
    CallbackData *cb_data;
    Scalar hsv;

    if (data == NULL)
    {
        std::cout << "Error: Dado Null" << std::endl;
        exit(1);
    }

    cb_data = (CallbackData *)data;

    hsv = colors->getEnemyMax();

    hsv[2] = pos;

    colors->setEnemyMax(hsv);

    Scalar minRange;

    minRange = colors->getEnemyMin();

    inRange(*cb_data->original, minRange, hsv, *cb_data->outDef);
}
