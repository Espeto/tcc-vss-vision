/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/*
 * File:   Robo.cpp
 * Author: gilberto
 *
 * Created on 22 de Setembro de 2018, 14:49
 */

#include "Robo.h"

using namespace std;
using namespace cv;

Robo::Robo() {
    Robo::setHSVMinDirectColor(Scalar(0));
    Robo::setHSVMaxDirectColor(Scalar(255));

    Robo::setHSVMinDefColor(Scalar(0));
    Robo::setHSVMaxDefColor(Scalar(255));
}

Robo::~Robo() {
}

//Cor 1

void Robo::setHSVMaxDirectColor(Scalar HSV) {
    Robo::HSVMaxDirectColor = HSV;
}

void Robo::setHSVMinDirectColor(Scalar HSV) {
    Robo::HSVMinDirectColor = HSV;
}

Scalar Robo::getHSVMaxDirectColor() const {
    return Robo::HSVMaxDirectColor;
}

Scalar Robo::getHSVMinDirectColor() const {
    return Robo::HSVMinDirectColor;
}
/*--------------------------------------------------------*/

//Cor 2

void Robo::setHSVMaxDefColor(Scalar HSV) {
    Robo::HSVMaxDefColor = HSV;
}

void Robo::setHSVMinDefColor(Scalar HSV) {
    Robo::HSVMinDefColor = HSV;
}

Scalar Robo::getHSVMaxDefColor() const {
    return Robo::HSVMaxDefColor;
}

Scalar Robo::getHSVMinDefColor() const {
    return Robo::HSVMinDefColor;
}

/*--------------------------------------------------------*/
