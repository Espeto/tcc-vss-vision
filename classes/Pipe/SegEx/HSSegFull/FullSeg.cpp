#include "FullSeg.hpp"
#include "../../../../libs/globals/Global.h"
#include "../../../../libs/libvision/ImgProcess.h"
#include "../../../../libs/helpers/helper.h"
#include <opencv2/highgui.hpp>
#include <iostream>
#include <omp.h>

objectsContours FullSeg::execute(cv::Mat preProcessedImg)
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

    return {allPlayersContours, teamContours, ballContour, enemyContours};
}

void FullSeg::segmentTeam(cv::Mat preProcessedImg, std::vector<std::vector<cv::Point>> *teamContours)
{
    std::vector<std::vector<cv::Point>> tempContours;
    cv::Mat teamThreshold;
    cv::Scalar teamMin, teamMax;

    teamMin = Global::getColors()->getAllyMin();
    teamMax = Global::getColors()->getAllyMax();

    cv::inRange(preProcessedImg, teamMin, teamMax, teamThreshold);

    posProcess1(teamThreshold);

    cv::findContours(teamThreshold, *teamContours, cv::RETR_LIST, cv::CHAIN_APPROX_SIMPLE);

    for (int i = 0; i < tempContours.size(); ++i)
    {
        cv::Moments teamMoments;
        double teamArea;

        teamMoments = cv::moments(tempContours[i]);

        teamArea = teamMoments.m00;

        if (teamArea >= MIN_OBJECT_AREA && teamArea <= MAX_OBJECT_AREA)
        {
            teamContours->push_back(tempContours[i]);
        }
    }
}

void FullSeg::segmentPlayers(cv::Mat preProcessedImg, std::vector<std::vector<std::vector<cv::Point>>> *allPlayersContours)
{
    // #pragma omp parallel for
    for (int i = 0; i < 3; ++i)
    {
        cv::Scalar playerColorMin, playerColorMax;
        cv::Mat thresholdPlayer;
        std::vector<std::vector<cv::Point>> playersContours, playerContours;

        playerColorMin = Global::getColors()->getRobotColorMin(i);
        playerColorMax = Global::getColors()->getRobotColorMax(i);

        cv::inRange(preProcessedImg, playerColorMin, playerColorMax, thresholdPlayer);

        posProcessRole(thresholdPlayer);

        cv::findContours(thresholdPlayer, playerContours, cv::RETR_LIST, cv::CHAIN_APPROX_SIMPLE);

        if (playerContours.size() >= 1)
        {
            for (auto j = 0ul; j < playerContours.size(); j++)
            {

                cv::Moments playerMoment;
                double playerArea;

                playerMoment = cv::moments(playerContours[j]);

                playerArea = playerMoment.m00;

                if (playerArea >= MIN_DIRECT_AREA && playerArea <= MAX_DIRECT_AREA)
                {
                    playersContours.push_back(playerContours[j]);
                }
            }
        }

        (*allPlayersContours)[i] = playersContours;
    }
}

void FullSeg::segmentBall(cv::Mat preProcessedImg, std::vector<cv::Point> *ballContour)
{
    std::vector<std::vector<cv::Point>> contours;
    cv::Mat ballThreshold;
    cv::Scalar ballMin, ballMax;

    ballMin = Global::getBall()->getHSVMin();
    ballMax = Global::getBall()->getHSVMax();

    cv::inRange(preProcessedImg, ballMin, ballMax, ballThreshold);

    posProcessBall(ballThreshold);

    cv::findContours(ballThreshold, contours, cv::RETR_LIST, cv::CHAIN_APPROX_SIMPLE);

    for (int i = 0; i < contours.size(); ++i)
    {
        cv::Moments ballMoment;
        double ballArea;

        ballMoment = cv::moments(contours[i]);

        ballArea = ballMoment.m00;

        if (ballArea >= MIN_BALL_AREA && ballArea <= MAX_BALL_AREA)
        {
            *ballContour = contours[i];

            break;
        }
    }
}

void FullSeg::segmentEnemy(cv::Mat preProcessedImg, std::vector<std::vector<cv::Point>> *enemyContours)
{
    std::vector<std::vector<cv::Point>> tempContours;
    cv::Mat enemyThreshold;
    cv::Scalar enemyMin, enemyMax;

    enemyMin = Global::getColors()->getEnemyMin();
    enemyMax = Global::getColors()->getEnemyMax();

    cv::inRange(preProcessedImg, enemyMin, enemyMax, enemyThreshold);

    posProcessEnemy(enemyThreshold);

    cv::findContours(enemyThreshold, tempContours, cv::RETR_LIST, cv::CHAIN_APPROX_SIMPLE);

    for (int i = 0; i < tempContours.size(); ++i)
    {
        cv::Moments enemyMoments;
        double enemyArea;

        enemyMoments = cv::moments(tempContours[i]);

        enemyArea = enemyMoments.m00;

        if (enemyArea >= MIN_OBJECT_AREA && enemyArea <= MAX_OBJECT_AREA)
        {
            enemyContours->push_back(tempContours[i]);
        }
    }
}

void FullSeg::posProcess1(cv::Mat &img)
{

    ImgProcess::morphOps(img, 3, cv::MORPH_OPEN, 1, ImgProcess::morphType::CROSS);
}

void FullSeg::posProcessEnemy(cv::Mat &img)
{
    ImgProcess::morphOps(img, 3, cv::MORPH_OPEN, 1, ImgProcess::morphType::CROSS);
}

void FullSeg::posProcessBall(cv::Mat &img)
{

    ImgProcess::morphOps(img, 3, cv::MORPH_OPEN, 1, ImgProcess::morphType::CROSS);
}

void FullSeg::posProcessRole(cv::Mat &img)
{

    ImgProcess::morphOps(img, 3, cv::MORPH_OPEN, 1, ImgProcess::morphType::CROSS);

    ImgProcess::singleMorph(img, 5, ImgProcess::singleOP::DILATE);
}