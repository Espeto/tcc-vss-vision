#include "ReducedSeg.hpp"
#include "../../../../libs/globals/Global.h"
#include "../../../../libs/libvision/ImgProcess.h"
#include "../../../../libs/helpers/helper.h"
#include <opencv2/highgui.hpp>
#include <iostream>
#include <omp.h>

// Helper no caso de não encontrar um objeto com a segmentação reduzida
// Tem que melhorar isso aqui
#include "../HSSegFull/FullSeg.hpp"

ReducedSeg::ReducedSeg()
{
    this->factor = WINDOW_SIZE / 2;

    this->ballFound = this->teamFound = this->rolesFound = this->enemiesFound = false;
}

bool ReducedSeg::isAllFound()
{
    return this->ballFound && this->teamFound && this->rolesFound && this->enemiesFound;
}

objectsContours ReducedSeg::execute(cv::Mat preProcessedImg)
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

    // #pragma omp taskwait

    if (!isAllFound())
    {
        return FullSeg().execute(preProcessedImg);
    }

    return {allPlayersContours, teamContours, ballContour, enemyContours};
}

void ReducedSeg::segmentTeam(cv::Mat preProcessedImg, std::vector<std::vector<cv::Point>> *teamContours)
{
    std::vector<std::vector<cv::Point>> tempContours;
    cv::Mat teamThreshold;
    cv::Scalar teamMin, teamMax;
    bool found[3] = {false, false, false};
    cv::Point offset;
    cv::Size wholeSize;

    teamMin = Global::getColors()->getAllyMin();
    teamMax = Global::getColors()->getAllyMax();

    for (int i = 0; i < 3; ++i)
    {
        teamThreshold = cv::Mat::zeros(preProcessedImg.size(), CV_8UC1);

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

        cv::inRange(roi, teamMin, teamMax, teamThreshold);

        posProcess1(teamThreshold);

        cv::findContours(teamThreshold, tempContours, cv::RETR_LIST, cv::CHAIN_APPROX_SIMPLE, offset);

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

void ReducedSeg::segmentPlayers(cv::Mat preProcessedImg, std::vector<std::vector<std::vector<cv::Point>>> *allPlayersContours)
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

        posProcessRole(thresholdPlayer);

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

void ReducedSeg::segmentBall(cv::Mat preProcessedImg, std::vector<cv::Point> *ballContour)
{
    std::vector<std::vector<cv::Point>> contours;
    cv::Mat ballThreshold;
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

    cv::inRange(roi, ballMin, ballMax, ballThreshold);

    posProcessBall(ballThreshold);

    cv::findContours(ballThreshold, contours, cv::RETR_LIST, cv::CHAIN_APPROX_SIMPLE, offset);

    for (int i = 0; i < contours.size(); ++i)
    {
        cv::Moments ballMoment;
        double ballArea;

        ballMoment = cv::moments(contours[i]);

        ballArea = ballMoment.m00;

        if (ballArea >= MIN_BALL_AREA && ballArea <= MAX_BALL_AREA)
        {
            *ballContour = contours[i];

            this->ballFound = true;

            break;
        }
    }
}

void ReducedSeg::segmentEnemy(cv::Mat preProcessedImg, std::vector<std::vector<cv::Point>> *enemyContours)
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

        posProcessEnemy(enemyThreshold);

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

void ReducedSeg::posProcess1(cv::Mat &img)
{

    ImgProcess::morphOps(img, 3, cv::MORPH_OPEN, 1, ImgProcess::morphType::CROSS);

}

void ReducedSeg::posProcessEnemy(cv::Mat &img)
{
    ImgProcess::morphOps(img, 3, cv::MORPH_OPEN, 1, ImgProcess::morphType::CROSS);

}

void ReducedSeg::posProcessBall(cv::Mat &img)
{

    ImgProcess::morphOps(img, 3, cv::MORPH_OPEN, 1, ImgProcess::morphType::CROSS);

}

void ReducedSeg::posProcessRole(cv::Mat &img)
{

    ImgProcess::morphOps(img, 3, cv::MORPH_OPEN, 1, ImgProcess::morphType::CROSS);

    ImgProcess::singleMorph(img, 5, ImgProcess::singleOP::DILATE);
}