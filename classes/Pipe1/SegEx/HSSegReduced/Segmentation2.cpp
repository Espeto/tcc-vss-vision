#include "Segmentation2.hpp"
#include "../../../../libs/globals/Global.h"
#include "../../../../libs/libvision/PreProcess.h"
#include "../../../../libs/helpers/helper.h"
#include <opencv2/highgui.hpp>
#include <iostream>
#include <omp.h>

Segmentation2::Segmentation2()
{
    this->fc = 1;

    this->factor = WINDOW_SIZE / 2;

    this->ballFound = this->teamFound = this->rolesFound = this->enemiesFound = false;
}

bool Segmentation2::isAllFound()
{
    return this->ballFound && this->teamFound && this->rolesFound && this->enemiesFound;
}

objectsContours Segmentation2::execute(cv::Mat preProcessedImg)
{
    std::vector<std::vector<std::vector<cv::Point>>> allPlayersContours(3, std::vector<std::vector<cv::Point>>());
    std::vector<std::vector<cv::Point>> teamContours, enemyContours;
    std::vector<cv::Point> ballContour;

    // #pragma omp task
    this->segmentBall(preProcessedImg, &ballContour);

    // #pragma omp task
    this->segmentTeam(preProcessedImg, &teamContours);

    // #pragma omp task
    this->segmentPlayers(preProcessedImg, &allPlayersContours);

    // #pragma omp task
    this->segmentEnemy(preProcessedImg, &enemyContours);

    this->fc++;

    // #pragma omp taskwait

    return {allPlayersContours, teamContours, ballContour, enemyContours};
}

void Segmentation2::segmentTeam(cv::Mat preProcessedImg, std::vector<std::vector<cv::Point>> *teamContours)
{
    std::vector<std::vector<cv::Point>> tempContours;
    cv::Mat enemyThreshold;
    cv::Scalar teamMin, teamMax;
    bool found[3] = {false, false, false};
    cv::Point offset;
    cv::Size wholeSize;

    teamMin = Global::getColors()->getAllyMin();
    teamMax = Global::getColors()->getAllyMax();

    for (int i = 0; i < 3; ++i)
    {
        enemyThreshold = cv::Mat::zeros(preProcessedImg.size(), CV_8UC1);

        int previous_x = Global::getAlliedRobots()[i]->getPosX();
        int previous_y = Global::getAlliedRobots()[i]->getPosY();

        previous_x - this->factor > 0 ? previous_x -= this->factor : previous_x = 0;

        previous_y - this->factor > 0 ? previous_y -= this->factor : previous_y = 0;

        if (previous_x > preProcessedImg.cols - this->WINDOW_SIZE)
            previous_x = preProcessedImg.cols - this->WINDOW_SIZE;

        if (previous_y > preProcessedImg.rows - this->WINDOW_SIZE)
            previous_y = preProcessedImg.rows - this->WINDOW_SIZE;

        cv::Mat roi(preProcessedImg, cv::Rect(previous_x, previous_y, this->WINDOW_SIZE, this->WINDOW_SIZE));

        roi.locateROI(wholeSize, offset);

        cv::inRange(roi, teamMin, teamMax, enemyThreshold);

        // if (this->fc > 504 && this->fc < 1500)
        // {
        //     helpers::createImageFile(enemyThreshold, this->fc, "team_frames/bposproc/frame");
        // }

        posProcess1(enemyThreshold);

        // if (this->fc > 504 && this->fc < 1500)
        // {
        //     helpers::createImageFile(enemyThreshold, this->fc, "team_frames/aposproc/frame");
        // }

        // cv::imshow("ThresholdSegTeam", enemyThreshold);

        cv::findContours(enemyThreshold, tempContours, cv::RETR_LIST, cv::CHAIN_APPROX_SIMPLE, offset);

        for (int j = 0; j < tempContours.size(); ++j)
        {
            cv::Moments teamMoments;
            double teamArea;

            teamMoments = cv::moments(tempContours[j]);

            teamArea = teamMoments.m00;

            if (teamArea >= MIN_OBJECT_AREA && teamArea <= MAX_OBJECT_AREA)
            {
                teamContours->push_back(tempContours[j]);

                found[i] = true;
            }
        }
        if (!found[i])
        {
            break;
        }
    }

    this->teamFound = found[0] && found[1] && found[2];
}

void Segmentation2::segmentPlayers(cv::Mat preProcessedImg, std::vector<std::vector<std::vector<cv::Point>>> *allPlayersContours)
{

    bool found[3] = {false, false, false};

    // #pragma omp parallel for
    for (int i = 0; i < 3; ++i)
    {
        cv::Scalar playerColorMin, playerColorMax;
        cv::Mat thresholdPlayer;
        std::vector<std::vector<cv::Point>> playersContours, playerContours;
        cv::Point offset;
        cv::Size wholeSize;

        playerColorMin = Global::getColors()->getRobotColorMin(i);
        playerColorMax = Global::getColors()->getRobotColorMax(i);

        int previous_x = Global::getAlliedRobots()[i]->getPosX();
        int previous_y = Global::getAlliedRobots()[i]->getPosY();

        previous_x - this->factor > 0 ? previous_x -= this->factor : previous_x = 0;

        previous_y - this->factor > 0 ? previous_y -= this->factor : previous_y = 0;

        if (previous_x > preProcessedImg.cols - this->WINDOW_SIZE)
            previous_x = preProcessedImg.cols - this->WINDOW_SIZE;

        if (previous_y > preProcessedImg.rows - this->WINDOW_SIZE)
            previous_y = preProcessedImg.rows - this->WINDOW_SIZE;

        cv::Mat roi(preProcessedImg, cv::Rect(previous_x, previous_y, this->WINDOW_SIZE, this->WINDOW_SIZE));

        roi.locateROI(wholeSize, offset);

        cv::inRange(roi, playerColorMin, playerColorMax, thresholdPlayer);

        // if (this->fc > 504 && this->fc < 1500)
        // {
        //     switch (i)
        //     {
        //     case 0:
        //         helpers::createImageFile(thresholdPlayer, this->fc, "dir_frames/player1/bposproc/frame");
        //         break;

        //     case 1:
        //         helpers::createImageFile(thresholdPlayer, this->fc, "dir_frames/player2/bposproc/frame");
        //         break;

        //     case 2:
        //         helpers::createImageFile(thresholdPlayer, this->fc, "dir_frames/player3/bposproc/frame");
        //         break;
        //     }
        // }

        posProcessRole(thresholdPlayer);

        // if (this->fc > 504 && this->fc < 1500)
        // {
        //     switch (i)
        //     {
        //     case 0:
        //         // cv::imshow("ThresholdSegPlayer1", thresholdPlayer);
        //         helpers::createImageFile(thresholdPlayer, this->fc, "dir_frames/player1/aposproc/frame");
        //         break;

        //     case 1:
        //         // cv::imshow("ThresholdSegPlayer2", thresholdPlayer);
        //         helpers::createImageFile(thresholdPlayer, this->fc, "dir_frames/player2/aposproc/frame");
        //         break;

        //     case 2:
        //         // cv::imshow("ThresholdSegPlayer3", thresholdPlayer);
        //         helpers::createImageFile(thresholdPlayer, this->fc, "dir_frames/player3/aposproc/frame");
        //         break;
        //     }
        // }

        cv::findContours(thresholdPlayer, playerContours, cv::RETR_LIST, cv::CHAIN_APPROX_SIMPLE, offset);

        if (playerContours.size() >= 1)
        {
            for (auto j = 0ul; j < playerContours.size(); j++)
            {

                cv::Moments playerMoment;
                double playerArea;

                playerMoment = cv::moments(playerContours[j]);

                playerArea = playerMoment.m00;

                // std::cout << "Area player [" << i << "]["<< j << "][" << this->fc << "] = " << playerArea << std::endl;

                if (playerArea >= MIN_DIRECT_AREA && playerArea <= MAX_DIRECT_AREA)
                {
                    playersContours.push_back(playerContours[j]);

                    found[i] = true;
                }
            }
        }

        if (!found[i])
            break;

        (*allPlayersContours)[i] = playersContours;
    }

    this->rolesFound = found[0] && found[1] && found[2];
}

void Segmentation2::segmentBall(cv::Mat preProcessedImg, std::vector<cv::Point> *ballContour)
{
    std::vector<std::vector<cv::Point>> contours;
    cv::Mat enemyThreshold;
    cv::Scalar ballMin, ballMax;
    int previous_x, previous_y;
    cv::Point offset;
    cv::Size wholeSize;

    ballMin = Global::getBall()->getHSVMin();
    ballMax = Global::getBall()->getHSVMax();

    previous_x = Global::getBall()->getPosX();
    previous_y = Global::getBall()->getPosY();

    previous_x - this->factor > 0 ? previous_x -= this->factor : previous_x = 0;

    previous_y - this->factor > 0 ? previous_y -= this->factor : previous_y = 0;

    if (previous_x > preProcessedImg.cols - this->WINDOW_SIZE)
        previous_x = preProcessedImg.cols - this->WINDOW_SIZE;

    if (previous_y > preProcessedImg.rows - this->WINDOW_SIZE)
        previous_y = preProcessedImg.rows - this->WINDOW_SIZE;

    cv::Mat roi(preProcessedImg, cv::Rect(previous_x, previous_y, this->WINDOW_SIZE, this->WINDOW_SIZE));

    roi.locateROI(wholeSize, offset);

    cv::inRange(roi, ballMin, ballMax, enemyThreshold);

    // if (this->fc > 504 && this->fc < 1500)
    // {
    //     helpers::createImageFile(enemyThreshold, this->fc, "ball_frames/bposproc/frame");
    // }

    posProcessBall(enemyThreshold);

    // if (this->fc > 504 && this->fc < 1500)
    // {
    //     helpers::createImageFile(enemyThreshold, this->fc, "ball_frames/aposproc/frame");
    // }

    //cv::imshow("ThresholdSegBall", enemyThreshold);

    cv::findContours(enemyThreshold, contours, cv::RETR_LIST, cv::CHAIN_APPROX_SIMPLE, offset);

    for (int i = 0; i < contours.size(); ++i)
    {
        cv::Moments ballMoment;
        double ballArea;

        ballMoment = cv::moments(contours[i]);

        ballArea = ballMoment.m00;

        // std::cout<< "Area ball " << i << " = " << playerArea << std::endl;

        if (ballArea >= MIN_BALL_AREA && ballArea <= MAX_BALL_AREA)
        {
            *ballContour = contours[i];

            this->ballFound = true;

            break;
        }
    }
}

void Segmentation2::segmentEnemy(cv::Mat preProcessedImg, std::vector<std::vector<cv::Point>> *enemyContours)
{
    bool found[3] = {false, false, false};

    for (int i = 0; i < 3; ++i)
    {

        std::vector<std::vector<cv::Point>> tempContours;
        cv::Mat enemyThreshold;
        cv::Scalar enemyMin, enemyMax;
        int previous_x, previous_y;
        cv::Point offset;
        cv::Size wholeSize;

        enemyMin = Global::getColors()->getEnemyMin();
        enemyMax = Global::getColors()->getEnemyMax();

        previous_x = Global::getEnemyRobots()[i]->getPosX();
        previous_y = Global::getEnemyRobots()[i]->getPosY();

        previous_x - this->factor > 0 ? previous_x -= this->factor : previous_x = 0;
        previous_y - this->factor > 0 ? previous_y -= this->factor : previous_y = 0;

        if (previous_x > preProcessedImg.cols - this->WINDOW_SIZE)
            previous_x = preProcessedImg.cols - this->WINDOW_SIZE;

        if (previous_y > preProcessedImg.rows - this->WINDOW_SIZE)
            previous_y = preProcessedImg.rows - this->WINDOW_SIZE;

        cv::Mat roi(preProcessedImg, cv::Rect(previous_x, previous_y, this->WINDOW_SIZE, this->WINDOW_SIZE));

        roi.locateROI(wholeSize, offset);

        cv::inRange(roi, enemyMin, enemyMax, enemyThreshold);

        // if (this->fc > 504 && this->fc < 1500)
        // {
        //     helpers::createImageFile(enemyThreshold, this->fc, "enemy_frames/bposproc/frame");
        // }

        posProcessEnemy(enemyThreshold);

        // if (this->fc > 504 && this->fc < 1500)
        // {
        //     helpers::createImageFile(enemyThreshold, this->fc, "enemy_frames/aposproc/frame");
        // }

        //cv::imshow("ThresholdSegEnemy", enemyThreshold);

        cv::findContours(enemyThreshold, tempContours, cv::RETR_LIST, cv::CHAIN_APPROX_SIMPLE, offset);

        for (int j = 0; j < tempContours.size(); ++j)
        {
            cv::Moments enemyMoments;
            double enemyArea;

            enemyMoments = cv::moments(tempContours[j]);

            enemyArea = enemyMoments.m00;

            if (enemyArea >= MIN_OBJECT_AREA && enemyArea <= MAX_OBJECT_AREA)
            {
                enemyContours->push_back(tempContours[j]);

                found[i] = true;
            }
        }
        if (!found[i])
            break;
    }

    this->enemiesFound = found[0] && found[1] && found[2];
}

void Segmentation2::posProcess1(cv::Mat &img)
{

    PreProcess::morphOps(img, 3, cv::MORPH_OPEN, 1, PreProcess::morphType::CROSS);

    // PreProcess::singleMorph(img, 3, PreProcess::singleOP::ERODE);
}

void Segmentation2::posProcessEnemy(cv::Mat &img)
{
    PreProcess::morphOps(img, 3, cv::MORPH_OPEN, 1, PreProcess::morphType::CROSS);

    PreProcess::morphOps(img, 5, cv::MORPH_CLOSE, 2, PreProcess::morphType::CROSS);
}

void Segmentation2::posProcessBall(cv::Mat &img)
{

    PreProcess::morphOps(img, 3, cv::MORPH_OPEN, 1, PreProcess::morphType::CROSS);

    PreProcess::morphOps(img, 5, cv::MORPH_CLOSE, 2, PreProcess::morphType::CROSS);
}

void Segmentation2::posProcessRole(cv::Mat &img)
{

    PreProcess::morphOps(img, 3, cv::MORPH_OPEN, 1, PreProcess::morphType::CROSS);

    PreProcess::singleMorph(img, 5, PreProcess::singleOP::DILATE);
}