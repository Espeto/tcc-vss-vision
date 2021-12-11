#pragma once

#include "../../../../libs/interfaces/SegmentationInterface.hpp"

class FullSeg : public SegmentationInterface
{
public:
    FullSeg() = default;
    objectsContours execute(cv::Mat preProcessedImg);

private:
    const int MIN_DIRECT_AREA = 70;
    const int MAX_DIRECT_AREA = 450;

    const int MIN_BALL_AREA = 200;
    const int MAX_BALL_AREA = 500;

    const int MIN_OBJECT_AREA = 700;
    const int MAX_OBJECT_AREA = 900;

    void segmentTeam(cv::Mat preProcessedImg, std::vector<std::vector<cv::Point>> *teamContours);
    void segmentPlayers(cv::Mat preProcessedImg, std::vector<std::vector<std::vector<cv::Point>>> *allPlayersContours);
    void segmentBall(cv::Mat preProcessedImg, std::vector<cv::Point> *ballContour);
    void segmentEnemy(cv::Mat preProcessedImg, std::vector<std::vector<cv::Point>> *enemyContours);

    void posProcess1(cv::Mat &img);
    void posProcessBall(cv::Mat &img);
    void posProcessRole(cv::Mat &img);
    void posProcessEnemy(cv::Mat &img);
};