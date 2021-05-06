/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/*
 * File:   PreProcess.cpp
 * Author: gilberto
 *
 * Created on 10 de Setembro de 2018, 23:05
 */

#include "PreProcess.h"

void PreProcess::morphOps(cv::Mat &thresh, int sizeElement, int op, int it, morphType type) {
    int shapeType;

    switch(type) {
        case morphType::BALL://bola
            shapeType = cv::MORPH_ELLIPSE;
        break;

        case morphType::ROBOT:
            shapeType = cv::MORPH_RECT;
        break;

        case morphType::CROSS:
            shapeType = cv::MORPH_CROSS;
        break;
    }

    cv::Mat kernelElement = cv::getStructuringElement(shapeType, cv::Size(sizeElement, sizeElement));

    cv::morphologyEx(thresh, thresh, op, kernelElement, cv::Point(-1,-1), it);
}

void PreProcess::singleMorph(cv::Mat &thresh, int sizeElement, singleOP op) {

    cv::Mat kernelElement = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(sizeElement, sizeElement));

    switch(op) {
        case singleOP::ERODE://erosão
            cv::erode(thresh, thresh, kernelElement);
        break;
        case singleOP::DILATE://dilatação
            cv::dilate(thresh,thresh, kernelElement);
        break;
    }

}

void PreProcess::applyBlur(cv::Mat &inputImg, cv::Mat &result, int size, smoothType type) {

    switch(type) {
        case smoothType::GAUSSIAN:
            cv::GaussianBlur(inputImg, result, cv::Size(size, size), 0, 0);
            break;
        case smoothType::MEDIAN:
            cv::medianBlur(inputImg, result, size);
            break;
        case smoothType::AVERAGE:
            cv::blur(inputImg, result, cv::Size(size, size));
            break;
    }
}

