#include "SegmentationPipe1.hpp"
#include "../../../../libs/globals/Global.h"
#include "../../../../libs/libvision/PreProcess.h"
#include "../../../../libs/helpers/helper.h"
#include <opencv2/highgui.hpp>
#include <iostream>

SegmentationPipe1::SegmentationPipe1(){
    this->fc = 1;
}

std::tuple<std::vector<std::vector<std::vector<cv::Point>>>, 
std::vector<std::vector<cv::Point>>> SegmentationPipe1::execute(cv::Mat preProcessedImg)
{
    std::vector<std::vector<std::vector<cv::Point>>> allPlayersContours(3, std::vector<std::vector<cv::Point>>());
    std::vector<std::vector<cv::Point>> teamContours;

    //Track cor do time
    {
        cv::Mat teamThreshold;
        cv::Scalar teamMin, teamMax;

        teamMin = Global::getColors()->getAllyMin();
        teamMax = Global::getColors()->getAllyMax();

        cv::inRange(preProcessedImg, teamMin, teamMax, teamThreshold);

        helpers::createImageFile(teamThreshold, this->fc, "team_frames/bposproc/team");

        posProcess1(teamThreshold);

        helpers::createImageFile(teamThreshold, this->fc, "team_frames/aposproc/team");

        cv::imshow("ThresholdSegTeam", teamThreshold);

        cv::findContours(teamThreshold, teamContours, cv::RETR_LIST, cv::CHAIN_APPROX_SIMPLE);
    }

    //Track cor do jogador
    for (int i = 0; i < 3; ++i)
    {
        cv::Scalar playerColorMin, playerColorMax;
        cv::Mat thresholdPlayer;
        std::vector<std::vector<cv::Point>> playersContours, playerContours;

        playerColorMin = Global::getColors()->getRobotColorMin(i);
        playerColorMax = Global::getColors()->getRobotColorMax(i);

        cv::inRange(preProcessedImg, playerColorMin, playerColorMax, thresholdPlayer);

        switch (i)
        {
        case 0:
            helpers::createImageFile(thresholdPlayer, this->fc, "dir_frames/player1/bposproc/frame");
            break;
        
        case 1:
            helpers::createImageFile(thresholdPlayer, this->fc, "dir_frames/player2/bposproc/frame");
            break;

        case 2:
            helpers::createImageFile(thresholdPlayer, this->fc, "dir_frames/player3/bposproc/frame");
            break;
        }

        posProcess1(thresholdPlayer);

        switch (i)
        {
        case 0:
            cv::imshow("ThresholdSegPlayer1", thresholdPlayer);
            helpers::createImageFile(thresholdPlayer, this->fc, "dir_frames/player1/aposproc/frame");
            break;
        
        case 1:
            cv::imshow("ThresholdSegPlayer2", thresholdPlayer);
            helpers::createImageFile(thresholdPlayer, this->fc, "dir_frames/player2/aposproc/frame");
            break;

        case 2:
            cv::imshow("ThresholdSegPlayer3", thresholdPlayer);
            helpers::createImageFile(thresholdPlayer, this->fc, "dir_frames/player3/aposproc/frame");
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

                // std::cout<< "Area player " << i << " = " << playerArea << std::endl;

                if (playerArea >= MIN_DIRECT_AREA && playerArea <= MAX_DIRECT_AREA)
                {
                    playersContours.push_back(playerContours[j]);
                }
            }
        }

        allPlayersContours[i] = playersContours;
    }

    this->fc++;

    return {allPlayersContours, teamContours};
}

void SegmentationPipe1::posProcess1(cv::Mat &img) {

    PreProcess::morphOps(img, 3, cv::MORPH_OPEN, 1, PreProcess::morphType::CROSS);

    PreProcess::singleMorph(img, 3, PreProcess::singleOP::ERODE);
}