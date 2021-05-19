#include "ExtractionPipe1.hpp"
#include "../../../../libs/globals/Global.h"
#include <math.h>
#include <iostream>

ExtractionPipe1::ExtractionPipe1() {}

void ExtractionPipe1::execute(objectsContours contours)
{
    std::vector<std::vector<cv::Point>> teamContour;
    std::vector<std::vector<std::vector<cv::Point>>> rolesContour;
    std::vector<cv::Point> ballContour;
    std::vector<std::vector<cv::Point>> enemyContours;

    rolesContour = contours.rolesContour;
    teamContour = contours.teamContours;
    ballContour = contours.ballContour;
    enemyContours = contours.enemyContours;

    // std::cout << "Size All roles [Tem que ser 3 ou menos] = " << rolesContour.size() << " | " << "Size team = " << teamContour.size() << std::endl;
    extractPlayer(rolesContour, teamContour);
    extractBall(ballContour);
    //extractEnemy(enemyContours);
    
}

void ExtractionPipe1::extractPlayer(
    std::vector<std::vector<std::vector<cv::Point>>> rolesContour,
    std::vector<std::vector<cv::Point>> teamContour)
{
    std::vector<int> alreadyUsed;

    alreadyUsed = std::vector<int>(teamContour.size(), 0);

    for (int i = 0; i < rolesContour.size(); ++i)
    {

        bool found = false;

        Robo *r = Global::getAlliedRobots()[i];

        std::vector<std::vector<cv::Point>> roleContour = rolesContour[i];

        std::cout << "Num roles for robot [" << i << "] = " << roleContour.size() << std::endl;

        for (int k = 0; k < roleContour.size(); ++k)
        {
            cv::Moments roleMoments;
            double roleArea;

            roleMoments = cv::moments(roleContour[k]);

            roleArea = roleMoments.m00;

            int playerX = static_cast<int>(roleMoments.m10 / roleArea);
            int playerY = static_cast<int>(roleMoments.m01 / roleArea);

            for (int j = 0; j < teamContour.size(); ++j)
            {

                if (!alreadyUsed[j])
                {
                    cv::Moments teamMoments = cv::moments(teamContour[j]);
                    double teamArea = teamMoments.m00;

                    int teamX = static_cast<int>(teamMoments.m10 / teamArea);
                    int teamY = static_cast<int>(teamMoments.m01 / teamArea);

                    double xDif, yDif, dist;

                    xDif = playerX - teamX;
                    yDif = playerY - teamY;

                    dist = std::sqrt(std::pow(xDif, 2.0) + std::pow(yDif, 2.0));

                    std::cout << "Role [" << k << "] | Time [" << j << "] | Distancia = " << dist << std::endl;

                    if (dist > 11.0 && dist < 16.0)
                    {
                        alreadyUsed[j] = 1;

                        int robot_x = static_cast<int>((playerX + teamX) / 2);
                        int robot_y = static_cast<int>((playerY + teamY) / 2);

                        double robotAngle = std::atan2(yDif, xDif) - (M_PI / 4);

                        r->setOrientation(robotAngle);

                        r->setPosX(robot_x);
                        r->setPosY(robot_y);

                        found = true;

                        break;

                        // Se encontrou tem que dar continue no for e seguir pra cor
                        // do proximo role
                    }
                }
            }

            if (found) break;
        }
    }
}

void ExtractionPipe1::extractBall(std::vector<cv::Point> ballContour)
{
    cv::Moments ballMoments = cv::moments(ballContour);
    double ballArea = ballMoments.m00;

    auto *b = Global::getBall();

    int ball_x = static_cast<int>(ballMoments.m10 / ballArea);
    int ball_y = static_cast<int>(ballMoments.m01 / ballArea);

    b->setPosX(ball_x);
    b->setPosY(ball_y);
}

void extractEnemy(std::vector<std::vector<cv::Point>> enemyContours)
{
    for (int i = 0; i < enemyContours.size(); ++i) {

        Robo *enemy = Global::getEnemyRobots()[i];

        cv::Moments enemyMoments = cv::moments(enemyContours[i]);
        double enemyArea = enemyMoments.m00;

        int enemy_x = static_cast<int>(enemyMoments.m10 / enemyArea);
        int enemy_y = static_cast<int>(enemyMoments.m01 / enemyArea);

        enemy->setPosX(enemy_x);
        enemy->setPosY(enemy_y);
    }
}