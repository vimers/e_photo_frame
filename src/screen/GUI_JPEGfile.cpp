#include "screen/GUI_JPEGfile.h"
#include <opencv2/opencv.hpp>
extern "C" {
#include "screen/Debug.h"
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

    // Convert to 7-color and paint
    for (int y = 0; y < 480; y++) {
        for (int x = 0; x < 800; x++) {
            cv::Vec3b pixel = resized.at<cv::Vec3b>(y, x);
            UBYTE color;

            // Convert RGB to 7-color
            if (pixel[2] < 64 && pixel[1] < 64 && pixel[0] < 64) {
                color = 0; // Black
            } else if (pixel[2] > 192 && pixel[1] > 192 && pixel[0] > 192) {
                color = 1; // White
            } else if (pixel[2] < 64 && pixel[1] > 192 && pixel[0] < 64) {
                color = 2; // Green
            } else if (pixel[2] > 192 && pixel[1] < 64 && pixel[0] < 64) {
                color = 3; // Blue
            } else if (pixel[2] < 64 && pixel[1] < 64 && pixel[0] > 192) {
                color = 4; // Red
            } else if (pixel[2] < 64 && pixel[1] > 192 && pixel[0] > 192) {
                color = 5; // Yellow
            } else if (pixel[2] < 64 && pixel[1] > 64 && pixel[1] < 192 && pixel[0] > 192) {
                color = 6; // Orange
            } else {
                color = 1; // Default to white for undefined colors
            }

            Paint_SetPixel(Xstart + x, Ystart + y, color);
        }
    }

    return 0;
}
