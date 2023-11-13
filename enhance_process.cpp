#include"enhance_process.h"

EnhanceProcess* EnhanceProcess::Instance()
{
    static EnhanceProcess* instance = nullptr;
    if (nullptr == instance) {
        instance = new EnhanceProcess();
    }
    return instance;
}

EnhanceProcess::EnhanceProcess()
{

}

EnhanceProcess::~EnhanceProcess()
{

}

cv::Mat EnhanceProcess::addGaussianNoise(cv::Mat& image, double mean, double stddev)
{    //image 原始图像 mean  stddev
    // 创建随机数生成器
    std::default_random_engine generator;
    std::normal_distribution<double> distribution(mean, stddev);

    // 添加高斯噪声
    cv::Mat noise_image = image.clone();
    for (int i = 0; i < noise_image.rows; i++)
    {
        for (int j = 0; j < noise_image.cols; j++)
        {
            double noise = distribution(generator);
            int pixel = noise_image.at<uchar>(i, j) + noise;
            if (pixel < 0)
            {
                pixel = 0;
            }
            else if (pixel > 255)
            {
                pixel = 255;
            }
            noise_image.at<uchar>(i, j) = pixel;
        }
    }

    return noise_image;
}

cv::Mat EnhanceProcess::ManyPicAve(cv::Mat& image, int number)
{
    std::vector<cv::Mat> noise_image;
    for (int i = 0; i < number; i++)
    {
        cv::Mat image_clone = image.clone();
        noise_image.push_back(image_clone);
    }
    cv::Mat average;
    if (image.channels() == 3)
    {
        average = cv::Mat::zeros(image.size(), CV_32FC3);
    }
    else
    {
        average = cv::Mat::zeros(image.size(), CV_32FC1);
    }
    
    for (int i = 0; i < noise_image.size(); i++) 
    {
        accumulate(noise_image.at(i), average);//累加到average
    }
    average = average / noise_image.size(); //求出平均图像
    cv::normalize(average, average, 0, 255, cv::NORM_MINMAX);
    average.convertTo(average, CV_8U);
    return average;

}

std::vector<double> EnhanceProcess::cal_rp(cv::Mat& src)
{
    cv::Mat kernel = (cv::Mat_<int>(3, 3) << 1, -2, 1, -2, 4, -2, 1, -2, 1);
    cv::flip(kernel, kernel, -1);
    if (src.channels() == 3)
    {
        std::vector<double> rp;
        std::vector<cv::Mat>mv;
        cv::split(src, mv);
        for (int x = 0; x < mv.size(); x++)
        {
            mv[x].convertTo(mv[x], CV_16SC1);
            cv::Mat mv_dst;
            cv::filter2D(mv[x], mv_dst, -1, kernel, cv::Point(-1, -1), cv::BORDER_DEFAULT);
            double sumvalue = 0;
            for (int i = 0; i < mv_dst.rows; i++)
            {
                signed short* data = mv_dst.ptr<signed short>(i);
                for (int j = 0; j < mv_dst.cols; j++)
                {
                    sumvalue += (double)abs(data[j]);
                }
            }
            double rp_x = sqrt(CV_PI / 2.0) * (1.0 / (6.0 * (double)(mv_dst.cols - 2) * (double)(mv_dst.rows - 2))) * sumvalue;
            rp.push_back(rp_x);

#if 0
            cv::normalize(mv_dst, mv_dst, 0, 255, cv::NORM_MINMAX);
            mv_dst.convertTo(mv_dst, CV_8U);
            std::string aa = std::to_string(x) + "tuxiang";
            cv::namedWindow(aa, cv::WINDOW_FREERATIO);
            cv::imshow(aa, mv_dst);

#endif // 0

        }
        return rp;
    }
    else if (src.channels() == 1)
    {
        std::vector<double> rp;
        cv::Mat mv_dst;
        cv::Mat src_clone = src.clone();
        src.convertTo(src_clone, CV_16SC1);
        cv::filter2D(src_clone, mv_dst, -1, kernel, cv::Point(-1, -1), cv::BORDER_CONSTANT);
        double sumvalue = 0;
        for (int i = 0; i < mv_dst.rows; i++)
        {
            signed short* data = mv_dst.ptr<signed short>(i);
            for (int j = 0; j < mv_dst.cols; j++)
            {
                sumvalue += (double)abs(data[j]);
            }
        }
        double rp_x = sqrt(CV_PI / 2.0) * (1.0 / (6.0 * (double)(mv_dst.cols - 2) * (double)(mv_dst.rows - 2))) * sumvalue;
        rp.push_back(rp_x);
        return rp;
    }
    else
    {
        std::vector<double> rp;
        rp.clear();
        return rp;
    }
}

void EnhanceProcess::calculateGradient(cv::Mat& image, std::vector<cv::Mat>& dx, std::vector<cv::Mat>& dy)
{
    if (image.channels() == 3)
    {
        std::vector<cv::Mat>mv;
        std::vector<cv::Mat>mv_sobel_X(3);
        std::vector<cv::Mat>mv_sobel_Y(3);
        cv::split(image, mv);
        for (int x = 0; x < mv.size(); x++)
        {
            cv::Sobel(image, mv_sobel_X[x], CV_64F, 1, 0);
            cv::Sobel(image, mv_sobel_Y[x], CV_64F, 0, 1);
            dx.push_back(mv_sobel_X[x]);
            dy.push_back(mv_sobel_Y[x]);
        }
        mv_sobel_X.clear();
        mv_sobel_X.shrink_to_fit();
        mv_sobel_Y.clear();
        mv_sobel_Y.shrink_to_fit();
    }
    else if (image.channels() == 1)
    {
        std::vector<cv::Mat>mv_sobel_X(1);
        std::vector<cv::Mat>mv_sobel_Y(1);
        cv::Sobel(image, mv_sobel_X[0], CV_64F, 1, 0);
        cv::Sobel(image, mv_sobel_Y[0], CV_64F, 0, 1);
        dx.push_back(mv_sobel_X[0]);
        dy.push_back(mv_sobel_Y[0]);
        mv_sobel_X.clear();
        mv_sobel_X.shrink_to_fit();
        mv_sobel_Y.clear();
        mv_sobel_Y.shrink_to_fit();
    }
}

std::vector<double> EnhanceProcess::calculateTotalVariation(std::vector<cv::Mat>& dx, std::vector<cv::Mat>& dy)
{
    std::vector<double> tv;
    if (dx.size() == 3 && dy.size() == 3)
    {
        for (int n = 0; n < dx.size(); n++)
        {
            cv::Mat gradientMag;
            cv::magnitude(dx[n], dy[n], gradientMag);
            tv.push_back(cv::sum(gradientMag)[0]);
        }
        return tv;
    }
    else if (dx.size() == 1 && dy.size() == 1)
    {
        std::vector<double> tv;
        cv::Mat gradientMag;
        cv::magnitude(dx[0], dy[0], gradientMag);
        tv.push_back(cv::sum(gradientMag)[0]);
        return tv;
    }
    else
    {
        return tv;
    }
}

cv::Mat EnhanceProcess::get_noise_layers(cv::Mat& src, int ksize)
{
    cv::Mat noise_layer;
    if (src.channels() == 3)
    {
        std::vector<double>rp = cal_rp(src);
        for (int i = 0; i < rp.size(); i++)
        {
            std::cout << i + 1 << "通道rp值：" << rp[i] << std::endl;
        }
        std::vector<cv::Mat> mv;
        cv::split(src, mv);
        std::vector<cv::Mat> noise_layers(mv.size());
        for (int i = 0; i < mv.size(); i++)
        {   
            cv::GaussianBlur(mv[i], noise_layers[i], cv::Size(ksize, ksize), rp[i], rp[i]);
            cv::subtract(mv[i], noise_layers[i], noise_layers[i]);
            noise_layers[i] = noise_layers[i] * 60;
        }
        
        cv::merge(noise_layers, noise_layer);
        cv::namedWindow("噪声层", cv::WINDOW_FREERATIO);
        cv::imshow("噪声层", noise_layer);
        return noise_layer;
    }
    else if (src.channels() == 1)
    {
        std::vector<double>rp = cal_rp(src);
        std::cout  << "rp值：" << rp[0] << std::endl;
        cv::GaussianBlur(src, noise_layer, cv::Size(ksize, ksize), rp[0], rp[0]);
        cv::subtract(src, noise_layer, noise_layer);
        cv::namedWindow("噪声层", cv::WINDOW_FREERATIO);
        cv::imshow("噪声层", noise_layer * 8);
        return noise_layer * 8;
    }
    else
    {
        noise_layer = cv::Mat(src.size(), CV_8UC1, cv::Scalar(255));
        cv::namedWindow("噪声层", cv::WINDOW_FREERATIO);
        cv::imshow("噪声层", noise_layer);
        return noise_layer;
    }   
}

cv::Mat EnhanceProcess::get_structrue_layers(cv::Mat& src, int ksize)
{
    cv::Mat structrue_layer;
    if (src.channels() == 3)
    {
        std::vector<double>rp = cal_rp(src);
        for (int i = 0; i < rp.size(); i++)
        {
            std::cout << i + 1 << "通道rp值：" << rp[i] << std::endl;
        }
        std::vector<cv::Mat> mv;
        cv::split(src, mv);
        std::vector<cv::Mat> structrue_layers(mv.size());
        for (int i = 0; i < mv.size(); i++)
        {
            cv::GaussianBlur(mv[i], structrue_layers[i], cv::Size(ksize, ksize), rp[i], rp[i]);
        }
        cv::merge(structrue_layers, structrue_layer);
        cv::namedWindow("结构层", cv::WINDOW_FREERATIO);
        cv::imshow("结构层", structrue_layer);
        return structrue_layer;
    }
    else if (src.channels() == 1)
    {
        std::vector<double>rp = cal_rp(src);
        std::cout << "rp值：" << rp[0] << std::endl;
        cv::GaussianBlur(src, structrue_layer, cv::Size(ksize, ksize), rp[0], rp[0]);
        cv::namedWindow("结构层", cv::WINDOW_FREERATIO);
        cv::imshow("结构层", structrue_layer);
        return structrue_layer;
    }
    else
    {
        structrue_layer = cv::Mat(src.size(), CV_8UC1, cv::Scalar(255));
        cv::namedWindow("结构层", cv::WINDOW_FREERATIO);
        cv::imshow("结构层", structrue_layer);
        return structrue_layer;
    }
}






cv::Mat EnhanceProcess::enhance_image(cv::Mat& src)
{
    cv::Mat src_gray = cv::Mat::zeros(src.size(), src.type());
    if (src.channels() == 3)
    {
        cv::cvtColor(src, src_gray, cv::COLOR_BGR2GRAY);
    }
    else if (src.channels() == 1)
    {
        src_gray = src.clone();
    }
    else
    {
        return src_gray;
    }
    cv::Scalar mean,stddev;
    cv::meanStdDev(src_gray, mean, stddev);
    float u = mean[0];
    cv::Mat src_gray_32F;
    src_gray.convertTo(src_gray_32F, CV_32F);
    cv::Mat src_gray_median = cv::Mat::zeros(src_gray.size(), src_gray.type());
    cv::medianBlur(src_gray, src_gray_median, 77);
    cv::Mat src_gray_median_32F;
    src_gray_median.convertTo(src_gray_median_32F, CV_32F);
    cv::Mat miu = cv::Mat(src_gray_32F.size(), src_gray_32F.type(), cv::Scalar(u));
#if 0
    for (int i = 0; i < src_gray_median.rows; i++)
    {
        float* data = src_gray_median.ptr<float>(i);
        for (int j = 0; j < src_gray_median.cols; j++)
        {
            std::cout << data[j] << std::endl;
        }
    }
#endif // 0
    cv::Mat Ii;
    cv::subtract(src_gray_32F, src_gray_median_32F, Ii);
    cv::add(Ii, miu, Ii);
    cv::Mat Ii_norm;
    cv::normalize(Ii, Ii_norm, 20, 235, cv::NORM_MINMAX);


    Ii_norm.convertTo(Ii_norm, CV_8U);
    cv::namedWindow("sdadad", cv::WINDOW_FREERATIO);
    cv::imshow("sdadad", Ii_norm);
    cv::imwrite("C:\\Users\\Administrator\\Desktop\\sdadad_20.bmp", Ii_norm);










}


void EnhanceProcess::artifical_color(cv::Mat& green, cv::Mat& red)
{
    std::vector<cv::Mat> mvt;
    std::vector<cv::Mat> mvt_yellow;
    cv::Mat yellow;
    cv::Mat black = cv::Mat::zeros(green.size(), green.type());
    mvt_yellow.push_back(black);
    mvt_yellow.push_back(green);
    mvt_yellow.push_back(red);
    cv::merge(mvt_yellow, yellow);
    cv::Mat yellow_gray;
    cv::cvtColor(yellow, yellow_gray, cv::COLOR_BGR2GRAY);
    mvt.push_back(green);
    mvt.push_back(yellow_gray);
    mvt.push_back(red);
    cv::Mat color;
    cv::merge(mvt, color);
    cv::namedWindow("yellow_", cv::WINDOW_FREERATIO);
    cv::imshow("yellow_", yellow);
    cv::imwrite("C:\\Users\\Administrator\\Desktop\\yellow.bmp", yellow);

}