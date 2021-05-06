#include "ExtractionPipe1.hpp"
#include "../../../../libs/globals/Global.h"
#include <math.h>

ExtractionPipe1::ExtractionPipe1(){}

void ExtractionPipe1::execute(std::tuple<std::vector<std::vector<cv::Point>>, std::vector<std::vector<cv::Point>>> contours)
{
    std::vector<std::vector<cv::Point>> roleContour, teamContour;

    std::vector<int> alreadyUsed;

    alreadyUsed = std::vector<int>(teamContour.size(), 0);

    std::tie(roleContour, teamContour) = contours;

    for(int i = 0; i < roleContour.size(); ++i) {

        Robo *r = Global::getAlliedRobots()[i];

        cv::Moments roleMoments;
        double roleArea;

        roleMoments = cv::moments(roleContour[i]);

        roleArea = roleMoments.m00;

        int playerX = static_cast<int>(roleMoments.m10/roleArea);
        int playerY = static_cast<int>(roleMoments.m01/roleArea);

        for(int j = 0; j < teamContour.size(); ++j) {
            if (!alreadyUsed[j]) {
                cv::Moments teamMoments = cv::moments(teamContour[j]);
                double teamArea = teamMoments.m00;

                int teamX = static_cast<int>(teamMoments.m10/teamArea);
                int teamY = static_cast<int>(teamMoments.m01/teamArea);

                double xDif, yDif, dist;

                xDif = playerX - teamX;
                yDif = playerY - teamY;

                dist = std::sqrt(std::pow(xDif,2.0)+std::pow(yDif,2.0));

                if (dist >= 15 && dist <= 35) {
                    alreadyUsed[j] = 1;

                    int robot_x = static_cast<int>((playerX+teamX)/2);
                    int robot_y = static_cast<int>((playerY+teamY)/2);

                    double robotAngle = std::atan2(yDif, xDif) - M_PI/4;

                    r->setOrientation(robotAngle);

                    r->setPosX(robot_x);
                    r->setPosY(robot_y);
                }

            }
        }
    }

}