#include <opencv2/opencv.hpp>
#include <iostream>

struct LightParams 
{
    float min_ratio = 0.1;
    float max_ratio = 0.4;
    float max_angle = 40.0;
};

void detectLights(const cv::Mat& src, const LightParams& params) 
{
    cv::Mat gray, binary;
    
    cv::cvtColor(src, gray, cv::COLOR_BGR2GRAY);
    cv::threshold(gray, binary, 200, 255, cv::THRESH_BINARY);

    std::vector<std::vector<cv::Point>> contours;
    cv::findContours(binary, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

    for (const auto& contour : contours) 
    {
        cv::RotatedRect rect = cv::minAreaRect(contour);
        float width = std::min(rect.size.width, rect.size.height);
        float length = std::max(rect.size.width, rect.size.height);
        float angle = rect.angle;

        float ratio = width / length;
        bool is_light = (params.min_ratio < ratio && ratio < params.max_ratio) && std::abs(angle) < params.max_angle;
        
        if (is_light) {
            cv::Point2f vertices[4];
            rect.points(vertices);
            for (int i = 0; i < 4; i++) 
            {
                cv::line(src, vertices[i], vertices[(i + 1) % 4], cv::Scalar(0, 255, 0), 2);
            }

            cv::Rect bounding_rect = rect.boundingRect();
            bounding_rect &= cv::Rect(0, 0, src.cols, src.rows);
            cv::Mat roi = src(bounding_rect);
            cv::Scalar mean_color = cv::mean(roi);
            
            std::string color = (mean_color[2] > mean_color[0]) ? "Red" : "Blue";
            std::cout << "Detected a " << color << " light!" << std::endl;

            cv::putText(src, color, vertices[0], cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(255, 0, 0), 1);
        }
    }

    cv::imshow("Detected Lights", src);
    cv::waitKey(0);
}

int main()
{
    cv::Mat image = cv::imread("/home/ktfine/pnxhomework/lightrecognition/image/image1.png");
    if (image.empty()) 
    {
        std::cerr << "Error loading image!" << std::endl;
        return -1;
    }

    LightParams params;
    detectLights(image, params);

    return 0;
}
