#include "libs/libvision/ImgProcess.h"
#include "libs/libobjects/Robo.h"
#include "libs/libobjects/Ball.h"
#include "libs/globals/Global.h"
#include "libs/libvision/Colors.h"
#include "libs/helpers/helper.h"

#include "libs/interfaces/PreProcessInterface.hpp"
#include "libs/interfaces/SegmentationInterface.hpp"
#include "libs/interfaces/ExtractionInterface.hpp"
#include "libs/interfaces/SegExInterface.hpp"

#include "classes/Pipe/PreProcess/PreProcess.hpp"

#include "classes/Pipe/SegEx/Segex.hpp"

#include "classes/Pipe/SegEx/HSSegFull/FullSeg.hpp"
#include "classes/Pipe/SegEx/HSSegReduced/ReducedSeg.hpp"

#include "classes/Pipe/SegEx/Extraction/Extraction.hpp"

#include <string>
#include <iostream>
#include <memory>
#include <fstream>

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

void robotTrackbarsCreator(std::string window_name, void *data);

void teamTrackbarsCreator(std::string window_name, void *data);
void enemyTrackbarsCreator(std::string window_name, void *data);

void ballTrackbarsCreator(std::string window_name, void *data);

void trackObjs();

struct CallbackData
{
    int id;
    Mat *original;
    Mat *out;
};

struct RobotCallbackData
{
    int id;
    Mat *original;
    Mat *out;
    int *Hmin, *Hmax;
    int *Smin, *Smax;
};

const std::string original_window = "Original";

const std::string out_dir_1 = "Ally Dir 1";

const std::string out_dir_2 = "Ally Dir 2";

const std::string out_dir_3 = "Ally Dir 3";

const std::string ball_window = "Ball Window";

const std::string window_enemy = "Enemy";

const std::string window_team = "Team";

Colors *colors;

int val = 0;

int main()
{
    // Players
    int H1Min = 0, H1Max = 180;
    int S1Min = 0, S1Max = 255;

    int H2Min = 0, H2Max = 180;
    int S2Min = 0, S2Max = 255;

    int H3Min = 0, H3Max = 180;
    int S3Min = 0, S3Max = 255;

    int ballHMin = 0, ballHMax = 180;
    int ballSMin = 0, ballSMax = 255;

    VideoCapture cap;

    Mat original, hsvImage, filteredImg;
    Mat allyDirOut1;
    Mat allyDirOut2;
    Mat allyDirOut3;
    Mat ballOut;
    Mat enemyOut, teamOut;

    CallbackData enemy_data, team_data;

    RobotCallbackData ballCallback;
    std::vector<RobotCallbackData *> ally_data = {new RobotCallbackData, new RobotCallbackData, new RobotCallbackData};

    // std::ofstream file;

    std::string videoPath = "../videos/escurecido.webm";

    cap.open(videoPath);

    if (!cap.isOpened())
    {
        std::cout << "Erro ao abrir vídeo" << std::endl;
        exit(-1);
    }

    cap >> original;

    // std::vector<int> frames = {
    //     522, 527, 883, 1199, 1249, 1303, 1375, 1389, 1401, 1673,
    //     1719, 1934, 1967, 1977, 2511, 2538, 2629, 2676, 2681, 2707,
    //     2862, 2908, 2955, 3040, 3074, 3092, 3101, 3150, 3152, 3209,
    //     3262, 3333, 3555, 3766, 3835, 3939, 3972, 3978, 4072, 4073,
    //     4132, 4256, 4371, 4571, 4586, 4605, 4629, 4637, 4655, 4741,
    //     4749, 4764, 4827, 4889, 5000, 5109, 5146, 5236, 5250, 5388,
    //     5416, 5586, 5652, 5940, 5971, 6135, 6185, 6217, 6276, 6496,
    //     6680, 6693, 6717, 6763, 7042, 7052, 7109, 7122, 7157, 7167,
    //     7185, 7229, 7306, 7496, 7536, 7711, 7966, 8031, 8053, 8099,
    //     8139, 8225, 8287, 8492, 8569, 8613, 8695, 8731, 8750, 8774};

    /* INICIALIZAÇÕES */

    // file.open("../output.csv");

    //Escrevendo o cabeçalho
    // file << "frame,1(verde),2(roxo),3(rosa),4(bola),5(everde),6(erosa),7(eroxo)\n";

    PreProcess preprocess = PreProcess();

    auto segmentation = std::make_unique<FullSeg>();
    auto extraction = std::make_unique<Extraction>();

    Segex segex = Segex(segmentation.get(), extraction.get());

    colors = Global::getColors();

    allyDirOut1 = Mat::zeros(original.size(), CV_8UC1);

    allyDirOut2 = Mat::zeros(original.size(), CV_8UC1);

    allyDirOut3 = Mat::zeros(original.size(), CV_8UC1);

    ballOut = Mat::zeros(original.size(), CV_8UC1);

    enemyOut = Mat::zeros(original.size(), CV_8UC1);

    teamOut = Mat::zeros(original.size(), CV_8UC1);

    for (int i = 0; i < 3; i++)
    {
        ally_data[i]->original = &hsvImage;
        ally_data[i]->id = i;
    }

    ally_data[0]->out = &allyDirOut1;
    ally_data[0]->Hmin = &H1Min;
    ally_data[0]->Hmax = &H1Max;
    ally_data[0]->Smin = &S1Min;
    ally_data[0]->Smax = &S1Max;

    ally_data[1]->out = &allyDirOut2;
    ally_data[1]->Hmin = &H2Min;
    ally_data[1]->Hmax = &H2Max;
    ally_data[1]->Smin = &S2Min;
    ally_data[1]->Smax = &S2Max;

    ally_data[2]->out = &allyDirOut3;
    ally_data[2]->Hmin = &H2Min;
    ally_data[2]->Hmax = &H2Max;
    ally_data[2]->Smin = &S2Min;
    ally_data[2]->Smax = &S2Max;

    ballCallback.original = &hsvImage;
    ballCallback.out = &ballOut;
    ballCallback.Hmin = &ballHMin;
    ballCallback.Hmax = &ballHMax;
    ballCallback.Smin = &ballSMin;
    ballCallback.Smax = &ballSMax;

    enemy_data.original = &hsvImage;
    enemy_data.out = &enemyOut;

    team_data.original = &hsvImage;
    team_data.out = &teamOut;

    /* FIM INICIALIZAÇÕES */

    /* JANELAS */
    namedWindow(original_window, WINDOW_NORMAL);

    namedWindow(window_team, WINDOW_NORMAL);

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
    teamTrackbarsCreator(window_team, (void *) &team_data);
    robotTrackbarsCreator(out_dir_1, (void *)ally_data[0]);
    robotTrackbarsCreator(out_dir_2, (void *)ally_data[1]);
    robotTrackbarsCreator(out_dir_3, (void *)ally_data[2]);

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
            preprocess.execute(original, filteredImg);

            segex.execute(filteredImg);
        }
        else
        {
            //calibragem
            while (!val)
            {

                preprocess.execute(original, hsvImage);

                imshow(window_team, teamOut);

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
                enemy_color = cv::Scalar(125, 123, 12);
                break;

            case 1:
                point_color = cv::Scalar(0, 255, 0);
                enemy_color = cv::Scalar(4, 82, 4);
                break;

            case 2:
                point_color = cv::Scalar(255, 0, 0);
                enemy_color = cv::Scalar(3, 38, 71);
                break;
            }

            helpers::drawObject(r->getPosX(), r->getPosY(), point_color, original);
            helpers::drawObject(enemy->getPosX(), enemy->getPosY(), enemy_color, original);
        }

        auto *b = Global::getBall();

        helpers::drawObject(b->getPosX(), b->getPosY(), cv::Scalar(7, 5, 82), original);

        imshow(original_window, original);

        waitKey(15);
    }

    //file.close();

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

    createTrackbar("H Def Min", window_name, &HEnemyDefMin, 180, setHEnemyMin, data);
    createTrackbar("H Def Max", window_name, &HEnemyDefMax, 180, setHEnemyMax, data);
    createTrackbar("S Def Min", window_name, &SEnemyDefMin, 255, setSEnemyMin, data);
    createTrackbar("S Def Max", window_name, &SEnemyDefMax, 255, setSEnemyMax, data);
}

void teamTrackbarsCreator(std::string window_name, void *data)
{
    // Team
    static int HDefMin = 0, HDefMax = 180;
    static int SDefMin = 0, SDefMax = 255;

    createTrackbar("H Def Min", window_name, &HDefMin, 180, setHDefMin, data);
    createTrackbar("H Def Max", window_name, &HDefMax, 180, setHDefMax, data);
    createTrackbar("S Def Min", window_name, &SDefMin, 255, setSDefMin, data);
    createTrackbar("S Def Max", window_name, &SDefMax, 255, setSDefMax, data);
}

void robotTrackbarsCreator(std::string window_name, void *data)
{
    RobotCallbackData *rdata = (RobotCallbackData *) data;

    createTrackbar("H Dir Min", window_name, rdata->Hmin, 180, setHDirectMin, data);
    createTrackbar("H Dir Max", window_name, rdata->Hmax, 180, setHDirectMax, data);
    createTrackbar("S Dir Min", window_name, rdata->Smin, 255, setSDirectMin, data);
    createTrackbar("S Dir Max", window_name, rdata->Smax, 255, setSDirectMax, data);
}

void ballTrackbarsCreator(std::string window_name, void *data)
{
    RobotCallbackData *ball = (RobotCallbackData *) data;

    createTrackbar("H Ball Min", window_name, ball->Hmin, 180, setBallHMin, data);
    createTrackbar("H Ball Max", window_name, ball->Hmax, 180, setBallHMax, data);
    createTrackbar("S Ball Min", window_name, ball->Smin, 255, setBallSMin, data);
    createTrackbar("S Ball Max", window_name, ball->Smax, 255, setBallSMax, data);
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

    inRange(*cb_data->original, hsv, maxRange, *cb_data->out);
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

    inRange(*cb_data->original, hsv, maxRange, *cb_data->out);
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

    inRange(*cb_data->original, hsv, maxRange, *cb_data->out);
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

    inRange(*cb_data->original, minRange, hsv, *cb_data->out);
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

    inRange(*cb_data->original, minRange, hsv, *cb_data->out);
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

    inRange(*cb_data->original, minRange, hsv, *cb_data->out);
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

    inRange(*cb_data->original, hsv, maxRange, *cb_data->out);
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

    inRange(*cb_data->original, hsv, maxRange, *cb_data->out);
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

    inRange(*cb_data->original, hsv, maxRange, *cb_data->out);
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

    inRange(*cb_data->original, minRange, hsv, *cb_data->out);
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

    inRange(*cb_data->original, minRange, hsv, *cb_data->out);
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

    inRange(*cb_data->original, minRange, hsv, *cb_data->out);
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

    inRange(*cb_data->original, hsv, maxRange, *cb_data->out);
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

    inRange(*cb_data->original, hsv, maxRange, *cb_data->out);
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

    inRange(*cb_data->original, hsv, maxRange, *cb_data->out);
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

    inRange(*cb_data->original, minRange, hsv, *cb_data->out);
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

    inRange(*cb_data->original, minRange, hsv, *cb_data->out);
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

    inRange(*cb_data->original, minRange, hsv, *cb_data->out);
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

    inRange(*cb_data->original, hsv, maxRange, *cb_data->out);
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

    inRange(*cb_data->original, hsv, maxRange, *cb_data->out);
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

    inRange(*cb_data->original, hsv, maxRange, *cb_data->out);
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

    inRange(*cb_data->original, minRange, hsv, *cb_data->out);
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

    inRange(*cb_data->original, minRange, hsv, *cb_data->out);
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

    inRange(*cb_data->original, minRange, hsv, *cb_data->out);
}
