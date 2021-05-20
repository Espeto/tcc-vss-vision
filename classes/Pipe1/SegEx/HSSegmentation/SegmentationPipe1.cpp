#include "SegmentationPipe1.hpp"
#include "../../../../libs/globals/Global.h"
#include "../../../../libs/libvision/PreProcess.h"
#include "../../../../libs/helpers/helper.h"
#include <opencv2/highgui.hpp>
#include <iostream>

SegmentationPipe1::SegmentationPipe1()
{
    this->fc = 1;
}

objectsContours SegmentationPipe1::execute(cv::Mat preProcessedImg)
{
    std::vector<std::vector<std::vector<cv::Point>>> allPlayersContours(3, std::vector<std::vector<cv::Point>>());
    std::vector<std::vector<cv::Point>> teamContours, enemyContours;
    std::vector<cv::Point> ballContour;

    this->segmentBall(preProcessedImg, &ballContour);
    this->segmentTeam(preProcessedImg, &teamContours);
    this->segmentPlayers(preProcessedImg, &allPlayersContours);
    this->segmentEnemy(preProcessedImg, &enemyContours);

    this->fc++;

    return {allPlayersContours, teamContours, ballContour, enemyContours};
}

void SegmentationPipe1::segmentTeam(cv::Mat preProcessedImg, std::vector<std::vector<cv::Point>> *teamContours)
{
    cv::Mat teamThreshold;
    cv::Scalar teamMin, teamMax;

    teamMin = Global::getColors()->getAllyMin();
    teamMax = Global::getColors()->getAllyMax();

    cv::inRange(preProcessedImg, teamMin, teamMax, teamThreshold);

    // helpers::createImageFile(teamThreshold, this->fc, "team_frames/bposproc/frame");

    posProcess1(teamThreshold);

    // helpers::createImageFile(teamThreshold, this->fc, "team_frames/aposproc/frame");

    cv::imshow("ThresholdSegTeam", teamThreshold);

    cv::findContours(teamThreshold, *teamContours, cv::RETR_LIST, cv::CHAIN_APPROX_SIMPLE);
}

void SegmentationPipe1::segmentPlayers(cv::Mat preProcessedImg, std::vector<std::vector<std::vector<cv::Point>>> *allPlayersContours)
{
    for (int i = 0; i < 3; ++i)
    {
        cv::Scalar playerColorMin, playerColorMax;
        cv::Mat thresholdPlayer;
        std::vector<std::vector<cv::Point>> playersContours, playerContours;

        playerColorMin = Global::getColors()->getRobotColorMin(i);
        playerColorMax = Global::getColors()->getRobotColorMax(i);

        cv::inRange(preProcessedImg, playerColorMin, playerColorMax, thresholdPlayer);

        // switch (i)
        // {
        // case 0:
        //     helpers::createImageFile(thresholdPlayer, this->fc, "dir_frames/player1/bposproc/frame");
        //     break;

        // case 1:
        //     helpers::createImageFile(thresholdPlayer, this->fc, "dir_frames/player2/bposproc/frame");
        //     break;

        // case 2:
        //     helpers::createImageFile(thresholdPlayer, this->fc, "dir_frames/player3/bposproc/frame");
        //     break;
        // }

        posProcessRole(thresholdPlayer);

        switch (i)
        {
        case 0:
            cv::imshow("ThresholdSegPlayer1", thresholdPlayer);
            //helpers::createImageFile(thresholdPlayer, this->fc, "dir_frames/player1/aposproc/frame");
            break;

        case 1:
            cv::imshow("ThresholdSegPlayer2", thresholdPlayer);
            //helpers::createImageFile(thresholdPlayer, this->fc, "dir_frames/player2/aposproc/frame");
            break;

        case 2:
            cv::imshow("ThresholdSegPlayer3", thresholdPlayer);
            //helpers::createImageFile(thresholdPlayer, this->fc, "dir_frames/player3/aposproc/frame");
            break;
        }

        cv::findContours(thresholdPlayer, playerContours, cv::RETR_LIST, cv::CHAIN_APPROX_SIMPLE);

        if (playerContours.size() >= 1)
        {
            for (auto j = 0ul; j < playerContours.size(); j++)
            {

                cv::Moments playerMoment;
                double playerArea;

                playerMoment = cv::moments(playerContours[j]);

                playerArea = playerMoment.m00;

                std::cout<< "Area player " << i << " = " << playerArea << std::endl;

                if (playerArea >= MIN_DIRECT_AREA && playerArea <= MAX_DIRECT_AREA)
                {
                    playersContours.push_back(playerContours[j]);
                }
            }
        }

        (*allPlayersContours)[i] = playersContours;
    }
}

void SegmentationPipe1::segmentBall(cv::Mat preProcessedImg, std::vector<cv::Point> *ballContour)
{
    std::vector<std::vector<cv::Point>> contours;
    cv::Mat ballThreshold;
    cv::Scalar ballMin, ballMax;

    ballMin = Global::getBall()->getHSVMin();
    ballMax = Global::getBall()->getHSVMax();

    cv::inRange(preProcessedImg, ballMin, ballMax, ballThreshold);

    // helpers::createImageFile(ballThreshold, this->fc, "ball_frames/bposproc/frame");

    posProcessBall(ballThreshold);

    // helpers::createImageFile(ballThreshold, this->fc, "ball_frames/aposproc/frame");

    cv::imshow("ThresholdSegBall", ballThreshold);

    cv::findContours(ballThreshold, contours, cv::RETR_LIST, cv::CHAIN_APPROX_SIMPLE);

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

            break;
        }
    }
}

void SegmentationPipe1::segmentEnemy(cv::Mat preProcessedImg, std::vector<std::vector<cv::Point>> *enemyContours)
{
    cv::Mat enemyThreshold;
    cv::Scalar enemyMin, enemyMax;

    enemyMin = Global::getColors()->getEnemyMin();
    enemyMax = Global::getColors()->getEnemyMax();

    cv::inRange(preProcessedImg, enemyMin, enemyMax, enemyThreshold);

    // helpers::createImageFile(enemyThreshold, this->fc, "enemy_frames/bposproc/frame");

    posProcess1(enemyThreshold);

    // helpers::createImageFile(enemyThreshold, this->fc, "enemy_frames/aposproc/frame");

    // cv::imshow("ThresholdSegEnemy", enemyThreshold);

    cv::findContours(enemyThreshold, *enemyContours, cv::RETR_LIST, cv::CHAIN_APPROX_SIMPLE);
}

void SegmentationPipe1::posProcess1(cv::Mat &img)
{

    PreProcess::morphOps(img, 3, cv::MORPH_OPEN, 1, PreProcess::morphType::CROSS);

    // PreProcess::singleMorph(img, 3, PreProcess::singleOP::ERODE);
}



void SegmentationPipe1::posProcessBall(cv::Mat &img)
{

    PreProcess::morphOps(img, 3, cv::MORPH_OPEN, 1, PreProcess::morphType::CROSS);

    PreProcess::morphOps(img, 5, cv::MORPH_CLOSE, 2, PreProcess::morphType::CROSS);

    // PreProcess::singleMorph(img, 5, PreProcess::singleOP::DILATE);
}

void SegmentationPipe1::posProcessRole(cv::Mat &img)
{

    PreProcess::morphOps(img, 3, cv::MORPH_OPEN, 1, PreProcess::morphType::CROSS);

    PreProcess::singleMorph(img, 5, PreProcess::singleOP::DILATE);
}