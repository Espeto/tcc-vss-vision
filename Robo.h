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

class Robo {
public:
    friend class VisionManager;

    Robo();
    virtual ~Robo();

    int getPosX() const {return x;}
    int getPosY() const {return y;}

    void setPosX(int _x) {x = _x;}
    void setPosY(int _y) {y = _y;}

    double getOrientation() const {return orientation;}
    void setOrientation(double _orientation) {orientation = _orientation;}

private:

    int x, y;
    double orientation;

};
