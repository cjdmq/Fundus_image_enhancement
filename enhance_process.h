#pragma once

#include <vector>
#include<random>
#include "opencv2/core/core.hpp"
#include <opencv2/opencv.hpp>
#include"mathcal.h"

class EnhanceProcess
{
public:

    static EnhanceProcess* Instance();


    cv::Mat addGaussianNoise(cv::Mat& image, double mean, double stddev);

    /** @XD hyit-bigvision
    *
    src is input image
    return output image
    */
    cv::Mat ManyPicAve(cv::Mat& image, int number);

    /** @XD hyit-bigvision
    * 
    construct the regulatory parameter rp
    if src channels is three  return vector'size is three
        0: channel  blue's rp
        1: channel  green's rp
        2: channel  red's rp
    if src channels is one  return vector'size is one
        0: channel  src's rp
    if src channels is NULL  return vector'size is zero
    */
    std::vector<double> cal_rp(cv::Mat& src);

    /** @XD hyit-bigvision
    *
    calculate parameter gradient
    if src channels is three  return vector'size is three
        0: channel  blue's gradient
        1: channel  green's gradient
        2: channel  red's gradient
    if src channels is one  return vector'size is one
        0: channel  src's gradient
    if src channels is NULL  return vector'size is zero
    */
    void calculateGradient(cv::Mat& image, std::vector<cv::Mat>& dx, std::vector<cv::Mat>& dy);

    /** @XD hyit-bigvision
    *
    calculate parameter TV
    if src channels is three  return vector'size is three
        0: channel  blue's TV
        1: channel  green's TV
        2: channel  red's TV
    if src channels is one  return vector'size is one
        0: channel  src's TV
    if src channels is NULL  return vector'size is zero
    */
    std::vector<double> calculateTotalVariation(std::vector<cv::Mat>& dx, std::vector<cv::Mat>& dy);

    /** @XD hyit-bigvision
    *
    get noise layers
    src is input image
    ksize is gaussian kernel size
    return noise layers image
    */
    cv::Mat get_noise_layers(cv::Mat& src, int ksize);

   /** @XD hyit-bigvision
    *
    get structrue layers
    src is input image
    ksize is gaussian kernel size
    return structrue layers image
    */
    cv::Mat get_structrue_layers(cv::Mat& src, int ksize);












    /** @XD hyit-bigvision
    *
    src is source image
    return Mat is enhanced image
    if Mat is a black image, illustrate src is not 1 or 3
    channels image
    */
    cv::Mat enhance_image(cv::Mat& src);


    void artifical_color(cv::Mat& green, cv::Mat& red);



private:
    EnhanceProcess();
    virtual ~EnhanceProcess();
    static EnhanceProcess* instance;

};