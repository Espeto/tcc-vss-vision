#include "SegmentationPipe1.hpp"
#include "../../../../libs/globals/Global.h"
#include "../../../../libs/libvision/PreProcess.h"

SegmentationPipe1::SegmentationPipe1(){}

std::tuple<std::vector<std::vector<cv::Point>>, std::vector<std::vector<cv::Point>>> SegmentationPipe1::execute(cv::Mat preProcessedImg)
{
    std::vector<std::vector<cv::Point>> teamContours, playersContours;

    //Track cor do time
    {
        cv::Mat teamThreshold;
        cv::Scalar teamMin, teamMax;

        teamMin = Global::getColors()->getAllyMin();
        teamMax = Global::getColors()->getAllyMax();

        inRange(preProcessedImg, teamMin, teamMax, teamThreshold);

        PreProcess::morphOps(teamThreshold, 3, cv::MORPH_OPEN, 1, PreProcess::morphType::CROSS);

        PreProcess::singleMorph(teamThreshold, 3, PreProcess::singleOP::ERODE);

        findContours(teamThreshold, teamContours, cv::RETR_LIST, cv::CHAIN_APPROX_SIMPLE);
    }

    //Track cor do jogador
    for (int i = 0; i < 1; ++i)
    {
        cv::Scalar playerColorMin, playerColorMax;
        cv::Mat thresholdPlayer;
        std::vector<std::vector<cv::Point>> playerContours;

        playerColorMin = Global::getColors()->getRobotColorMin(i);
        playerColorMax = Global::getColors()->getRobotColorMax(i);

        inRange(preProcessedImg, playerColorMin, playerColorMax, thresholdPlayer);

        PreProcess::morphOps(thresholdPlayer, 3, cv::MORPH_OPEN, 1, PreProcess::morphType::CROSS);

        PreProcess::singleMorph(thresholdPlayer, 3, PreProcess::singleOP::ERODE);

        findContours(thresholdPlayer, playerContours, cv::RETR_LIST, cv::CHAIN_APPROX_SIMPLE);

        if (playerContours.size() >= 1)
        {
            for (auto j = 0ul; j < playerContours.size(); j++)
            {

                cv::Moments playerMoment;
                double playerArea;

                playerMoment = moments(playerContours[j]);

                playerArea = playerMoment.m00;

                if (playerArea >= MIN_DIRECT_AREA && playerArea <= MAX_DIRECT_AREA)
                {
                    playersContours.push_back(playerContours[j]);
                }
            }
        }
    }

    return {playersContours, teamContours};
}