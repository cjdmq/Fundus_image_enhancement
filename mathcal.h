#pragma once

#include <vector>
#include<random>
#include "opencv2/core/core.hpp"
#include <opencv2/opencv.hpp>

class CalMath
{
public:

    static CalMath* Instance();

    //���㨌u1
    cv::Mat gradient_one(cv::Mat& src);

    //���㨌u2
    cv::Mat gradient_two(cv::Mat& src);

    //������ɢ�ܱ��
    double total_variation(cv::Mat& gradient_one, cv::Mat& gradient_two);

    //����ɢ��
    cv::Mat divergence_operator(cv::Mat& src);


    



private:
    CalMath();
    virtual ~CalMath();
    static CalMath* instance;

};