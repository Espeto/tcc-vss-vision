/*
 * File:   VisionManager.h
 * Author: gilberto
 *
 * Created on 21 de Agosto de 2018, 17:28
 */

#pragma once

#include <string>

#include <opencv2/core.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>

#include "PreProcess.h"
#include "../libobjects/Robo.h"
#include "../libobjects/Ball.h"

class VisionManager
{
public:
    VisionManager(std::string img_path);

    void trackObjects();

private:
    cv::Mat originalImage;
    cv::Mat blurImage;
    cv::Mat hsvImage;

    const int FRAME_WIDTH = 640;
    const int FRAME_HEIGHT = 480;

    const int MIN_DIRECT_AREA = 500;
    const int MAX_DIRECT_AREA = 2000;

    const int MIN_BALL_AREA = 200;
    const int MAX_BALL_AREA = 700;

    const int MIN_OBJECT_AREA = 600;
    const int MAX_OBEJCT_AREA = 20000;

    void trackEnemyRobots();
    void trackAlliedRobots();
    void drawObject(int x, int y, cv::Scalar color);
    void trackBall();
};
