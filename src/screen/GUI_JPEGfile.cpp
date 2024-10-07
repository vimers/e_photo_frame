#include "screen/GUI_JPEGfile.h"
#include "GUI_Paint.h"
#include <opencv2/opencv.hpp>
extern "C" {
#include "screen/Debug.h"
}

// Function to calculate weighted distance in HSV space
double colorDistance(const cv::Vec3b& c1, const cv::Vec3b& c2) {
    double hueWeight = 0.5;
    double satWeight = 0.3;
    double valWeight = 0.2;
    
    double hueDiff = std::min(std::abs(c1[0] - c2[0]), 180 - std::abs(c1[0] - c2[0])) / 180.0;
    double satDiff = std::abs(c1[1] - c2[1]) / 255.0;
    double valDiff = std::abs(c1[2] - c2[2]) / 255.0;
    
    return hueWeight * hueDiff + satWeight * satDiff + valWeight * valDiff;
}

UBYTE GUI_ReadJpeg_RGB_7Color(const char *path, UWORD Xstart, UWORD Ystart)
{
    cv::Mat image = cv::imread(path, cv::IMREAD_COLOR);
    if (image.empty()) {
        Debug("Cannot open image file!\n");
        return 1;
    }

    // Create a white canvas of 800x480
    cv::Mat resized(480, 800, CV_8UC3, cv::Scalar(255, 255, 255));

    // Calculate scaling factor
    double scale = std::min(800.0 / image.cols, 480.0 / image.rows);
    
    // Resize the image
    cv::Mat scaled;
    cv::resize(image, scaled, cv::Size(), scale, scale, cv::INTER_AREA);

    // Calculate position to paste the resized image
    int x_offset = (800 - scaled.cols) / 2;
    int y_offset = (480 - scaled.rows) / 2;

    // Copy the resized image onto the white canvas
    scaled.copyTo(resized(cv::Rect(x_offset, y_offset, scaled.cols, scaled.rows)));

    // Convert to HSV color space
    cv::Mat hsvImage;
    cv::cvtColor(resized, hsvImage, cv::COLOR_BGR2HSV);

    // Define the 7 colors in HSV space
    const cv::Vec3b COLORS[] = {
        {0, 0, 0},       // Black
        {0, 0, 255},     // White
        {60, 255, 255},  // Green
        {120, 255, 255}, // Blue
        {0, 255, 255},   // Red
        {30, 255, 255},  // Yellow
        {15, 255, 255}   // Orange
    };

    // Convert to 7-color and paint
    for (int y = 0; y < 480; y++) {
        for (int x = 0; x < 800; x++) {
            cv::Vec3b pixel = hsvImage.at<cv::Vec3b>(y, x);
            UBYTE color;

            // Find the closest color
            double minDistance = std::numeric_limits<double>::max();
            for (int i = 0; i < 7; i++) {
                double distance = colorDistance(pixel, COLORS[i]);
                if (distance < minDistance) {
                    minDistance = distance;
                    color = i;
                }
            }

            // Special handling for grays
            if (pixel[1] < 30 && pixel[2] > 20) {  // Low saturation and not too dark
                color = (pixel[2] < 128) ? 0 : 1;  // Choose black or white based on value
            }

            Paint_SetPixel(Xstart + x, Ystart + y, color);
        }
    }

    return 0;
}
