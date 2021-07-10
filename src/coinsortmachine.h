#ifndef __COINSORTMACHINE_H
#define __COINSORTMACHINE_H

#include "opencv2/opencv.hpp"
#include "opencv2/imgproc.hpp"

class CoinSortMachine
{
    public:
        CoinSortMachine(cv::Mat inputImage);
        void FindImageEdges();
        void FindImageContours();
        void FitEllipsesToCoins();
        void DisplayTotalMonetaryValue(int, int , int , int);
        void DisplayResults();        
        
    private:
        std::vector<cv::RotatedRect> fittedEllipses;
        cv::Mat _originalImage;
        cv::Mat _processingImage;
        std::vector<std::vector<cv::Point>> _contours;
};
#endif