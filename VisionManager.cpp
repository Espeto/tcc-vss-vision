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
#include <iostream>
#include <sstream>

#include "Global.h"

using namespace cv;
using namespace std;

//TODO: Função de track dos robôs adversários

VisionManager::VisionManager(std::string img_path)
{
    this->originalImage = imread(img_path);

    double t = (double)getTickCount();
    PreProcess::applyBlur(this->originalImage, this->blurImage, 3);
    t = ((double)getTickCount() - t) / getTickFrequency();

    std::ostringstream fname;

    fname << "../Images/outputs/blur_img_" << t << "_.png";
    imwrite(fname.str(), this->blurImage);
    fname.str("");

    t = (double)getTickCount();
    cvtColor(this->originalImage, this->hsvImage, COLOR_BGR2HSV);
    t = ((double)getTickCount() - t) / getTickFrequency();
    fname.str("");

    fname << "../Images/outputs/toHSV_" << t << "_.png";
    imwrite(fname.str(), this->hsvImage);
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

    for (auto i = 0ul; i < Global::getEnemyRobots().size(); ++i)
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
    }
}

void VisionManager::trackAlliedRobots()
{
    Mat thresholdTotal, thresholdDef, thresholdDirect;
    Mat tempTotal, tempDirect;
    vector<vector<Point>> totalContours, directContours;

    //Global::getAlliedRobots().size()
    for (unsigned int i = 0; i < 1; ++i)
    {

        Robo *r = Global::getAlliedRobots()[i];

        double t = (double)getTickCount();

        inRange(this->hsvImage, r->getHSVMinDefColor(), r->getHSVMaxDefColor(), thresholdDef);

        t = ((double)getTickCount() - t) / getTickFrequency();

        std::ostringstream name;

        switch (i)
        {
        case 0:

            name << "../Images/outputs/"
                 << "r1_team_" << t << ".png";
            imwrite(name.str(), thresholdDef);
            name.str("");

            break;
        case 1:
            name << "../Images/outputs/"
                 << "r2_team_" << t << ".png";
            imwrite(name.str(), thresholdDef);
            name.str("");

            break;
        case 2:
            name << "../Images/outputs/"
                 << "r3_team_" << t << ".png";
            imwrite(name.str(), thresholdDef);
            name.str("");

            break;
        }

        t = (double)getTickCount();

        inRange(this->hsvImage, r->getHSVMinDirectColor(), r->getHSVMaxDirectColor(), thresholdDirect);

        t = ((double)getTickCount() - t) / getTickFrequency();

        switch (i)
        {
        case 0:

            name << "../Images/outputs/"
                 << "r1_role_" << t << ".png";
            imwrite(name.str(), thresholdDirect);
            name.str("");

            break;
        case 1:

            name << "../Images/outputs/"
                 << "r2_role_" << t << ".png";
            imwrite(name.str(), thresholdDirect);
            name.str("");

            break;
        case 2:

            name << "../Images/outputs/"
                 << "r3_role_" << t << ".png";
            imwrite(name.str(), thresholdDirect);
            name.str("");

            break;
        }

        t = (double)getTickCount();
        PreProcess::morphOps(thresholdDef, 3, MORPH_OPEN, 1, PreProcess::morphType::CROSS);
        t = ((double)getTickCount() - t) / getTickFrequency();

        switch (i)
        {
        case 0:
            name << "../Images/outputs/"
                 << "r1_team_morph_open_" << t << ".png";
            imwrite(name.str(), thresholdDef);
            name.str("");
            break;
        case 1:
            name << "../Images/outputs/"
                 << "r2_team_morph_open_" << t << ".png";
            imwrite(name.str(), thresholdDef);
            name.str("");
            break;
        case 2:
            name << "../Images/outputs/"
                 << "r3_team_morph_open_" << t << ".png";
            imwrite(name.str(), thresholdDef);
            name.str("");
            break;
        }   

        t = (double)getTickCount();
        PreProcess::morphOps(thresholdDirect, 3, MORPH_OPEN, 1, PreProcess::morphType::ROBOT);
        t = ((double)getTickCount() - t)/getTickFrequency();
        
        switch (i)
        {
        case 0:
            name << "../Images/outputs/"
                 << "r1_role_morph_open_" << t << ".png";
            imwrite(name.str(), thresholdDirect);
            name.str("");
            break;
        case 1:
            name << "../Images/outputs/"
                 << "r2_role_morph_open_" << t << ".png";
            imwrite(name.str(), thresholdDirect);
            name.str("");
            break;
        case 2:
            name << "../Images/outputs/"
                 << "r3_role_morph_open_" << t << ".png";
            imwrite(name.str(), thresholdDirect);
            name.str("");
            break;
        } 

        /*  */
        t = (double)getTickCount();
        add(thresholdDef, thresholdDirect, thresholdTotal);
        t = ((double)getTickCount() - t) / getTickFrequency();

        switch (i)
        {
        case 0:

            name << "../Images/outputs/"
                 << "r1_total_" << t << ".png";
            imwrite(name.str(), thresholdTotal);
            name.str("");

            break;
        case 1:

            name << "../Images/outputs/"
                 << "r2_total_" << t << ".png";
            imwrite(name.str(), thresholdTotal);
            name.str("");
            break;
        case 2:

            name << "../Images/outputs/"
                 << "r3_total_" << t << ".png";
            imwrite(name.str(), thresholdTotal);
            name.str("");
            break;
        }
        /*  */

        t = (double)getTickCount();
        PreProcess::morphOps(thresholdTotal, 3, MORPH_CLOSE, 2, PreProcess::morphType::ROBOT);
        t = ((double)getTickCount() - t)/getTickFrequency();

        switch (i)
        {
        case 0:

            name << "../Images/outputs/"
                 << "r1_total_morph_close_" << t << ".png";
            imwrite(name.str(), thresholdTotal);
            name.str("");

            break;
        case 1:

            name << "../Images/outputs/"
                 << "r2_total_morph_close_" << t << ".png";
            imwrite(name.str(), thresholdTotal);
            name.str("");
            break;
        case 2:

            name << "../Images/outputs/"
                 << "r3_total_morph_close_" << t << ".png";
            imwrite(name.str(), thresholdTotal);
            name.str("");
            break;
        }

        t = (double)getTickCount();
        PreProcess::morphOps(thresholdTotal, 3, MORPH_OPEN, 2, PreProcess::morphType::ROBOT);
        t = ((double)getTickCount() - t)/getTickFrequency();

        switch (i)
        {
        case 0:

            name << "../Images/outputs/"
                 << "r1_total_morph_open_" << t << ".png";
            imwrite(name.str(), thresholdTotal);
            name.str("");

            break;
        case 1:

            name << "../Images/outputs/"
                 << "r2_total_morph_open_" << t << ".png";
            imwrite(name.str(), thresholdTotal);
            name.str("");
            break;
        case 2:

            name << "../Images/outputs/"
                 << "r3_total_morph_open_" << t << ".png";
            imwrite(name.str(), thresholdTotal);
            name.str("");
            break;
        }

        t = (double)getTickCount();
        PreProcess::singleMorph(thresholdDirect, 3, PreProcess::singleOP::DILATE);
        t = ((double)getTickCount() - t)/getTickFrequency();

        switch (i)
        {
        case 0:

            name << "../Images/outputs/"
                 << "r1_role_dilate_" << t << ".png";
            imwrite(name.str(), thresholdDirect);
            name.str("");

            break;
        case 1:

            name << "../Images/outputs/"
                 << "r2_role_dilate_" << t << ".png";
            imwrite(name.str(), thresholdDirect);
            name.str("");
            break;
        case 2:

            name << "../Images/outputs/"
                 << "r3_role_dilate_" << t << ".png";
            imwrite(name.str(), thresholdDirect);
            name.str("");
            break;
        }

        t = (double)getTickCount();
        PreProcess::singleMorph(thresholdTotal, 3, PreProcess::singleOP::DILATE);
        t = ((double)getTickCount() - t)/getTickFrequency();

        switch (i)
        {
        case 0:

            name << "../Images/outputs/"
                 << "r1_total_dilate_" << t << ".png";
            imwrite(name.str(), thresholdTotal);
            name.str("");

            break;
        case 1:

            name << "../Images/outputs/"
                 << "r2_total_dilate_" << t << ".png";
            imwrite(name.str(), thresholdTotal);
            name.str("");
            break;
        case 2:

            name << "../Images/outputs/"
                 << "r3_total_dilate_" << t << ".png";
            imwrite(name.str(), thresholdTotal);
            name.str("");
            break;
        }

        t = (double)getTickCount();
        thresholdDirect.copyTo(tempDirect);
        findContours(tempDirect, directContours, RETR_CCOMP, CHAIN_APPROX_SIMPLE);
        t = ((double)getTickCount() - t)/getTickFrequency();

        std::cout << "findContours(tempDirect): time = "  << t << std::endl;


        t = (double)getTickCount();
        thresholdTotal.copyTo(tempTotal);
        findContours(tempTotal, totalContours, RETR_CCOMP, CHAIN_APPROX_SIMPLE);
        t = ((double)getTickCount() - t)/getTickFrequency();

        std::cout << "findContours(tempTotal): time = " << t << std::endl;


        /*  switch(i) {
        case 0:
            imshow("DEFINE TIME", thresholdDef);
            imshow("DEFINE GOLEIRO", thresholdDirect);
            imshow("GOLEIRO TOTAL", thresholdTotal);
            break;
        case 1:
            imshow("DEFINE JOGADOR1", thresholdDirect);
            imshow("JOGADOR1 TOTAL", thresholdTotal);
            break;
        case 2:
            imshow("DEFINE JOGADOR2", thresholdDirect);
            imshow("JOGADOR2 TOTAL", thresholdTotal);
            break;

        }
 */

        t = (double)getTickCount();
        if (directContours.size() >= 1)
        {

            for (auto j = 0ul; j < directContours.size(); j++)
            {

                Moments directMoment;
                double directArea;

                directMoment = moments(directContours[j]);

                directArea = directMoment.m00;

                if (directArea >= MIN_DIRECT_AREA && directArea <= MAX_DIRECT_AREA)
                {

                    r->setDirectionPosX(static_cast<int>(directMoment.m10 / directArea));
                    r->setDirectionPosY(static_cast<int>(directMoment.m01 / directArea));

                    //drawObject(directContours[j], Scalar(255, 0, 0));
                }
            }
        }
        t = ((double)getTickCount() - t)/getTickFrequency();

        std::cout << "getXYPos(role): time = " << t << std::endl;

        t = (double)getTickCount();
        if (totalContours.size() >= 1)
        {
            vector<Point> approxQuad;

            for (auto j = 0ul; j < totalContours.size(); j++)
            {

                approxPolyDP(totalContours[j], approxQuad, arcLength(Mat(totalContours[j]), true) * 0.05, true);

                if (approxQuad.size() == 4)
                {
                    Moments totalMoment;
                    double totalArea;

                    totalMoment = moments(totalContours[j]);

                    totalArea = totalMoment.m00;

                    if (totalArea >= MIN_OBJECT_AREA && totalArea <= MAX_OBEJCT_AREA)
                    {

                        r->setPosX(static_cast<int>(totalMoment.m10 / totalArea));
                        r->setPosY(static_cast<int>(totalMoment.m01 / totalArea));

                        //drawObject(approxQuad, Scalar(0, 255, 0));
                    }
                }
            }
        }
        t = ((double)getTickCount() - t)/getTickFrequency();

        std::cout << "getXYPos(Total): time = " << t << std::endl;

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