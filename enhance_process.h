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


    
    //************************************
    // Company:   BigVision
    // Author:    Xu Duo
    // Date:	  2023/11/13
    // Method:    addGaussianNoise
    // FullName:  EnhanceProcess::addGaussianNoise
    // Access:    public 
    // Returns:   cv::Mat
    // Qualifier:
    // Parameter: cv::Mat & image
    // Parameter: double mean
    // Parameter: double stddev
    //************************************
    cv::Mat addGaussianNoise(cv::Mat& image, double mean, double stddev);

    
    //************************************
    // Company:   BigVision
    // Author:    Xu Duo
    // Date:	  2023/11/13
    // Method:    ManyPicAve
    // FullName:  EnhanceProcess::ManyPicAve
    // Access:    public 
    // Returns:   cv::Mat
    // Qualifier:
    // Parameter: cv::Mat & image
    // Parameter: int number
    //************************************
    cv::Mat ManyPicAve(cv::Mat& image, int number);

   
    //************************************
    // Company:   BigVision
    // Author:    Xu Duo
    // Date:	  2023/11/13
    // Method:    cal_rp
    // FullName:  EnhanceProcess::cal_rp
    // Access:    public 
    // Returns:   std::vector<double>
    // Qualifier:
    // Parameter: cv::Mat & src
    //************************************
    std::vector<double> cal_rp(cv::Mat& src);

    
    //************************************
    // Company:   BigVision
    // Author:    Xu Duo
    // Date:	  2023/11/13
    // Method:    calculateGradient
    // FullName:  EnhanceProcess::calculateGradient
    // Access:    public 
    // Returns:   void
    // Qualifier:
    // Parameter: cv::Mat & image
    // Parameter: std::vector<cv::Mat> & dx
    // Parameter: std::vector<cv::Mat> & dy
    //************************************
    void calculateGradient(cv::Mat& image, std::vector<cv::Mat>& dx, std::vector<cv::Mat>& dy);

    
    //************************************
    // Company:   BigVision
    // Author:    Xu Duo
    // Date:	  2023/11/13
    // Method:    calculateTotalVariation
    // FullName:  EnhanceProcess::calculateTotalVariation
    // Access:    public 
    // Returns:   std::vector<double>
    // Qualifier:
    // Parameter: std::vector<cv::Mat> & dx
    // Parameter: std::vector<cv::Mat> & dy
    //************************************
    std::vector<double> calculateTotalVariation(std::vector<cv::Mat>& dx, std::vector<cv::Mat>& dy);

    
    //************************************
    // Company:   BigVision
    // Author:    Xu Duo
    // Date:	  2023/11/13
    // Method:    get_noise_layers
    // FullName:  EnhanceProcess::get_noise_layers
    // Access:    public 
    // Returns:   cv::Mat
    // Qualifier:
    // Parameter: cv::Mat & src
    // Parameter: int ksize
    //************************************
    cv::Mat get_noise_layers(cv::Mat& src, int ksize);

   
    //************************************
    // Company:   BigVision
    // Author:    Xu Duo
    // Date:	  2023/11/13
    // Method:    get_structrue_layers
    // FullName:  EnhanceProcess::get_structrue_layers
    // Access:    public 
    // Returns:   cv::Mat
    // Qualifier:
    // Parameter: cv::Mat & src
    // Parameter: int ksize
    //************************************
    cv::Mat get_structrue_layers(cv::Mat& src, int ksize);

    //************************************
    // Company:   BigVision
    // Author:    Xu Duo
    // Date:	  2023/11/13
    // Method:    enhance_image
    // FullName:  EnhanceProcess::enhance_image
    // Access:    public 
    // Returns:   cv::Mat
    // Qualifier:
    // Parameter: cv::Mat & src
    //************************************
    cv::Mat enhance_image(cv::Mat& src);


    //************************************
    // Company:   BigVision
    // Author:    Xu Duo
    // Date:	  2023/11/13
    // Method:    artifical_color
    // FullName:  EnhanceProcess::artifical_color
    // Access:    public 
    // Returns:   void
    // Qualifier:
    // Parameter: cv::Mat & green
    // Parameter: cv::Mat & red
    //************************************
    void artifical_color(cv::Mat& green, cv::Mat& red);





























private:
    EnhanceProcess();
    virtual ~EnhanceProcess();
    static EnhanceProcess* instance;

};