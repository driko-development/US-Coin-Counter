#include "coinsortmachine.h"

CoinSortMachine::CoinSortMachine(cv::Mat inputImage) : _originalImage{inputImage}
{
}

void CoinSortMachine::FindImageEdges()
{
    // Convert to grayscale
    cv::cvtColor(_originalImage, _processingImage, cv::COLOR_BGR2GRAY);

    // Find canny edges via threshold
    const double cannyThreshold1 = 50;
    const double cannyThreshold2 = 200;
    const int cannyAperture = 3;

    cv::Canny(_processingImage, _processingImage, cannyThreshold1, cannyThreshold2, cannyAperture);
}

void CoinSortMachine::FindImageContours()
{
    // erode and dilate the edges to remove noise
    int morphologySize = 1;

    // Dilate the image first
    cv::dilate(_processingImage, _processingImage, cv::Mat(), cv::Point(-1, -1), morphologySize);

    // Then erode
    cv::erode(_processingImage, _processingImage, cv::Mat(), cv::Point(-1, -1), morphologySize);

    // Finally with noise gone, find the contours
    cv::findContours(_processingImage, _contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE, cv::Point(0, 0));
}

void CoinSortMachine::FitEllipsesToCoins()
{
    // fit ellipses to contours containing sufficient inliers
    fittedEllipses = std::vector<cv::RotatedRect>{_contours.size()};
    
    for(int i = 0; i < _contours.size(); i++)
    {
        // compute an ellipse only if the contour has more than 5 points (the minimum for ellipse fitting)
        if(_contours.at(i).size() > 5)
        {
            fittedEllipses[i] = cv::fitEllipse(_contours[i]);
        }
    }
}

void CoinSortMachine::DisplayTotalMonetaryValue(int q, int d, int n, int p)
{
    std::cout << "Penny - " << p << std::endl;
    std::cout << "Nickel - " << n << std::endl;
    std::cout << "Dime - " << d << std::endl;
    std::cout << "Quarter - " << q << std::endl;
    std::cout << "Total - " << ((q * 0.25)+(d * 0.10)+(n * 0.05)+(p*0.01))<< std::endl;
}

void CoinSortMachine::DisplayResults()
{
    // Variables to keep track of coin count
    int quarter = 0, dime = 0, nickel = 0, penny = 0;

    const int minEllipseInliers = 50;
    for(int i = 0; i < _contours.size(); i++)
    {
        // draw any ellipse with sufficient inliers
        if(_contours.at(i).size() > minEllipseInliers)
        {
            cv::Scalar color;

            // Use the perimeter size to determine colors
            double perimeter_i = cv::arcLength(_contours.at(i), true); 

            // Green color for quarters
            if (perimeter_i > 350)
            {
                quarter++;
                color = cv::Scalar(0, 255, 0);
            }
            // Yellow color for nickels
            else if (perimeter_i > 300)
            {
                nickel++;
                color = cv::Scalar(0,255,255);
            }
            // Red color for pennies
            else if (perimeter_i > 275)
            {
                penny++;
                color = cv::Scalar(0,0,255);
            }
            // Blue color for dimes
            else
            {     
                dime++;             
                color = cv::Scalar(255, 0, 0);
            }   
            cv::ellipse(_originalImage, fittedEllipses[i], color, 2);
        }
    }

    // Display the coin count and the corresponding total
    DisplayTotalMonetaryValue(quarter, dime, nickel, penny);

    // Display the original image with the ellipses drawn on
    cv::imshow("Coins", _originalImage);
        
    cv::waitKey();
}