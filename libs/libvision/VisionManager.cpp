/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/*
 * File:   VisionManager.cpp
 * Author: gilberto
 *
 * Created on dc21 de Agosto de 2018, 17:28
 */

#include "VisionManager.h"
#include <cmath>

#include "../globals/Global.h"
#include "../helpers/helper.h"

using namespace cv;
using namespace std;

//TODO: Função de track dos robôs adversários

VisionManager::VisionManager(std::string img_path)
{
    this->originalImage = imread(img_path);

    double t = (double)getTickCount();
    PreProcess::applyBlur(this->originalImage, this->blurImage, 3);
    t = ((double)getTickCount() - t) / getTickFrequency();

    helpers::createImageFile(this->blurImage, t, "../Images/outputs/blur_img_");

    t = (double)getTickCount();
    cvtColor(this->originalImage, this->hsvImage, COLOR_BGR2HSV);
    t = ((double)getTickCount() - t) / getTickFrequency();

    helpers::createImageFile(this->hsvImage, t, "toHSV_");
}

void VisionManager::trackObjects()
{

    //trackBall();
    trackAlliedRobots();
    //trackEnemyRobots();
}

void VisionManager::trackEnemyRobots()
{

    Mat thresholdRobot;
    Mat tempRobot;
    vector<vector<Point>> robotContours;

    /* for (auto i = 0ul; i < Global::getEnemyRobots().size(); ++i)
    {
        auto r = Global::getEnemyRobots()[i];

        inRange(this->hsvImage, r->getHSVMinDefColor(), r->getHSVMaxDefColor(), thresholdRobot);

        PreProcess::morphOps(thresholdRobot, 3, MORPH_OPEN, 2, PreProcess::morphType::CROSS);

        thresholdRobot.copyTo(tempRobot);
        findContours(tempRobot, robotContours, RETR_CCOMP, CHAIN_APPROX_SIMPLE);

        imshow("ENEMY TEAM", thresholdRobot);

        if (robotContours.size() >= 1)
        {
            for (auto i = 0ul; i < robotContours.size(); ++i)
            {

                Moments enemyMoment;
                double enemyArea;

                enemyMoment = moments(robotContours[i]);
                enemyArea = enemyMoment.m00;

                r->setPosX(static_cast<int>(enemyMoment.m10 / enemyArea));
                r->setPosY(static_cast<int>(enemyMoment.m01 / enemyArea));
            }
        }
    } */
}

void VisionManager::trackAlliedRobots()
{
    vector<vector<Point>> teamContours;
    vector<int> alreadyUsed;

    //Track cor do time
    {
        Mat teamThreshold;
    
        Scalar teamMin, teamMax;
        teamMin = Global::getColors()->getAllyMin();
        teamMax = Global::getColors()->getAllyMax();
        inRange(this->hsvImage, teamMin, teamMax, teamThreshold);

        PreProcess::morphOps(teamThreshold, 3, MORPH_OPEN, 1, PreProcess::morphType::CROSS);

        PreProcess::singleMorph(teamThreshold, 3, PreProcess::singleOP::ERODE);

        findContours(teamThreshold, teamContours, RETR_LIST, CHAIN_APPROX_SIMPLE);

        alreadyUsed = vector<int>(0, teamContours.size());
    }

    //Track cor do jogador
    for (int i = 0; i < 1; ++i)
    {
        Scalar playerColorMin, playerColorMax;
        Mat thresholdPlayer;
        vector<vector<Point>> playerContours;

        Robo *r = Global::getAlliedRobots()[i];

        playerColorMin = Global::getColors()->getRobotColorMin(i);
        playerColorMax = Global::getColors()->getRobotColorMax(i);

        double t = (double)getTickCount();
        inRange(this->hsvImage, playerColorMin, playerColorMax, thresholdPlayer);
        t = ((double)getTickCount() - t) / getTickFrequency();

        std::string name;

        switch (i)
        {
        case 0:

            name = "r1_player_";
            break;
        case 1:
            name = "r2_player_";
            break;
        case 2:
            name = "r3_player_";
            break;
        }

        helpers::createImageFile(thresholdPlayer, t, name);

        t = (double)getTickCount();
        PreProcess::morphOps(thresholdPlayer, 3, MORPH_OPEN, 1, PreProcess::morphType::CROSS);
        t = ((double)getTickCount() - t) / getTickFrequency();

        switch (i)
        {
        case 0:
            name = "r1_team_morph_open_";
            break;
        case 1:
            name = "r2_team_morph_open_";
            break;
        case 2:
            name = "r3_team_morph_open_";
            break;
        }  
        helpers::createImageFile(thresholdPlayer, t, name);

        t = (double)getTickCount();
        PreProcess::singleMorph(thresholdPlayer, 3, PreProcess::singleOP::ERODE);
        t = ((double)getTickCount() - t) / getTickFrequency();

        switch (i)
        {
        case 0:
            name = "r1_team_erode_";
            break;
        case 1:
            name = "r2_team_erode_";
            break;
        case 2:
            name = "r3_team_erode_";
            break;
        }  
        helpers::createImageFile(thresholdPlayer, t, name);


        t = (double)getTickCount();
        findContours(thresholdPlayer, playerContours, RETR_LIST, CHAIN_APPROX_SIMPLE);
        t = ((double)getTickCount() - t)/getTickFrequency();

        std::cout << "findContours(thresholdPlayer): time = "  << t << std::endl;


        t = (double)getTickCount();
        if (playerContours.size() >= 1)
        {

            for (auto j = 0ul; j < playerContours.size(); j++)
            {

                Moments playerMoment;
                double playerArea;

                playerMoment = moments(playerContours[j]);

                playerArea = playerMoment.m00;

                if (playerArea >= MIN_DIRECT_AREA && playerArea <= MAX_DIRECT_AREA)
                {

                    int playerX = static_cast<int>(playerMoment.m10 / playerArea);
                    int playerY = static_cast<int>(playerMoment.m01 / playerArea);

                    for (auto k = 0ul; k < teamContours.size(); ++k) {

                        if (!alreadyUsed[k]) {
                            Moments teamMoment = moments(teamContours);
                            double teamArea = teamMoment.m00;

                            int teamX = static_cast<int>(teamMoment.m10/teamArea);
                            int teamY = static_cast<int>(teamMoment.m01/teamArea);

                            double xDif, yDif;

                            xDif = playerX - teamX;
                            yDif = playerY - teamY;

                            double dist;

                            dist = sqrt(pow(xDif,2.0)+pow(yDif,2.0));

                            // if (dist >= 1.0 && dist <= 2.0) {
                                
                            // }

                            std::cout << "Distance [" << i << "] = " << dist << std::endl;
                        }
                    }

                    //drawObject(directContours[j], Scalar(255, 0, 0));
                }
            }
        }
        t = ((double)getTickCount() - t)/getTickFrequency();

        std::cout << "getXYPos(role): time = " << t << std::endl;

        /* else {
            putText(this->cameraFeed, "MUITO RUIDO", Point(0,50), 2, 1, Scalar(0, 255,0), 2);
        }

        waitKey(10); */

        //break;
    }
}

void VisionManager::trackBall()
{
    Mat thresholdBall, tempBall;

    inRange(this->hsvImage, Global::getBall()->getHSVMin(), Global::getBall()->getHSVMax(), thresholdBall);

    PreProcess::morphOps(thresholdBall, 3, MORPH_CLOSE, 3, PreProcess::morphType::BALL);
    PreProcess::singleMorph(thresholdBall, 5, PreProcess::singleOP::DILATE);

    vector<vector<Point>> ballContours;

    thresholdBall.copyTo(tempBall);
    findContours(tempBall, ballContours, RETR_CCOMP, CHAIN_APPROX_SIMPLE);

    //HoughCircles(threshold, circles, HOUGH_GRADIENT, 1, threshold.rows / 8, 20, 18, 0, 0);

    if (ballContours.size() >= 1)
    {

        for (size_t i = 0; i < ballContours.size(); ++i)
        {
            //Point center(cvRound(circles[i][0]), cvRound(circles[i][1]));
            //int radius = cvRound(circles[i][2]);

            Moments moment = moments(ballContours[i]);
            double area = moment.m00;

            if (area >= MIN_BALL_AREA && area <= MAX_BALL_AREA)
            {

                Global::getBall()->setPosX(static_cast<int>(moment.m10 / area));
                Global::getBall()->setPosY(static_cast<int>(moment.m01 / area));

                drawObject(ballContours[i], Scalar(201, 20, 189));
            }
        }
    }
}

void VisionManager::drawObject(vector<Point> contour, Scalar color)
{

    vector<Point> contours_poly(contour.size());
    Rect boundRect;
    Point2f center;
    float radius;

    approxPolyDP(contour, contours_poly, 3, true);
    boundRect = boundingRect(contours_poly);
    minEnclosingCircle(contours_poly, center, radius);

    rectangle(this->originalImage, boundRect.tl(), boundRect.br(), color, 2);

    imwrite("../Images/outputs/final_image.png", this->originalImage);
}