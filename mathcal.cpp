#include"mathcal.h"

CalMath* CalMath::Instance()
{
    static CalMath* instance = nullptr;
    if (nullptr == instance) {
        instance = new CalMath();
    }
    return instance;
}

CalMath::CalMath()
{

}

CalMath::~CalMath()
{

}


cv::Mat CalMath::gradient_one(cv::Mat& src)
{
    src.convertTo(src, CV_16S);
    cv::Mat dst;
    if (src.channels() != 1)
    {
        dst = cv::Mat(src.size(), src.type(), cv::Scalar(255));
        return dst;
    }
    else
    {
        dst = cv::Mat::zeros(src.size(), src.type());
        for (int i = 0; i < src.rows; i++)
        {
            signed short* data_dst = dst.ptr<signed short>(i);
            for (int j = 0; j < src.cols; j++)
            {
                if (i < src.rows - 1)
                {
                    data_dst[j] = src.ptr<signed short>(i + 1)[j] - src.ptr<signed short>(i)[j];
                }
                else
                {
                    data_dst[j] = 0;
                }
            } 
        }
        return dst;
    }
}

cv::Mat CalMath::gradient_two(cv::Mat& src)
{
    src.convertTo(src, CV_16S);
    cv::Mat dst;
    if (src.channels() != 1)
    {
        dst = cv::Mat(src.size(), src.type(), cv::Scalar(255));
        return dst;
    }
    else
    {
        dst = cv::Mat::zeros(src.size(), src.type());
        for (int i = 0; i < src.rows; i++)
        {
            signed short* data_dst = dst.ptr<signed short>(i);
            for (int j = 0; j < src.cols; j++)
            {
                if (j < src.cols - 1)
                {
                    data_dst[j] = src.ptr<signed short>(i)[j + 1] - src.ptr<signed short>(i)[j];
                }
                else
                {
                    data_dst[j] = 0;
                }
            }
        }
        return dst;
    }
}

double CalMath::total_variation(cv::Mat& gradient_one, cv::Mat& gradient_two)
{
    double total_variation = 0.0;
    if (gradient_one.channels() != 1 || gradient_two.channels() != 1)
    {
        return total_variation;
    }
    else
    {
        cv::Mat g1,g2;
        gradient_one.convertTo(g1, CV_64F);
        gradient_two.convertTo(g2, CV_64F);
        cv::Mat mag = cv::Mat(g1.size(), g1.type());
        cv::magnitude(g1, g2, mag);
        //std::cout << mag << std::endl;
        for (int i = 0; i < gradient_one.rows; i++)
        {
            double* data = mag.ptr<double>(i);
            for (int j = 0; j < gradient_one.cols; j++)
            {
                total_variation += data[j];
            }
        }
        return total_variation;
    }
}

cv::Mat CalMath::divergence_operator(cv::Mat& src)
{
    cv::Mat dst;
    if (src.channels() != 1)
    {
        dst = cv::Mat(src.size(), src.type(), cv::Scalar(255));
        return dst;
    }

    else
    {
        cv::Mat g1 = gradient_one(src);
        cv::Mat g2 = gradient_two(src);
        cv::Mat div1 = cv::Mat::zeros(g1.size(), g1.type());
        cv::Mat div2 = cv::Mat::zeros(g2.size(), g2.type());
        for (int i = 0; i < g1.rows; i++)
        {
            signed short* data_div1 = div1.ptr<signed short>(i);
            for (int j = 0; j < g1.cols; j++)
            {
                if (i == 0)
                {
                    data_div1[j] = g1.ptr<signed short>(i)[j];
                }
                else if (i > 0 && i < g1.rows - 1)
                {
                    data_div1[j] = g1.ptr<signed short>(i)[j] - g1.ptr<signed short>(i - 1)[j];
                }
                else
                {
                    data_div1[j] = -g1.ptr<signed short>(i - 1)[j];
                }
            }
        }
        std::cout << "div1" << std::endl;
        std::cout << div1 << std::endl;

        for (int i = 0; i < g2.rows; i++)
        {
            signed short* data_div2 = div2.ptr<signed short>(i);
            for (int j = 0; j < g2.cols; j++)
            {
                if (j == 0)
                {
                    data_div2[j] = g2.ptr<signed short>(i)[j];
                }
                else if (j > 0 && j < g2.cols - 1)
                {
                    data_div2[j] = g2.ptr<signed short>(i)[j] - g2.ptr<signed short>(i)[j - 1];
                    //std::cout << data_div2[j] << " = "<< g2.ptr<signed short>(i)[j]<<" - "<< g2.ptr<signed short>(i)[j - 1]<<",";
                }
                else
                {
                    data_div2[j] = -g2.ptr<signed short>(i)[j - 1];
                }
            }
            //std::cout << std::endl;
        }

        std::cout << "div2" << std::endl;
        std::cout << div2 << std::endl;
        dst = cv::Mat::zeros(div1.size(), div1.type());
        for (int i = 0; i < dst.rows; i++)
        {
            signed short* data_dst = dst.ptr<signed short>(i);
            signed short* data_div1 = div1.ptr<signed short>(i);
            signed short* data_div2 = div2.ptr<signed short>(i);
            for (int j = 0; j < dst.cols; j++)
            {
                data_dst[j] = data_div1[j] + data_div2[j];
            }
        }
        return dst;
    }
}