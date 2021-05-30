/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/*
 * File:   Ball.cpp
 * Author: gilberto
 *
 * Created on 20 de Outubro de 2018, 10:57
 */

#include "Ball.h"

using namespace cv;

Ball::Ball() {

    Ball::setHSVMin(Scalar(0, 147, 0));
    Ball::setHSVMax(Scalar(23, 255, 255));
}

Ball::~Ball() {
}

void Ball::setHSVMax(Scalar HSV) {
    Ball::HSVMax = HSV;
}

void Ball::setHSVMin(Scalar HSV) {
    Ball::HSVMin = HSV;
}

Scalar Ball::getHSVMax() const {
    return Ball::HSVMax;
}

Scalar Ball::getHSVMin() const {
    return Ball::HSVMin;
}

void Ball::setPosX(int x) {
    Ball::X = x;
}

void Ball::setPosY(int y) {
    Ball::Y = y;
}

int Ball::getPosX() const {
    return Ball::X;
}

int Ball::getPosY() const {
    return Ball::Y;
}
