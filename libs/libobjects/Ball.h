/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/*
 * File:   Ball.h
 * Author: gilberto
 *
 * Created on 20 de Outubro de 2018, 10:57
 */

#pragma once

#include <opencv2/opencv.hpp>
#include <opencv2/imgcodecs.hpp>

class Ball {
public:
    friend class VisionManager;

    Ball();
    virtual ~Ball();

    int getPosX() const;
    int getPosY() const;

    void setPosX(int x);
    void setPosY(int y);

    void setHSVMax(cv::Scalar HSV);
    void setHSVMin(cv::Scalar HSV);

    cv::Scalar getHSVMax() const;
    cv::Scalar getHSVMin() const;

private:
    int X, Y;
    cv::Scalar HSVMax, HSVMin;

};
