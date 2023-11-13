#include<opencv2/opencv.hpp>
#include<opencv2/highgui.hpp>
#include<iostream>
#include<string>
#include<vector>
#include<time.h>
#include<algorithm>
#include<stdio.h>
#include<opencv2/imgproc/imgproc.hpp>
#include<opencv2/features2d/features2d.hpp>
#include<stdint.h>
#include<opencv2/core.hpp>
#include<numeric>
#include<iomanip>
#include<stack>
#include <time.h>
#include<thread>
#include<chrono>
#include"enhance_process.h"
#include"imagej_threshold.h"
#include"mathcal.h"





int main(int argc, char** argv)

{
    //测试数据类型大小
#if 0
        std::cout << "Minimum value of signed char: " << static_cast<int>(std::numeric_limits<signed short>::min()) << std::endl;
        std::cout << "Maximum value of signed char: " << static_cast<int>(std::numeric_limits<signed short>::max()) << std::endl;

#endif // 0



    cv::Mat image = cv::imread("C:\\Users\\Administrator\\Desktop\\微信图片_20230829100149.bmp", 0);
    cv::Mat image2 = cv::imread("C:\\Users\\Administrator\\Desktop\\20230713005_徐多\\2023_07_14\\IR\\2023_07_14_16_59_00.395_IR.jpg", 0);
    cv::Mat image3 = cv::imread("C:\\Users\\Administrator\\Desktop\\20230713005_徐多\\2023_07_14\\RF\\2023_07_14_16_59_00.582_RF.jpg", 0);
    cv::Mat image4 = cv::imread("C:\\Users\\Administrator\\Desktop\\20230713005_徐多\\2023_07_14\\MCOLOR\\2023_07_14_16_59_04.933_MCOLOR.jpg", cv::IMREAD_COLOR);
    cv::Mat image5 = cv::imread("C:\\Users\\Administrator\\Desktop\\眼底图像增强文献\\data_eye\\healthy_eye1\\14_h.jpg", cv::IMREAD_COLOR);
    cv::Mat green = cv::imread("C:\\Users\\Administrator\\Desktop\\green.jpg",0);
    cv::Mat red = cv::imread("C:\\Users\\Administrator\\Desktop\\red.jpg", 0);

    


    if (!image2.data)
    {
        std::cout << "没有检测到图片，请检查文件路径是否正确！" << std::endl;
        //return -1;
    }


#if 0

    
    EnhanceProcess::Instance()->artifical_color(image3, image2);


#endif // 0














    //BB的算法
#if 0
    cv::Mat image1 = cv::imread("C:\\Users\\Administrator\\Desktop\\微信图片_20230824111102.bmp", cv::IMREAD_COLOR);
    for (int i = 0; i < image1.rows; i++)
    {
        for (int j = 0; j < image1.cols; j++)
        {
            if (image1.at<cv::Vec3b>(i, j) == cv::Vec3b(0, 0, 0))
            {
                image1.at<cv::Vec3b>(i, j) = cv::Vec3b(255, 255, 251);
            }
            else
            {
                image1.at<cv::Vec3b>(i, j) = cv::Vec3b(14, 14, 150);
            }
        }
    }
    cv::Mat image2;
    cv::resize(image1, image2, image1.size(), 0, 0, cv::INTER_LINEAR);
    cv::imwrite("C:\\Users\\Administrator\\Desktop\\微信图片_20.bmp", image2);
    cv::namedWindow("QIAN", cv::WINDOW_FREERATIO);
    cv::imshow("QIAN", image2);

#endif // 0


   
    
    


#if 1
    cv::Mat src_prenoise = cv::imread("C:\\Users\\Administrator\\Desktop\\徐多\\眼底图像增强文献\\data_eye\\healthy_eye1\\04_h.jpg", 1);
    //cv::Mat src_prenoise = cv::imread("C:\\Users\\Administrator\\Desktop\\眼底图像.bmp", 0);

    //cv::Mat noise_image = EnhanceProcess::Instance()->addGaussianNoise(src_prenoise, 0, 20);
    //cv::Mat noise_image_color;
    //cv::cvtColor(noise_image, noise_image_color, cv::COLOR_GRAY2BGR);

    
    cv::Mat noise_layer = EnhanceProcess::Instance()->get_noise_layers(src_prenoise, 3);
    cv::Mat structrue_layer = EnhanceProcess::Instance()->get_structrue_layers(src_prenoise, 3);
    cv::Mat testimage = (cv::Mat_<uchar>(4, 3) << 1, 2, 1, 2, 4, 2, 1, 2, 1, 2, 6, 5);
    
    cv::Mat rezult_g1 = CalMath::Instance()->gradient_one(testimage);
    cv::Mat rezult_g2 = CalMath::Instance()->gradient_two(testimage);
    double tv = CalMath::Instance()->total_variation(rezult_g1, rezult_g2);
    cv::Mat rezult_div = CalMath::Instance()->divergence_operator(testimage);
    std::cout << "原始：" << std::endl;
    std::cout << testimage << std::endl;
    std::cout << "g1：" << std::endl;
    std::cout << rezult_g1 << std::endl;
    std::cout << "g2：" << std::endl;
    std::cout << rezult_g2 << std::endl;
    std::cout << "总变差：" << tv << std::endl;
    std::cout << "rezult_div：" << std::endl;
    std::cout << rezult_div << std::endl;

    cv::namedWindow("src", cv::WINDOW_FREERATIO);
    cv::imshow("src", src_prenoise);

    //保存
#if 0
    cv::imwrite("C:\\Users\\Administrator\\Desktop\\噪声图像.bmp", noise_layer);
    cv::imwrite("C:\\Users\\Administrator\\Desktop\\结构图像.bmp", structrue_layer);


#endif // 0

    
    
    
   

    
    


#endif // 1



#if 0

    cv::Mat blue = cv::Mat(cv::Size(500, 500), CV_8UC1, cv::Scalar(206));
    cv::Mat RED = cv::Mat(cv::Size(500, 500), CV_8UC1, cv::Scalar(154));
    cv::Mat gren = cv::Mat(cv::Size(500, 500), CV_8UC1, cv::Scalar(54));
    std::vector<cv::Mat> mvt;
    mvt.push_back(blue);
    mvt.push_back(gren);
    mvt.push_back(RED);
    cv::Mat ssssss;
    cv::merge(mvt, ssssss);
    cv::namedWindow("sss", cv::WINDOW_FREERATIO);
    cv::imshow("sss", ssssss);
#endif // 1

    //淮阴工学院透明标
#if 0
    cv::Mat image_hyit = cv::imread("C:\\Users\\Administrator\\Desktop\\微信图片_20230918164312.bmp",0);
    cv::Mat input_bgra;
    cv::cvtColor(image_hyit, input_bgra, cv::COLOR_GRAY2BGRA);

    // find all white pixel and set alpha value to zero:
    for (int y = 0; y < input_bgra.rows; ++y)
        for (int x = 0; x < input_bgra.cols; ++x)
        {
            cv::Vec4b& pixel = input_bgra.at<cv::Vec4b>(y, x);
            // if pixel is white
            if (pixel[0] == 0 && pixel[1] == 0 && pixel[2] == 0)
            {
                // set alpha to zero:
                pixel[3] = 0;
            }
            if (pixel[0] == 255 && pixel[1] == 255 && pixel[2] == 255)
            {
                // set alpha to zero:
                pixel[0] = 0;
                pixel[1] = 0;
                pixel[2] = 0;
            }
        }

    // save as .png file (which supports alpha channels/transparency)
    cv::imwrite("C:\\Users\\Administrator\\Desktop\\c.png", input_bgra);
    

#endif // 1


    /*
    cv::namedWindow("QIAN2", cv::WINDOW_FREERATIO);
    cv::imshow("QIAN2", noise_image);*/
   



    
    cv::waitKey(0);
    cv::destroyAllWindows();
    return 0;




}