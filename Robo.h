/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/*
 * File:   Robo.h
 * Author: gilberto
 *
 * Created on 22 de Setembro de 2018, 14:49
 */
#pragma once

#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>

class Robo {
public:
    friend class VisionManager;

    Robo();
    virtual ~Robo();

    int getPosX() const {return x;}
    int getPosY() const {return y;}

    void setDesiredPosX(int x) {desiredX = x;}
    void setDesiredPosY(int y) {desiredY = y;}

    int getDirectionPosX() const {return directionX;}
    int getDirectionPosY() const {return directionY;}

    void setDirectionPosX(int x) {directionX = x;}
    void setDirectionPosY(int y) {directionY = y;}

    void setHSVMaxDefColor(cv::Scalar HSV);
    void setHSVMinDefColor(cv::Scalar HSV);

    cv::Scalar getHSVMaxDefColor() const;
    cv::Scalar getHSVMinDefColor() const;

    void setHSVMaxDirectColor(cv::Scalar HSV);
    void setHSVMinDirectColor(cv::Scalar HSV);

    cv::Scalar getHSVMaxDirectColor() const;
    cv::Scalar getHSVMinDirectColor() const;

    void setRobotAngle(int angle) {robotAngle = angle;}
    int getRobotAngle() {return robotAngle;}

    void setBallAngle(int angle) {ballAngle = angle;}
    int getBallAngle() {return ballAngle;}

private:
    void setPosX(int _x) {x = _x;}
    void setPosY(int _y) {y = _y;}

    int x, y;
    int desiredX;
    int desiredY;
    int directionX, directionY;
    int robotAngle, ballAngle;
    cv::Scalar HSVMaxDefColor, HSVMinDefColor;
    cv::Scalar HSVMaxDirectColor, HSVMinDirectColor;

};
