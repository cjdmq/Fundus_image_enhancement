#pragma once

#include <vector>
#include<random>
#include "opencv2/core/core.hpp"
#include <opencv2/opencv.hpp>

class CalMath
{
public:

    static CalMath* Instance();

    //计算▽u1
    cv::Mat gradient_one(cv::Mat& src);

    //计算▽u2
    cv::Mat gradient_two(cv::Mat& src);

    //计算离散总变差
    double total_variation(cv::Mat& gradient_one, cv::Mat& gradient_two);

    //计算散度
    cv::Mat divergence_operator(cv::Mat& src);


    



private:
    CalMath();
    virtual ~CalMath();
    static CalMath* instance;

};