/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/*
 * File:   ImgProcess.cpp
 * Author: gilberto
 *
 * Created on 10 de Setembro de 2018, 23:05
 */

#include "ImgProcess.h"

void ImgProcess::morphOps(cv::Mat &thresh, int sizeElement, int op, int it, morphType type) {
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

void ImgProcess::singleMorph(cv::Mat &thresh, int sizeElement, singleOP op) {

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

void ImgProcess::applyBlur(cv::Mat &inputImg, cv::Mat &result, int size, smoothType type) {

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

void ImgProcess::gammaCorrection(cv::Mat src, cv::Mat &dst, double gamma_) {

    cv::Mat lookUpTable(1, 256, CV_8U);

    uchar* p = lookUpTable.ptr();

    for (int i = 0; i < 256; ++i) {
        p[i] = cv::saturate_cast<uchar>(pow(i / 255.0, gamma_ ) * 255.0);
    }

    dst = src.clone();
    cv::LUT(src, lookUpTable, dst);
}

