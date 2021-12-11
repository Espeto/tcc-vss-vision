
/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/*
 * File:   ImgProcess.h
 * Author: gilberto
 *
 * Created on 10 de Setembro de 2018, 23:05
 */

#pragma once

#include <opencv2/imgproc.hpp>

namespace ImgProcess {

    enum class singleOP {
        DILATE = 1,
        ERODE = 2
    };

    enum class morphType {
        BALL = 1,
        ROBOT = 2,
        CROSS = 3
    };

    enum class smoothType {
        GAUSSIAN = 1,
        MEDIAN = 2,
        AVERAGE = 3

    };

    void applyBlur(cv::Mat &inputImg, cv::Mat &result, int size, smoothType type);
    void morphOps(cv::Mat &thresh, int sizeElement, int op, int it, morphType type);
    void singleMorph(cv::Mat &thresh, int sizeElement, singleOP op);
    void gammaCorrection(cv::Mat src, cv::Mat &dst, double gamma_);
}

